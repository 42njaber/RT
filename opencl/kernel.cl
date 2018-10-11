
#include "math.clh"
#include "hit_equations.clh"

int						can_trace(__global int *obj_type, __global float16 *obj_transform, __global float16	*obj_rot_mat, int nobjs, float3 v1, float3 v2);
float4					get_point_color(__global int *obj_type, __global uchar4 *obj_color, __global float16 *obj_transform, __global float16 *obj_rot_mat,
										__global float16 *obj_rev_rot, __global float3 *spot_pos, __global float *spot_lum, int nspots, int nobjs, int obj_hit,
										float3 v, float3 dir);

int						can_trace(
						   __global int		*obj_type,
						   __global float16	*obj_transform,
						   __global float16	*obj_rot_mat,
						   int				nobjs,
						   float3			v1,
						   float3			v2
						)
{
	float3			line;
	float3			ori_tmp;
	float3			dir_tmp;
	float			len;
	float			tmp;
	int				i;

	line = v2 - v1;
	i = -1;
	// Iterate throught all the objects to detect the first one hit by the ray
	while (++i < nobjs)
	{
		ori_tmp = vec_mat_mult(obj_transform[i], v1);
		dir_tmp = vec_mat_mult(obj_rot_mat[i], line);
		len = length(dir_tmp);
		dir_tmp = normalize(dir_tmp);
		if (((obj_type[i] == 0 && sphere_hit(ori_tmp, dir_tmp, &tmp) == 1) ||
			(obj_type[i] == 1 && plane_hit(ori_tmp, dir_tmp, &tmp) == 1) ||
			(obj_type[i] == 2 && cylinder_hit(ori_tmp, dir_tmp, &tmp) == 1) ||
			(obj_type[i] == 3 && cone_hit(ori_tmp, dir_tmp, &tmp) == 1) ||
			(obj_type[i] == 4 && torus_hit(ori_tmp, dir_tmp, &tmp) == 1) ||
			(obj_type[i] == 5 && moebius_hit(ori_tmp, dir_tmp, &tmp) == 1)) &&
				tmp < len - 0.0001)
			return (0);
	}
	return (1);
}

float4					get_point_color(
								   __global int *obj_type,
								   __global uchar4 *obj_color,
								   __global float16 *obj_transform,
								   __global float16 *obj_rot_mat,
								   __global float16 *obj_rev_rot,
								   __global float3 *spot_pos,
								   __global float *spot_lum,
								   int nspots,
								   int nobjs,
								   int obj_hit,
								   float3 v,
								   float3 dir
								  )
{
	float3			normal;
	float3			r_in;
	//float3			r_out;
	float			a_in;
	float			brilliance;
	float			lum;
	float4			color;
	int				i;

	lum = 0.15;
	brilliance = 0;
	i = -1;
	normal = get_normal(obj_type[obj_hit], obj_transform[obj_hit], obj_rev_rot[obj_hit], v, dir);
	while (++i < nspots)
		if (can_trace(obj_type, obj_transform, obj_rot_mat, nobjs, spot_pos[i], v))
		{
			r_in = v - spot_pos[i];
			a_in = dot(normalize(r_in), normal);
			//r_out = normalize(normalize(r_in) - (normal * 2 * a_in));
			//brilliance = pow(dot(normalize(r_in), normal) , (float)400);
			if (a_in > 0)
				lum += spot_lum[i] * a_in / pow(length(r_in), (float)2) + brilliance * spot_lum[i];
		}
	color = (float4)(lum * ((float)1 + obj_color[obj_hit].r) / 255, lum * ((float)1 + obj_color[obj_hit].g) / 255,
					lum * ((float)1 + obj_color[obj_hit].b) / 255, lum * ((float)1 + obj_color[obj_hit].a) / 255);
	//if (fabs(fmod(v.x + 0.2, (float)20)) < 0.03 || fabs(fmod(v.y + 0.2, (float)20)) < 0.03 || fabs(fmod(v.z + 0.2, (float)20)) < 0.03)
		//color = 1 / color;
	return (color);
}

__kernel void			process_image(
						   __global uchar4	*buf,
						   __global int		*obj_type,
						   __global uchar4	*obj_color,
						   __global float16	*obj_transform,
						   __global float16	*obj_rot_mat,
						   __global float16	*obj_rev_rot,
						   __global float	*obj_reflectivness,
						   __global float	*spot_lum,
						   __global float3	*spot_pos,
						   int2				img_size,
						   float16			cam_mat,
						   float16			cam_mat_rot,
						   int				nobjs,
						   int				nspots,
						   int				max_rays,
						   float			far,
						   float			fov
						  )
{
	int2			id;
	int				buf_pos;
	float			s;

	if ((id.x = get_global_id(0)) >= img_size.x
			|| (id.y = get_global_id(1)) >= img_size.y)
		return ;
	buf_pos = id.x + img_size.x * id.y;
	s = 1 / tan((float)(fov * 0.5 * M_PI_F / 180));

	// Creation of origin and direction vectors of the ray
	float3			ori;
	float3			dir;

	ori = (float3)((float)(id.x - img_size.x / 2) / (max(img_size.x, img_size.y) / 2) / s,
				   (float)(id.y - img_size.y / 2) / (max(img_size.x, img_size.y) / 2) / s, 1);
	dir = normalize(ori);
	ori = vec_mat_mult(cam_mat, ori);
	dir = vec_mat_mult(cam_mat_rot, dir);

	int				reflected;
	float			reflect_amount;
	float4			color;

	color = (float4)0;
	reflected = 0;
	reflect_amount = 1;
	while (reflected++ < max_rays && reflect_amount > 0.05)
	{
		// Iterate throught all the objects to detect the first one hit by the ray
		float3			ori_tmp;
		float3			dir_tmp;
		float			hit;
		float			tmp;
		int				i;
		int				obj_hit;
		float3			v;

		i = -1;
		hit = far + 1;
		obj_hit = -1;
		while (++i < nobjs)
		{
			ori_tmp = vec_mat_mult(obj_transform[i], ori);
			dir_tmp = vec_mat_mult(obj_rot_mat[i], dir);
			if (((obj_type[i] == 0 && sphere_hit(ori_tmp, dir_tmp, &tmp) == 1) ||
				(obj_type[i] == 1 && plane_hit(ori_tmp, dir_tmp, &tmp) == 1) ||
				(obj_type[i] == 2 && cylinder_hit(ori_tmp, dir_tmp, &tmp) == 1) ||
				(obj_type[i] == 3 && cone_hit(ori_tmp, dir_tmp, &tmp) == 1) ||
				(obj_type[i] == 4 && torus_hit(ori_tmp, dir_tmp, &tmp) == 1) ||
				(obj_type[i] == 5 && moebius_hit(ori_tmp, dir_tmp, &tmp) == 1)) &&
					tmp < hit)
			{
				hit = tmp;
				obj_hit = i;
			}
		}

		// Creates à halo of light when ray passes close to a spot
		float	shine;
		float	spot_dist;
		float3	halo_pos;

		i = -1;
		while (++i < nspots)
		{
			spot_dist = distance(ori, spot_pos[i]);
			if (spot_dist < hit)
			{
				halo_pos = ori + dir * spot_dist;
				shine = spot_lum[i] / pow(distance(halo_pos, spot_pos[i]), 5);
				color += (float4)shine;
			}
		}

		// If the closest object hit is closer then the far plane, calculate the luminosity add it tu the color value
		v = ori + dir * hit;
		if (hit < far)
		{
			color += get_point_color(obj_type, obj_color, obj_transform, obj_rot_mat, obj_rev_rot, spot_pos, spot_lum,
					nspots, nobjs, obj_hit, v, dir) * (reflect_amount  * (1 - obj_reflectivness[obj_hit]));
			reflect_amount *= obj_reflectivness[obj_hit];

			// Calculates the reflected ray
			if (reflected < max_rays && reflect_amount > 0.05)
			{
				float3		normal;

				normal = get_normal(obj_type[obj_hit], obj_transform[obj_hit], obj_rev_rot[obj_hit], v, dir);
				dir = normalize(dir - (normal * 2 * dot(dir, normal)));
				ori = v;
			}
		}
		else
			break ;
	}

	color = color / (color + 1); 
	buf[buf_pos] = (uchar4)(255 * color.r, 255 * color.g, 255 * color.b, 255 * color.a);
}

__kernel void	sampler(
				  __global uchar4	*dest_buf,
				  __global uchar4	*origin_buf,
				  int2				dest_size,
				  int2				origin_size
				 )
{
	int2			id;
	int				buf_pos;
	uchar4			color;
	uchar4			tmp_colors[2];
	float4			tmp_color_mix;
	float2			ratio;
	float			total_ratio;
	float2			sampler_coord;
	float2			sampler_fract;
	int2			origin_pos;
	int				origin_buf_pos;
	int				i;
	int				j;

	if ((id.x = get_global_id(0)) >= dest_size.x
			|| (id.y = get_global_id(1)) >= dest_size.y)
		return ;
	buf_pos = id.x + dest_size.x * id.y;
	ratio = (float2)((float)origin_size.x / dest_size.x, (float)origin_size.y / dest_size.y);
	sampler_coord = (float2)(id.x * ratio.x, id.y * ratio.y);
	if (ratio.x < 0.99)
	{
		sampler_fract = fmod(sampler_coord, 1);
		origin_pos = (int2)floor(sampler_coord);
		origin_buf_pos = (int)sampler_coord.x + origin_size.x * (int)sampler_coord.y;
		tmp_colors[0] = mix(origin_buf[origin_buf_pos], origin_buf[origin_buf_pos + 1], sampler_fract.x);
		tmp_colors[1] = mix(origin_buf[origin_buf_pos + origin_size.x], origin_buf[origin_buf_pos + origin_size.x + 1], sampler_fract.x);
		color = mix(tmp_colors[0], tmp_colors[1], sampler_fract.y);
	}
	else if (ratio.x > 1.01)
	{
		origin_pos = (int2)floor(sampler_coord);
		origin_buf_pos = (int)sampler_coord.x + origin_size.x * (int)sampler_coord.y;
		total_ratio = round(ratio.x) * round(ratio.y);
		tmp_color_mix = (float4)0;
		i = 0;
		j = 0;
		while (j < round(ratio.y))
		{
			tmp_colors[0] = origin_buf[origin_buf_pos + i + j * origin_size.x];
			tmp_color_mix += (float4)(tmp_colors[0].r / total_ratio, tmp_colors[0].g / total_ratio, tmp_colors[0].b / total_ratio, tmp_colors[0].a / total_ratio);
			i = (i + 1) % (int)round(ratio.x);
			if (i == 0)
				j++;
		}
		color = (uchar4)(tmp_color_mix.r, tmp_color_mix.g, tmp_color_mix.b, tmp_color_mix.a);
	}
	else
		color = origin_buf[buf_pos];
	dest_buf[buf_pos] = (uchar4)(color.rgb, 0x4F);
}

__kernel void	clear_buf(__global uchar4 *buf,
							uchar4 val)
{
	int		id;

	id = get_global_id(0);
	buf[id] = (uchar4)(val.r, val.g, val.b, 0x00);
}
