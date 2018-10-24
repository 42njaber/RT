
#include "math.clh"
#include "hit_equations.clh"

float4					spot_projected_color(__global t_obj *obj, int nobjs, float3 v1, __global t_spot *spot);
float4					get_point_color(__global t_obj *objs, __global t_spot *spots, int nspots, int nobjs, int obj_hit,
															float ambiant_light, float3 v, float3 dir, image2d_t texture);

__const sampler_t near_sampler = CLK_NORMALIZED_COORDS_FALSE |
								 CLK_ADDRESS_REPEAT |
								 CLK_FILTER_NEAREST;

__const sampler_t normal_sampler = CLK_NORMALIZED_COORDS_TRUE |
								   CLK_ADDRESS_REPEAT |
								   CLK_FILTER_NEAREST;

float4					spot_projected_color(
						   __global t_obj	*obj,
						   int				nobjs,
						   float3			v1,
						   __global t_spot	*spot
						)
{
	float3			line;
	float3			ori_tmp;
	float3			dir_tmp;
	float4			ret;
	float			len;
	float			tmp;
	int				i;

	line = spot->pos - v1;
	ret = convert_float4(spot->color) / 255.f;
	i = -1;
	// Iterate throught all the objects to detect the first one hit by the ray
	while (++i < nobjs)
	{
		ori_tmp = vec_mat_mult(obj[i].transform, v1);
		dir_tmp = vec_mat_mult(obj[i].rot_mat, line);
		len = length(dir_tmp);
		dir_tmp = normalize(dir_tmp);
		if (((obj[i].type == SPHERE && sphere_hit(ori_tmp, dir_tmp, &tmp, 0)) ||
			(obj[i].type == PLANE && plane_hit(ori_tmp, dir_tmp, &tmp, 0)) ||
			(obj[i].type == CYLINDER && cylinder_hit(ori_tmp, dir_tmp, &tmp, 0)) ||
			(obj[i].type == CONE && cone_hit(ori_tmp, dir_tmp, &tmp, 0)) ||
			(obj[i].type == TORUS && torus_hit(ori_tmp, dir_tmp, &tmp, 0)) ||
			(obj[i].type == MOEBIUS && moebius_hit(ori_tmp, dir_tmp, &tmp, 0))) &&
				tmp < len)
		{
			if (obj[i].transparency < 0.05)
				return ((float4)0);
			else
			{
				ori_tmp = vec_mat_mult(obj[i].transform, v1 + tmp * line / len);
				dir_tmp = normalize(dir_tmp);
				if ((obj[i].type == SPHERE && sphere_hit(ori_tmp, dir_tmp, &tmp, 1)) ||
					(obj[i].type == PLANE && plane_hit(ori_tmp, dir_tmp, &tmp, 1)) ||
					(obj[i].type == CYLINDER && cylinder_hit(ori_tmp, dir_tmp, &tmp, 1)) ||
					(obj[i].type == CONE && cone_hit(ori_tmp, dir_tmp, &tmp, 1)) ||
					(obj[i].type == TORUS && torus_hit(ori_tmp, dir_tmp, &tmp, 1)) ||
					(obj[i].type == MOEBIUS && moebius_hit(ori_tmp, dir_tmp, &tmp, 1)))
					ret *= pow(obj[i].transparency, tmp / 3) * convert_float4(obj[i].color) / 255.0f;
				else
					ret *= obj[i].transparency * convert_float4(obj[i].color) / 255.0f;
			}
		}
	}
	return (ret);
}

static float rand_noise(int t)
{
	t = (t << 13) ^ t;
	t = (t * (t * t * 15731 + 789221) + 1376312589);
	return (1.0 - (t & 0x7fffffff) / 1073741824.0);
}

static float	noise_2d(float2 position)
{
	int tmp;

	tmp = rand_noise(position.x * 1000) * 850000;
	return rand_noise(tmp + (position.y * 1000));
}

//calculer les 3 couleur

float4					get_point_color(
								   __global t_obj *objs,
								   __global t_spot *spots,
								   int nspots,
								   int nobjs,
								   int obj_hit,
								   float ambiant_light,
								   float3 v,
								   float3 dir,
								   image2d_t texture
								  )
{
	float3			normal;
	float3			r_in;
	//float3			r_out;
	float			a_in;
	float			brilliance;
	float4			lum;
	float4			obj_color;
	float4			tmp;
	float2			tex_pos;
	float4			color;
	int				i;

	lum = (float4)ambiant_light;
	brilliance = 0;
	i = -1;
	normal = get_normal(objs + obj_hit, v, dir);
	while (++i < nspots)
	{
		tmp = spot_projected_color(objs, nobjs, v, spots + i);
		if (tmp.r + tmp.g + tmp.b > 0.05)
		{
			r_in = v - spots[i].pos;
			a_in = dot(normalize(r_in), normal);
			//r_out = normalize(normalize(r_in) - (normal * 2 * a_in));
			//brilliance = pow(dot(normalize(r_in), normal) , (float)400);
			if (a_in > 0)
				lum += tmp * (float4)(spots[i].lum * a_in / pow(length(r_in), (float)2) + brilliance * spots[i].lum);
		}
	}
	tex_pos = get_surface_pos(objs + obj_hit, v, dir);
	obj_color = convert_float4(objs[obj_hit].color) / 255.0f;
	//obj_color *= read_imagef(texture, normal_sampler, tex_pos);
	obj_color *= (float4)noise_2d(tex_pos);
	color = lum * obj_color;
	//if (fabs(fmod(v.x + 0.2, (float)20)) < 0.03 || fabs(fmod(v.y + 0.2, (float)20)) < 0.03 || fabs(fmod(v.z + 0.2, (float)20)) < 0.03)
		//color = 1 / color;
	return (color);
}

typedef struct		s_ray {
	float3		ori;
	float3		dir;
	float4		absorption;
	float		ref_index;
}					t_ray;

#define RAY_NUMBER_MAX 20
#define MIN_RAY_ABSORPTION 0.001f

__kernel void			process_image(
						   __write_only image2d_t		image,
						   __global t_obj				*objs,
						   __global t_spot				*spots,
						   int							nobjs,
						   int							nspots,
						   t_set						set,
						   __read_only image2d_t		texture
						  )
{
	int2			id;
	float			s;
	int2			img_size;


	id.x = get_global_id(0);
	id.y = get_global_id(1);
	if (set.progressive)
		id = id * 16 + set.iter_pos;
	img_size = (int2)(get_image_width(image), get_image_height(image));
	if (id.x >= img_size.x || id.y >= img_size.y)
		return ;
	s = 1 / tan((float)(set.fov * 0.5 * M_PI_F / 180));

	// Creation of origin and direction vectors of the ray
	t_ray			rays[RAY_NUMBER_MAX];
	int				nrays;
	int				ray;
	int				max_rays;

	rays[0].ori = (float3)((float)(id.x - img_size.x / 2) / (max(img_size.x, img_size.y) / 2) / s,
				   (float)(img_size.y / 2 - id.y) / (max(img_size.x, img_size.y) / 2) / s, 1);
	rays[0].dir = rays[0].ori;
	rays[0].ori = vec_mat_mult(set.cam_mat, rays[0].ori);
	rays[0].dir = normalize(vec_mat_mult(set.cam_mat_rot, rays[0].dir));
	rays[0].absorption = (float4)(1, 1, 1, 1);
	rays[0].ref_index = 1;
	max_rays = min(RAY_NUMBER_MAX, set.max_rays);
	nrays = 1;
	ray = 0;

	float4			color;
	color = (float4)0;

	while (ray < nrays)
	{
		float3			ori;
		float3			dir;
		float3			ori_tmp;
		float3			dir_tmp;
		float4			reflect_amount;
		float			hit;
		float			tmp;
		int				i;
		int				obj_hit;
		float3			v;

		ori = rays[ray].ori;
		dir = rays[ray].dir;
		reflect_amount = rays[ray].absorption;
		ray++;
		if (reflect_amount.x + reflect_amount.y + reflect_amount.z < MIN_RAY_ABSORPTION)
			continue ;

		i = -1;
		hit = -1;
		obj_hit = -1;
		while (++i < nobjs)
		{
			ori_tmp = vec_mat_mult(objs[i].transform, ori);
			dir_tmp = vec_mat_mult(objs[i].rot_mat, dir);
			if (((objs[i].type == SPHERE && sphere_hit(ori_tmp, dir_tmp, &tmp, 0)) ||
				(objs[i].type == PLANE && plane_hit(ori_tmp, dir_tmp, &tmp, 0)) ||
				(objs[i].type == CYLINDER && cylinder_hit(ori_tmp, dir_tmp, &tmp, 0)) ||
				(objs[i].type == CONE && cone_hit(ori_tmp, dir_tmp, &tmp, 0)) ||
				(objs[i].type == TORUS && torus_hit(ori_tmp, dir_tmp, &tmp, 0)) ||
				(objs[i].type == MOEBIUS && moebius_hit(ori_tmp, dir_tmp, &tmp, 0))) &&
					((tmp > 0 && hit < 0) || tmp < hit))
			{
				hit = tmp;
				obj_hit = i;
			}
		}

		// Creates à halo of light when ray passes close to a spots
		float	shine;
		float	spot_dist;
		float3	ray_diff;
		float3	spot_ray;

		i = -1;
		while (++i < nspots)
		{
			spot_ray = spots[i].pos - ori;
			spot_dist = length(spot_ray);
			spot_ray = normalize(spot_ray);
			if (dot(dir, spot_ray) > 0.5 && (hit < 0 || spot_dist < hit))
			{
				ray_diff = spot_ray - dir;
				shine = spots[i].lum / pow(spot_dist, 2) *
					(length(ray_diff) < 0.001 ? 1 : powr((1 - sqrt(powr(length(ray_diff), 2) - powr(dot(ray_diff, spot_ray), 2))), 12));
				color += (float4)shine * convert_float4(spots[i].color) / 255.f * reflect_amount;
			}
		}

		// If the closest object hit is closer then the far plane, calculate the luminosity add it too the color value
		if (hit > 0)
		{
			float3	normal;
			float4	intensity_mult;
			float4	hit_color;
			float4	absorption;

			v = ori + dir * hit;
			normal = get_normal(objs + obj_hit, v, dir);
			hit_color = convert_float4(objs[obj_hit].color) / 255.0f;

			// Calculates the refracted/reflected rays
			if (nrays < max_rays && (reflect_amount.x + reflect_amount.y + reflect_amount.z) * objs[obj_hit].transparency > MIN_RAY_ABSORPTION)
			{
				float	cost1;
				float	cost2;
				float	ratio;
				float3	refract_ori;
				float3	refract_dir;
				float	n1;
				float	power_coefficient;

				n1 = objs[obj_hit].ref_index;
				cost1 = dot(dir, normal);
				ratio = 1. / n1;
				cost2 = 1 - ratio * ratio * (1 - cost1 * cost1);
				power_coefficient = pow((n1 - 1) / (n1 + 1), 2);
				power_coefficient = power_coefficient + (1 - power_coefficient) * powr(1 - cost2, 5);
				absorption = hit_color * objs[obj_hit].transparency;
				if (cost2 > 0)
				{
					cost2 = sqrt(cost2);
					refract_dir = normalize(ratio * dir - (ratio * cost1 - cost2) * normal);
					refract_ori = v + refract_dir * 0.01f;
					ori_tmp = vec_mat_mult(objs[obj_hit].transform, refract_ori);
					dir_tmp = vec_mat_mult(objs[obj_hit].rot_mat, refract_dir);
					if ((objs[obj_hit].type == SPHERE && sphere_hit(ori_tmp, dir_tmp, &tmp, 1)) ||
						(objs[obj_hit].type == PLANE && plane_hit(ori_tmp, dir_tmp, &tmp, 1)) ||
						(objs[obj_hit].type == CYLINDER && cylinder_hit(ori_tmp, dir_tmp, &tmp, 1)) ||
						(objs[obj_hit].type == CONE && cone_hit(ori_tmp, dir_tmp, &tmp, 1)) ||
						(objs[obj_hit].type == TORUS && torus_hit(ori_tmp, dir_tmp, &tmp, 1)) ||
						(objs[obj_hit].type == MOEBIUS && moebius_hit(ori_tmp, dir_tmp, &tmp, 1)))
					{
						float3	refract_normal;

						absorption = pow(absorption, tmp / 20);
						refract_ori = refract_ori + refract_dir * (tmp + 0.01f);
						refract_normal = get_normal(objs + obj_hit, refract_ori, refract_dir);
						cost1 = dot(refract_dir, refract_normal);
						ratio = n1 / 1.;
						cost2 = 1 - ratio * ratio * (1 - cost1 * cost1);
						if (cost2 > 0)
						{
							cost2 = sqrt(cost2);
							dir = normalize(ratio * refract_dir - (ratio * cost1 - cost2) * refract_normal);
						}
					}
					rays[nrays].ori = refract_ori;
					rays[nrays].dir = refract_dir;
					rays[nrays].absorption = reflect_amount * absorption * (1 - power_coefficient);
					nrays++;
				}
			}
			else
			{
				intensity_mult = reflect_amount;
				intensity_mult *= 1 - objs[obj_hit].reflect;
 				if ((reflect_amount.x + reflect_amount.y + reflect_amount.z) * objs[obj_hit].transparency > MIN_RAY_ABSORPTION)
					intensity_mult *= 1 - objs[obj_hit].transparency;
				color += get_point_color(objs, spots, nspots, nobjs, obj_hit, set.ambiant_light, v, dir, texture) * intensity_mult;
			}
			if (nrays < max_rays && (reflect_amount.x + reflect_amount.y + reflect_amount.z) * objs[obj_hit].reflect > MIN_RAY_ABSORPTION)
			{
				float4	reflect_mult;

				if (objs[obj_hit].transparency > MIN_RAY_ABSORPTION)
				{
					float	power_coefficient;
					float	cost1;
					float	n2;

					n2 = objs[obj_hit].ref_index;
					cost1 = dot(dir, normal);
					power_coefficient = pow((1 - n2) / (n2 + 1), 2);
					reflect_mult = (float4)(power_coefficient + (1 - power_coefficient) * pow(1 - cost1, 5));
				}
				else
					reflect_mult = hit_color;
				dir = normalize(dir - (normal * 2 * dot(dir, normal)));
				ori = v;
				rays[nrays].ori = ori + dir * 0.01f;
				rays[nrays].dir = dir;
				rays[nrays].absorption = reflect_amount * objs[obj_hit].reflect * reflect_mult;
				nrays++;
			}
		}
		else
			color += ((1 / (1 - (convert_float4(set.sky_color) / 255))) - 1) * reflect_amount;
	}

	color = clamp(0, 1, color);
	write_imageui(image, id, (uint4)(255.0f * color.r, 255.0f * color.g, 255.0f * color.b, 0));
	//if (logger(get_image_width(texture), 1))
		//write_imageui(image, id, (uint4)(0, 255.0f, 255.0f, 0));
}

__kernel void	sampler256(
				  __write_only image2d_t	dest_image,
				  __read_only image2d_t		src_image,
				  int2						size,
				  int						iter
				 )
{
	int2			id;
	int2			pos;
	int				bits;
	uint4			color;
	int2			extra_px[16];

	if (size.x * 4 > get_image_width(src_image) ||
			size.y * 4 > get_image_height(src_image))
		return ;
	if ((id.x = get_global_id(0)) >= size.x
			|| (id.y = get_global_id(1)) >= size.y)
		return ;
	bits = (id.x & 2 ? 1 : 0) | (id.y & 2 ? 2 : 0) | (id.x & 1 ? 4 : 0) | (id.y & 1 ? 8 : 0);
	if (iter >= 15)
	{
		int		pxnb;
		int		i;
		uint4	tmp;

		pos = id * 4;
		pxnb = (iter - bits) / 16 + 1;
		tmp = (uint4)(0, 0, 0, 0);
		extra_px[0]  = (int2)(0, 0);
		extra_px[1]  = (int2)(2, 0);
		extra_px[2]  = (int2)(0, 2);
		extra_px[3]  = (int2)(2, 2);
		extra_px[4]  = (int2)(1, 0);
		extra_px[5]  = (int2)(3, 0);
		extra_px[6]  = (int2)(1, 2);
		extra_px[7]  = (int2)(3, 2);
		extra_px[8]  = (int2)(0, 1);
		extra_px[9]  = (int2)(2, 1);
		extra_px[10] = (int2)(0, 3);
		extra_px[11] = (int2)(2, 3);
		extra_px[12] = (int2)(1, 1);
		extra_px[13] = (int2)(3, 1);
		extra_px[14] = (int2)(1, 3);
		extra_px[15] = (int2)(3, 3);
		i = -1;
		while (++i < pxnb)
			tmp += read_imageui(src_image, pos + extra_px[i]);
		color = tmp / pxnb;
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
		color = read_imageui(src_image, pos);
	}
	write_imageui(dest_image, id, (uint4)(color.rgb, 0x00));
}

__kernel void	sampler1(
				  __write_only image2d_array_t	dest_image,
				  __read_only image2d_t			src_image,
				  int2							size,
				  int							thumbnail_id
				 )
{
	int2			id;
	uint4			color;

	if ((id.x = get_global_id(0)) >= size.x
			|| (id.y = get_global_id(1)) >= size.y)
		return ;
	color = read_imageui(src_image, id);
	write_imageui(dest_image, (int4)(id, thumbnail_id, 0), (uint4)(color.rgb, 0x00));
}


__kernel void	paint_gui(
						__write_only image2d_t			screen,
						 t_gui							gui,
						 int2							size,
						 __read_only image2d_t			scene,
						 __read_only image2d_array_t	thumbnails,
						 t_set							set
						)
{
	int2			id;
	float2			npos;
	uint4			color;
	int				buf_pos;

	if ((id.x = get_global_id(0)) >= size.x 
			|| (id.y = get_global_id(1)) >= size.y)
		return ;
	npos = (float2)id / (float2)size * 100;
	buf_pos = id.x + size.x * id.y;
	color = (uint4)(0x22, 0x77, 0xbb, 0x00);
	if (gui.state == SCENE)
		color = read_imageui(scene, near_sampler, id);
	else
	{
		float2	center_dist;
		float2	container_pos;
		float2	mouse_dist;
		float2	tbn_center;

		center_dist = (float2)id / (float2)size - (float2)0.5  - ((float2)(-0.3 + 0.2 * (gui.scene_id % 4), -0.3 + 0.2 * (gui.scene_id / 4)) * ((float)1 - gui.zoom));
		if ((gui.state == ZOOM_SCENE || gui.state == UNZOOM_SCENE) &&
				fabs(center_dist.x) < 0.075 + (0.425 * gui.zoom) &&
				fabs(center_dist.y) < 0.075 + (0.425 * gui.zoom))
		{
			container_pos = center_dist / (float)(0.075 + (0.425 * gui.zoom)) / 2 + (float2)0.5;
			color = read_imageui(scene, normal_sampler, container_pos);
		}
		else
		{
			center_dist = (float2)((float2)id - (float2)(size / 2)) / (float2)size;
			mouse_dist = (float2)((float2)gui.mouse_pos - (float2)(size / 2)) / (float2)size;
			if (fabs(center_dist.x) < 0.4 && fabs(center_dist.y) < 0.4)
			{
				int		tbnnb = get_image_array_size(thumbnails);
				int		i;

				color = (uint4)(0x77, 0x77, 0x77, 0x00);
				i = -1;
				while (++i <  tbnnb)
				{
					tbn_center = (float2)(0.2 * (i % 4 + 1), 0.2 * (i / 4 + 1));
					center_dist = (float2)id / (float2)size - (float2)(tbn_center);
					mouse_dist = (float2)gui.mouse_pos / (float2)size - (float2)(tbn_center);
					if (fabs(center_dist.x) < 0.075 && fabs(center_dist.y) < 0.075)
					{
						container_pos = center_dist / 3 * 20 + (float2)0.5;
						color = read_imageui(thumbnails, normal_sampler, (float4)(container_pos, i, 0));
					}
					else if (fabs(center_dist.x) < 0.075 + 4 / (float)size.x && fabs(center_dist.y) < 0.075 + 4 / (float)size.y)
					{
						if (fabs(mouse_dist.x) < 0.075 && fabs(mouse_dist.y) < 0.075)
							color = (uint4)(0xaf, 0x00, 0x00, 0x00);
						else
							color = (uint4)(0x66, 0x66, 0x66, 0x00);
					}
				}
			}
			else if (fabs(center_dist.x) < 0.4 + 10 / (float)size.x && fabs(center_dist.y) < 0.4 + 10 / (float)size.y)
			{
				if (fabs(mouse_dist.x) < 0.4 && fabs(mouse_dist.y) < 0.4)
					color = (uint4)(0x22, 0x00, 0x00, 0x00);
				else
					color = (uint4)(0x22, 0x22, 0x22, 0x00);
			}
		}
	}
	write_imagef(screen, id, (float4)(convert_float3(clamp(0, 255, color.bgr)) / 255, 0.));
	//test_moebius(screen, size, gui.mouse_pos, id, set);
}

__kernel void	clear_buf(__write_only image2d_t buf,
							uchar4 val)
{
	int2		id;

	id.x = get_global_id(0);
	id.y = get_global_id(1);
	write_imageui(buf, id, (uint4)(val.r, val.g, val.b, 0x00));
}
