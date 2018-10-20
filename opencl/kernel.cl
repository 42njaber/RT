
#include "math.clh"
#include "hit_equations.clh"

int						can_trace(__global t_obj *obj, int nobjs, float3 v1, float3 v2);
float4					get_point_color(__global t_obj *objs, __global t_spot *spots, int nspots, int nobjs, int obj_hit,
															float ambiant_light, float3 v, float3 dir);

int						can_trace(
						   __global t_obj	*obj,
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
		ori_tmp = vec_mat_mult(obj[i].transform, v1);
		dir_tmp = vec_mat_mult(obj[i].rot_mat, line);
		len = length(dir_tmp);
		dir_tmp = normalize(dir_tmp);
		if (((obj[i].type == SPHERE && sphere_hit(ori_tmp, dir_tmp, &tmp)) ||
			(obj[i].type == PLANE && plane_hit(ori_tmp, dir_tmp, &tmp)) ||
			(obj[i].type == CYLINDER && cylinder_hit(ori_tmp, dir_tmp, &tmp)) ||
			(obj[i].type == CONE && cone_hit(ori_tmp, dir_tmp, &tmp)) ||
			(obj[i].type == TORUS && torus_hit(ori_tmp, dir_tmp, &tmp)) ||
			(obj[i].type == MOEBIUS && moebius_hit(ori_tmp, dir_tmp, &tmp))) &&
				tmp + 0.0001 < len)
			return (0);
	}
	return (1);
}

float4					get_point_color(
								   __global t_obj *objs,
								   __global t_spot *spots,
								   int nspots,
								   int nobjs,
								   int obj_hit,
								   float ambiant_light,
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

	lum = ambiant_light;
	brilliance = 0;
	i = -1;
	normal = get_normal(objs + obj_hit, v, dir);
	while (++i < nspots)
		if (can_trace(objs, nobjs, v, spots[i].pos))
		{
			r_in = v - spots[i].pos;
			a_in = dot(normalize(r_in), normal);
			//r_out = normalize(normalize(r_in) - (normal * 2 * a_in));
			//brilliance = pow(dot(normalize(r_in), normal) , (float)400);
			if (a_in > 0)
				lum += spots[i].lum * a_in / pow(length(r_in), (float)2) + brilliance * spots[i].lum;
		}
	color = (float4)(lum * ((float)1 + objs[obj_hit].color.r) / 255, lum * ((float)1 + objs[obj_hit].color.g) / 255,
					lum * ((float)1 + objs[obj_hit].color.b) / 255, lum * ((float)1 + objs[obj_hit].color.a) / 255);
	//if (fabs(fmod(v.x + 0.2, (float)20)) < 0.03 || fabs(fmod(v.y + 0.2, (float)20)) < 0.03 || fabs(fmod(v.z + 0.2, (float)20)) < 0.03)
		//color = 1 / color;
	return (color);
}

#include "debug.clh"

__kernel void			process_image(
						   __write_only image2d_t		image,
						   __global t_obj				*objs,
						   __global t_spot				*spots,
						   int							nobjs,
						   int							nspots,
						   t_set						set
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
	float3			ori;
	float3			dir;

	ori = (float3)((float)(id.x - img_size.x / 2) / (max(img_size.x, img_size.y) / 2) / s,
				   (float)(img_size.y / 2 - id.y) / (max(img_size.x, img_size.y) / 2) / s, 1);
	dir = ori;
	ori = vec_mat_mult(set.cam_mat, ori);
	dir = normalize(vec_mat_mult(set.cam_mat_rot, dir));

	int				reflected;
	float			reflect_amount;
	float4			color;

	color = (float4)0;
	reflected = 0;
	reflect_amount = 1;
	while (reflected++ < set.max_rays && reflect_amount > 0.05)
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
		hit = -1;
		obj_hit = -1;
		while (++i < nobjs)
		{
			ori_tmp = vec_mat_mult(objs[i].transform, ori);
			dir_tmp = vec_mat_mult(objs[i].rot_mat, dir);
			if (((objs[i].type == SPHERE && sphere_hit(ori_tmp, dir_tmp, &tmp)) ||
				(objs[i].type == PLANE && plane_hit(ori_tmp, dir_tmp, &tmp)) ||
				(objs[i].type == CYLINDER && cylinder_hit(ori_tmp, dir_tmp, &tmp)) ||
				(objs[i].type == CONE && cone_hit(ori_tmp, dir_tmp, &tmp)) ||
				(objs[i].type == TORUS && torus_hit(ori_tmp, dir_tmp, &tmp)) ||
				(objs[i].type == MOEBIUS && moebius_hit(ori_tmp, dir_tmp, &tmp))) &&
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
				shine = spots[i].lum / powr(spot_dist, 2) *
					(length(ray_diff) < 0.001 ? 1 : powr((1 - sqrt(powr(length(ray_diff), 2) - powr(dot(ray_diff, spot_ray), 2))), 12));
				color += (float4)shine * reflect_amount;
			}
		}

		// If the closest object hit is closer then the far plane, calculate the luminosity add it too the color value
		if (hit > 0)
		{
			v = ori + dir * hit;
			color += get_point_color(objs, spots, nspots, nobjs, obj_hit, set.ambiant_light, v, dir) * reflect_amount * (1 - objs[obj_hit].reflect);
			reflect_amount *= objs[obj_hit].reflect;

			// Calculates the reflected ray
			if (reflected < set.max_rays && reflect_amount > 0.05)
			{
				float3		normal;

				normal = get_normal(objs + obj_hit, v, dir);
				dir = normalize(dir - (normal * 2 * dot(dir, normal)));
				ori = v;
			}
		}
		else
		{
			color += ((1 / (1 - (convert_float4(set.sky_color) / 255))) - 1) * reflect_amount;
			break ;
		}
	}

	color = color / (color + 1); 
	write_imageui(image, id, (uint4)(255 * color.r, 255 * color.g, 255 * color.b, 0));
}

__const sampler_t near_sampler = CLK_NORMALIZED_COORDS_FALSE |
								 CLK_ADDRESS_REPEAT |
								 CLK_FILTER_NEAREST;

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

__const sampler_t normal_sampler = CLK_NORMALIZED_COORDS_TRUE |
								   CLK_ADDRESS_MIRRORED_REPEAT |
								   CLK_FILTER_NEAREST;

static float3				test_solver(float3 parm)
{
	float3	root;
	float	p;
	float	q;
	float	delta;
	float	sqrdelta;
	float	root3;
	float	tmp1;
	float	tmp2;
	float	sub;

	root = NAN;
	p = parm.y - parm.x * parm.x / 3;
	q = 2 * parm.x * parm.x * parm.x / 27 - parm.y * parm.x / 3 + parm.z;
	sub = -parm.x / 3;
	delta = q * q / 4 + p * p * p / 27;
	sqrdelta = sqrt(delta);
	if (delta > 0)
		root.s0 = cbrt(-q / 2 + sqrdelta) + cbrt(-q / 2 - sqrdelta) + sub;
	else
	{
		root3 = sqrt((float)3);
		tmp2 = sqrt(-p);
		tmp1 = (2 / root3) * tmp2;
		tmp2 = asin((3 * root3 * q) / (2 * tmp2 * tmp2 * tmp2)) / 3;
		root.s0 = tmp1 * sin(tmp2) + sub;
		root.s1 = -tmp1 * sin(tmp2 + M_PI_F / 3) + sub;
		root.s2 = tmp1 * cos(tmp2 + M_PI_F / 6) + sub;
	}
	return (order(root));
}

static bool				moebius_in_band(float3 hit)
{
	float2	band_pos;
	float	tmp;

	band_pos = (float2)(length((float2)(hit.x, hit.y)) - 2, hit.z);
	tmp = angle(hit.xy) / 2;
	if (length(band_pos) < 1 && fabs(dot(band_pos, (float2)(cos(tmp), sin(tmp)))) < 0.01)
		return (1);
	return (0);
}

static bool					test_moebius(__global uchar4 *buf, int2 size, int2 pos, int2 id, t_set set)
{
	float	a;
	float	b;
	float	c;
	float	d;
	float3	inter;
	float3			ori;
	float3			dir;
	float			s;

	s = 1 / tan((float)(set.fov * 0.5 * M_PI_F / 180));
	ori = (float3)((float)(pos.x - size.x / 2) / (max(size.x, size.y) / 2) / s,
				   (float)(size.y / 2 - pos.y) / (max(size.x, size.y) / 2) / s, 1);
	dir = ori;
	ori = vec_mat_mult(set.cam_mat, ori);
	dir = normalize(vec_mat_mult(set.cam_mat_rot, dir));
	ori = ori - (float3)(0, -5, 40);
	ori.yz = ori.zy * (float2)(1, -1);
	dir.yz = dir.zy * (float2)(1, -1);
	ori /= 5;
	dir /= 5;

	a = dir.x * dir.x * dir.y + dir.y * dir.y * dir.y - 2 * dir.x * dir.x * dir.z - 2 * dir.y * dir.y * dir.z + dir.y * dir.z * dir.z;
	b = -4 * dir.x * dir.z + 2 * dir.x * dir.y * ori.x - 4 * dir.x * dir.z * ori.x + dir.x * dir.x * ori.y + 3 * dir.y * dir.y * ori.y
			- 4 * dir.y * dir.z * ori.y + dir.z * dir.z * ori.y - 2 * dir.x * dir.x * ori.z - 2 * dir.y * dir.y * ori.z + 2 * dir.y * dir.z * ori.z;
	c = -4 * dir.y - 4 * dir.z * ori.x + dir.y * ori.x * ori.x - 2 * dir.z * ori.x * ori.x + 2 * dir.x * ori.x * ori.y + 3 * dir.y * ori.y * ori.y
			- 2 * dir.z * ori.y * ori.y - 4 * dir.x * ori.z - 4 * dir.x * ori.x * ori.z - 4 * dir.y * ori.y * ori.z + 2 * dir.z * ori.y * ori.z + dir.y * ori.z * ori.z;
	d = -4 * ori.y + ori.x * ori.x * ori.y + ori.y * ori.y * ori.y - 4 * ori.x * ori.z - 2 * ori.x * ori.x * ori.z - 2 * ori.y * ori.y * ori.z + ori.y * ori.z * ori.z;
	inter = NAN;
	inter = test_solver((float3)(b, c, d) / a);

	float16		test;
	test = NAN;

	if (inter.x >= 0.01 && moebius_in_band(ori + dir * inter.x))
		test.s0 = inter.x;
	else if (inter.y >= 0.01 && moebius_in_band(ori + dir * inter.y))
		test.s0 = inter.y;
	else if (moebius_in_band(ori + dir * inter.z))
		test.s0 = inter.z;

	test.s012 = inter;
	if (logger(test, 4))
		buf[id.x + size.x * id.y] = (uchar4)(0x00, 0xff, 0xff, 0x00);
	return (0);
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
	//test_moebius(buf, size, gui.mouse_pos, id, set);
}

__kernel void	clear_buf(__write_only image2d_t buf,
							uchar4 val)
{
	int2		id;

	id.x = get_global_id(0);
	id.y = get_global_id(1);
	write_imageui(buf, id, (uint4)(val.r, val.g, val.b, 0x00));
}
