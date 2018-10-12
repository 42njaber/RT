
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
						   float			fov,
						   int				iter,
						   int2				iter_pos
						  )
{
	int2			id;
	int				buf_pos;
	float			s;

	id.x = get_global_id(0);
	id.y = get_global_id(1);
	id = id * 16 + iter_pos;
	if (id.x >= img_size.x || id.y >= img_size.y)
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
				  int2				size,
				  int				iter
				 )
{
	int2			id;
	int2			pos;
	int				buf_pos;
	int				bits;
	uchar4			color;
	int				extra_px[16];
	int				line = size.x * 4;

	if ((id.x = get_global_id(0)) >= size.x
			|| (id.y = get_global_id(1)) >= size.y)
		return ;
	buf_pos = id.x + size.x * id.y;
	bits = (id.x & 2 ? 1 : 0) | (id.y & 2 ? 2 : 0) | (id.x & 1 ? 4 : 0) | (id.y & 1 ? 8 : 0);
	if (iter >= 15)
	{
		int		pxnb;
		int		i;
		int4	tmp;

		pos = id * 4;
		pxnb = (iter - bits) / 16 + 1;
		tmp = (int4)(0, 0, 0, 0);
		extra_px[0] = 0;
		extra_px[1] = 2;
		extra_px[2] = 2 * line;
		extra_px[3] = 2 + 2 * line;
		extra_px[4] = 1;
		extra_px[5] = 3;
		extra_px[6] = 1 + 2 * line;
		extra_px[7] = 3 + 2 * line;
		extra_px[8] = line;
		extra_px[9] = 2 + line;
		extra_px[10] = 3 * line;
		extra_px[11] = 2 + 3 * line;
		extra_px[12] = 1 + line;
		extra_px[13] = 3 + line;
		extra_px[14] = 1 + 3 * line;
		extra_px[15] = 3 + 3 * line;
		i = -1;
		while (++i < pxnb)
			tmp += convert_int4(origin_buf[pos.x + line * pos.y + extra_px[i]]);
		color = convert_uchar4(tmp / pxnb);
	}
	else
	{
		int2		cpos = (int2)(0, 0);

		if (iter >= 1 && bits & 1)
			cpos.x |= 2;
		if (iter >= 2 + (bits & (1 << 1) - 1) && bits & 2)
			cpos.y |= 2;
		if (iter >= 4 + (bits & (1 << 2) - 1) && bits & 4)
			cpos.x |= 1;
		if (iter >= 8 + (bits & (1 << 3) - 1) && bits & 8)
			cpos.y |= 1;
		pos = ((id / 4) * 4 + cpos) * 4;
		color = origin_buf[pos.x + line * pos.y];
	}
	dest_buf[buf_pos] = (uchar4)(color.rgb, 0x4F);
}

__kernel void	clear_buf(__global uchar4 *buf,
							uchar4 val)
{
	int		id;

	id = get_global_id(0);
	buf[id] = (uchar4)(val.r, val.g, val.b, 0x00);
}
