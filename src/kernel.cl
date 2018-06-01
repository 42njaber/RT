
float3					vec_mat_mult(float16 mat, float3 vec);
uchar4 __OVERLOAD__		mix(uchar4 c1, uchar4 c2, float gradiant);
void					put_px(int2 px, uchar4 color, __global uchar4 *buf, int2 img_size);
uchar4					choose_color(float gradiant, int iter, int iter_max, float shift, char palette);
float2					c_mult(float2 z1, float2 z2);
float2					c_pow(float2 z, int pow);
uchar4					sky(float gradiant, float shift);
uchar4					neon(float gradiant, float shift);
uchar4					black_white(float gradiant, float shift);
int						sphere_hit(float3 ori, float3 dir, float *t);
int						plane_hit(float3 ori, float3 dir, float *t);
int						cylinder_hit(float3 ori, float3 dir, float *t);
int						cone_hit(float3 ori, float3 dir, float *t);
float3					get_normal(int type, float16 transform, float16 rev_rot, float3 v);
int						can_trace(__global int *obj_type, __global float16 *obj_transform, __global float16	*obj_rot_mat, int nobjs, float3 v1, float3 v2);
void					write_nb(__global uchar4 *buf, int id, int2 img_size, int nb, int x, int y);

constant char nb_minus[16] =	".........XXX....";
constant char nb_plus[16] =		"......X..XXX..X.";
constant char nb_0[16] =		".XX.X..XX..X.XX.";
constant char nb_1[16] =		".X..XX...X..XXX.";
constant char nb_2[16] =		".XX.X..X..X.XXXX";
constant char nb_3[16] =		"XXX....X..XXXXX.";
constant char nb_4[16] =		"..X..XX.XXXX..X.";
constant char nb_5[16] =		"XXXXXX....X.XX..";
constant char nb_6[16] =		"XXX..X.XX.XXXXX.";
constant char nb_7[16] =		"XXXX..X...X..X..";
constant char nb_8[16] =		".XXX.X.XX.X.XXX.";
constant char nb_9[16] =		"XXX.X.XX.X.XXXX.";
constant char nb_X[16] =		"X..X.XX..XX.X..X";

void					write_nb(__global uchar4 *buf, int id, int2 img_size, int nb, int x, int y)
{
	int2	px;
	int		buf_pos;

	px = (int2)(id % img_size.x, id / img_size.x);
	if (px.x >= x && px.x < x + 8 && px.y >= y && px.y < y + 8)
	{
		buf_pos = (px.x - x) / 2 + 4 * ((px.y - y) / 2);
		if (nb == -3 && nb_minus[buf_pos] == 'X')
			buf[id] = (uchar4)(0x00, 0x00, 0x00, 0x00);
		else if (nb == -1 && nb_plus[buf_pos] == 'X')
			buf[id] = (uchar4)(0x00, 0x00, 0x00, 0x00);
		else if (nb == 0 && nb_0[buf_pos] == 'X')
			buf[id] = (uchar4)(0x00, 0x00, 0x00, 0x00);
		else if (nb == 1 && nb_1[buf_pos] == 'X')
			buf[id] = (uchar4)(0x00, 0x00, 0x00, 0x00);
		else if (nb == 2 && nb_2[buf_pos] == 'X')
			buf[id] = (uchar4)(0x00, 0x00, 0x00, 0x00);
		else if (nb == 3 && nb_3[buf_pos] == 'X')
			buf[id] = (uchar4)(0x00, 0x00, 0x00, 0x00);
		else if (nb == 4 && nb_4[buf_pos] == 'X')
			buf[id] = (uchar4)(0x00, 0x00, 0x00, 0x00);
		else if (nb == 5 && nb_5[buf_pos] == 'X')
			buf[id] = (uchar4)(0x00, 0x00, 0x00, 0x00);
		else if (nb == 6 && nb_6[buf_pos] == 'X')
			buf[id] = (uchar4)(0x00, 0x00, 0x00, 0x00);
		else if (nb == 7 && nb_7[buf_pos] == 'X')
			buf[id] = (uchar4)(0x00, 0x00, 0x00, 0x00);
		else if (nb == 8 && nb_8[buf_pos] == 'X')
			buf[id] = (uchar4)(0x00, 0x00, 0x00, 0x00);
		else if (nb == 9 && nb_9[buf_pos] == 'X')
			buf[id] = (uchar4)(0x00, 0x00, 0x00, 0x00);
		else if (!((nb >= -3 && nb <= 9)) && nb_X[buf_pos] == 'X')
			buf[id] = (uchar4)(0x00, 0x00, 0x00, 0x00);
	}
}

float3					vec_mat_mult(float16 mat, float3 vec)
{
	float3	ret;
	float	w;

	w = (mat.sc * vec.x) + (mat.sd * vec.y) + (mat.se * vec.z) + mat.sf;
	ret.x = ((mat.s0 * vec.x) + (mat.s1 * vec.y) + (mat.s2 * vec.z) + mat.s3) / w;
	ret.y = ((mat.s4 * vec.x) + (mat.s5 * vec.y) + (mat.s6 * vec.z) + mat.s7) / w;
	ret.z = ((mat.s8 * vec.x) + (mat.s9 * vec.y) + (mat.sa * vec.z) + mat.sb) / w;
	return (ret);
}

uchar4 __OVERLOAD__ mix(uchar4 c1, uchar4 c2, float gradiant)
{
	float4	tmp;
	float4	tmp1;
	float4	tmp2;

	tmp1 = (float4)(c1.x, c1.y, c1.z, c1.w);
	tmp2 = (float4)(c2.x, c2.y, c2.z, c2.w);
	tmp = mix(tmp1, tmp2, gradiant);
	return ((uchar4)(tmp.x, tmp.y, tmp.z, tmp.w));
}

int			sphere_hit(float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float	d;

	a = pow(dir.x, 2) + pow(dir.y, 2) + pow(dir.z, 2);
	b = 2 * (ori.x * dir.x + ori.y * dir.y + ori.z * dir.z);
	c = pow(ori.x, 2) + pow(ori.y, 2) + pow(ori.z, 2) - 1;
	d = pow(b, 2) - 4 * a * c;
	if (d < 0)
		return (0);
	c = (-b - sqrt(d)) / (2 * a);
	d = (-b + sqrt(d)) / (2 * a);
	if (c <= 0 && d <= 0)
		return (0);
	if (c < 0 || d < 0)
		*t = fmax(c, d);
	else
		*t = fmin(c, d);
	return (1);
}

int			plane_hit(float3 ori, float3 dir, float *t)
{
	float	a;

	if (ori.z == 0)
	{
		*t = 0;
		return (1);
	}
	if (dir.z == 0)
		return (0);
	a = -ori.z / dir.z;
	if (a < 0)
		return (0);
	*t = a;
	return (1);
}

int			cylinder_hit(float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float	d;

	a = pow(dir.x, 2) + pow(dir.z, 2);
	b = 2 * (ori.x * dir.x + ori.z * dir.z);
	c = pow(ori.x, 2) + pow(ori.z, 2) - 1;
	d = pow(b, 2) - 4 * a * c;
	if (d < 0)
		return (0);
	c = (-b - sqrt(d)) / (2 * a);
	d = (-b + sqrt(d)) / (2 * a);
	if (c <= 0 && d <= 0)
		return (0);
	if (c < 0 || d < 0)
		*t = fmax(c, d);
	else
		*t = fmin(c, d);
	return (1);
}

int			cone_hit(float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float	d;

	a = pow(dir.x, 2) + pow(dir.z, 2) - pow(dir.y * 1, 2);
	b = 2 * (ori.x * dir.x + ori.z * dir.z - ori.y * dir.y);
	c = pow(ori.x, 2) + pow(ori.z, 2) - pow(ori.y * 1, 2);
	d = pow(b, 2) - 4 * a * c;
	if (d < 0)
		return (0);
	c = (-b - sqrt(d)) / (2 * a);
	d = (-b + sqrt(d)) / (2 * a);
	if (c <= 0 && d <= 0)
		return (0);
	if (c < 0 || d < 0)
		*t = fmax(c, d);
	else
		*t = fmin(c, d);
	return (1);
}

float3					get_normal(int type, float16 transform, float16 rev_rot, float3 v)
{
	float3	ret;

	v = vec_mat_mult(transform, v);
	if (type == 0)
		ret = (float3){-v.x, -v.y, -v.z};
	else if (type == 1)
		ret = (float3){0, 0, copysign(1, -v.z)};
	else if (type == 2)
		ret = (float3){-v.x, 0, -v.z};
	else if (type == 3)
		ret = (float3){-v.x, v.y, -v.z};
	else
		ret = (float3){0, 0, 1};
	ret = vec_mat_mult(rev_rot, ret);
	return (normalize(ret));
}

int						can_trace(
						   __global int		*obj_type,
						   __global float16	*obj_transform,
						   __global float16	*obj_rot_mat,
						   int				nobjs,
						   float3			v1,
						   float3			v2
						)
{
	// Iterate throught all the objects to detect the first one hit by the ray
	float3			line;
	float3			ori_tmp;
	float3			dir_tmp;
	float			tmp;
	int				i;

	line = v2 - v1;
	i = -1;
	while (++i < nobjs)
	{
		ori_tmp = vec_mat_mult(obj_transform[i], v1);
		dir_tmp = vec_mat_mult(obj_rot_mat[i], line);
		if (((obj_type[i] == 0 && sphere_hit(ori_tmp, dir_tmp, &tmp) == 1) ||
			(obj_type[i] == 1 && plane_hit(ori_tmp, dir_tmp, &tmp) == 1) ||
			(obj_type[i] == 2 && cylinder_hit(ori_tmp, dir_tmp, &tmp) == 1) ||
			(obj_type[i] == 3 && cone_hit(ori_tmp, dir_tmp, &tmp) == 1)) &&
				tmp < 1)
			return (0);
	}
	return (1);
}

#ifndef SQR_WORK_GROUP_SIZE
# define SQR_WORK_GROUP_SIZE 8
#endif

__kernel void			process_image(
						   __global uchar4	*buf,
						   __global int		*obj_type,
						   __global uchar4	*obj_color,
						   __global float16	*obj_transform,
						   __global float16	*obj_rot_mat,
						   __global float16	*obj_rev_rot,
						   __global float	*spot_lum,
						   __global float3	*spot_pos,
						   int2				img_size,
						   float16			cam_mat,
						   float16			cam_mat_rot,
						   int				nobjs,
						   int				nspots,
						   float			far,
						   float			fov
						  )
{
	__local uchar4	tmp_buf[SQR_WORK_GROUP_SIZE][SQR_WORK_GROUP_SIZE];
	int2			id;
	int2			wg_id;
	int				buf_pos;
	float			s;
	int				i;

	if ((id.x = get_global_id(0)) >= img_size.x
			|| (id.y = get_global_id(1)) >= img_size.y)
		return ;
	wg_id = (int2)(get_local_id(0), get_local_id(1));
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

	// Iterate throught all the objects to detect the first one hit by the ray
	float3			ori_tmp;
	float3			dir_tmp;
	float			hit;
	float			tmp;
	int				obj_hit;

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
			(obj_type[i] == 3 && cone_hit(ori_tmp, dir_tmp, &tmp) == 1)) &&
				tmp < hit)
		{
			hit = tmp;
			obj_hit = i;
		}
	}

	// If the closest object hit is closer then the far plane, calculate the luminosity and display the processed color
	float			lum;
	float3			v_hit;
	float3			normal;
	float3			r_in;
	float3			r_out;
	float			a_in;
	float			reflected;

	hit -= 0.02; // To prevent precision errors
	lum = 1;
	if (hit < far)
	{
		v_hit = ori + dir * hit;
		normal = get_normal(obj_type[obj_hit], obj_transform[obj_hit], obj_rev_rot[obj_hit], v_hit);
		lum = 0.15;
		reflected = 0;
		i = -1;
		while (++i < nspots)
			if (can_trace(obj_type, obj_transform, obj_rot_mat, nobjs, spot_pos[i], v_hit))
			{
				r_in = v_hit - spot_pos[i];
				a_in = dot(normalize(r_in), normal);
				r_out = normalize(normalize(r_in) - (normal * 2 * a_in));
				reflected = pow(fmax(dot(r_out, -dir), 0), (float)1200);
				//reflected = pow(dot(normalize(r_in), normal) , (float)400);
				if (a_in > 0)
					lum += spot_lum[i] * a_in / pow(length(r_in), (float)2) + reflected * spot_lum[i];
			}
		if (lum < 1)
			tmp_buf[wg_id.y][wg_id.x] = mix((uchar4)(0x00, 0x00, 0x00, 0x00), (obj_color[obj_hit]), lum);
		else
			tmp_buf[wg_id.y][wg_id.x] = mix((uchar4)(0xFF, 0xFF, 0xFF, 0x00), (obj_color[obj_hit]), 1 / lum);
	}
	else
		tmp_buf[wg_id.y][wg_id.x] = (uchar4)(0x00, 0x00, 0x00, 0x00);
	buf[buf_pos] = tmp_buf[wg_id.y][wg_id.x];
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
		dest_buf[buf_pos] = color;
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
		dest_buf[buf_pos] = color;
	}
	else
		dest_buf[buf_pos] = origin_buf[buf_pos];
}

__kernel void	clear_buf(__global uchar4 *buf,
							uchar4 val)
{
	int		id;

	id = get_global_id(0);
	buf[id] = (uchar4)(val.r, val.g, val.b, 0x00);
}
