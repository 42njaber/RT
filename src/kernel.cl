
float3					vec_mat_mult(float16 mat, float3 vec);
uchar4 __OVERLOAD__		mix(uchar4 c1, uchar4 c2, float gradiant);
void					put_px(int2 px, uchar4 color, __global uchar4 *buf, int2 img_size);
uchar4					choose_color(float gradiant, int iter, int iter_max, float shift, char palette);
float2					c_mult(float2 z1, float2 z2);
float2					c_pow(float2 z, int pow);
uchar4					sky(float gradiant, float shift);
uchar4					neon(float gradiant, float shift);
uchar4					black_white(float gradiant, float shift);
float2 __OVERLOAD__		order(float2 in);
float3 __OVERLOAD__		order(float3 in);
float4 __OVERLOAD__		order(float4 in);
float2					quadratic_solver(float2 parm);
float3					cubic_solver(float3 parm);
//float4					quartic_solver(float4 parm, __global uchar4 *buf, int id, int2 img_size);
float4					quartic_solver(float4 parm);
int						sphere_hit(float3 ori, float3 dir, float *t);
int						plane_hit(float3 ori, float3 dir, float *t);
int						cylinder_hit(float3 ori, float3 dir, float *t);
int						torus_hit(float3 ori, float3 dir, float *t);
int						cone_hit(float3 ori, float3 dir, float *t);
float3					get_normal(int type, float16 transform, float16 rev_rot, float3 v, float3 dir);
int						can_trace(__global int *obj_type, __global float16 *obj_transform, __global float16	*obj_rot_mat, int nobjs, float3 v1, float3 v2);
float4					get_point_color(__global int *obj_type, __global uchar4 *obj_color, __global float16 *obj_transform, __global float16 *obj_rot_mat,
										__global float16 *obj_rev_rot, __global float3 *spot_pos, __global float *spot_lum, int nspots, int nobjs, int obj_hit,
										float3 v, float3 dir);
void					write_nb(__global uchar4 *buf, int id, int2 img_size, int nb, int x, int y);

constant char nb_minus[16] =	".........XXX....";
constant char nb_plus[16] =		"......X..XXX..X.";
constant char nb_0[16] =		".XX.X..XX..X.XX.";
constant char nb_1[16] =		".X..XX...X..XXX.";
constant char nb_2[16] =		".XX.X..X..X.XXXX";
constant char nb_3[16] =		"XXX....X..XXXXX.";
constant char nb_4[16] =		"..X..XX.XXXX..X.";
constant char nb_5[16] =		"XXXXXX....X.XX..";
constant char nb_6[16] =		".XX....XXXXX.XX.";
constant char nb_7[16] =		"XXXX..X...X..X..";
constant char nb_8[16] =		".XXX.X.XX.X.XXX.";
constant char nb_9[16] =		".XX.XXXX...X.XX.";
constant char nb_X[16] =		"X..X.XX..XX.X..X";

/* LOGGER
	int		i;
	float	test;
	i = -1;
	while (++i < 4)
	{
		test = root[i];
		if (isinf(test))
		{
			write_nb(buf, buf_pos, img_size, -sign(test) - 2					, 10, 70 + i * 10);
		}
		else if (isnan(test))
		{
			write_nb(buf, buf_pos, img_size, -4									, 10, 70 + i * 10);
		}
		else
		{
			write_nb(buf, buf_pos, img_size, copysign(1, test) - 2				, 10, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, (int)fabs(test) / 100000000		, 20, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, ((int)fabs(test) / 10000000) % 10	, 30, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, ((int)fabs(test) / 1000000) % 10	, 40, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, ((int)fabs(test) / 100000) % 10	, 50, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, ((int)fabs(test) / 10000) % 10		, 60, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, ((int)fabs(test) / 1000) % 10		, 70, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, ((int)fabs(test) / 100) % 10		, 80, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, ((int)fabs(test) / 10) % 10		, 90, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, (int)fabs(test) % 10				, 100, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, -2									, 110, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, (int)(fabs(test) * 10) % 10		, 120, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, (int)(fabs(test) * 100) % 10		, 130, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, (int)(fabs(test) * 1000) % 10		, 140, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, (int)(fabs(test) * 10000) % 10		, 150, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, (int)(fabs(test) * 100000) % 10	, 160, 70 + i * 10);
			write_nb(buf, buf_pos, img_size, (int)(fabs(test) * 1000000) % 10	, 170, 70 + i * 10);
		}
	}
*/

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

uchar4 __OVERLOAD__		mix(uchar4 c1, uchar4 c2, float gradiant)
{
	float4	tmp;
	float4	tmp1;
	float4	tmp2;

	tmp1 = (float4)(c1.x, c1.y, c1.z, c1.w);
	tmp2 = (float4)(c2.x, c2.y, c2.z, c2.w);
	tmp = mix(tmp1, tmp2, gradiant);
	return ((uchar4)(tmp.x, tmp.y, tmp.z, tmp.w));
}

float2 __OVERLOAD__		order(float2 in)
{
	float2	out;

	out.x = fmin(in.x, in.y);
	out.y = fmax(in.x, in.y);
	return (out);
}

float3 __OVERLOAD__		order(float3 in)
{
	float3	out;

	if (in.x < in.y || isnan(out.z))
		out.xy = in.xy;
	else
		out.xy = in.yx;
	if (in.z < out.x || isnan(out.z))
		out = (float3)(in.z, out.xy);
	else if (in.z < out.y || isnan(out.z))
		out = (float3)(out.x, in.z, out.y);
	else
		out.z = in.z;
	return (out);
}

float4 __OVERLOAD__		order(float4 in)
{
	float4	out;

	if (in.x < in.y || isnan(in.y))
		out.xy = in.xy;
	else
		out.xy = in.yx;
	if (in.z < out.x || isnan(out.x))
		out.xyz = (float3)(in.z, out.xy);
	else if (in.z < out.y || isnan(out.y))
		out.xyz = (float3)(out.x, in.z, out.y);
	else
		out.z = in.z;
	if (in.w < out.x || isnan(out.x))
		out = (float4)(in.w, out.xyz);
	else if (in.w < out.y || isnan(out.y))
		out = (float4)(out.x, in.w, out.yz);
	else if (in.w < out.z || isnan(out.z))
		out = (float4)(out.xy, in.w, out.z);
	else
		out.w = in.w;
	return (out);
}

//float4					quartic_solver(float4 parm, __global uchar4 *buf, int buf_pos, int2 img_size)
float4					quartic_solver(float4 parm)
{
	float4	root;
	float	p;
	float	q;
	float	r;
	float3	cub_res;
	float3	cub_eq;
	float	m;
	float	root2m;
	float	tmp;
	float2	w;
	float2	z;
	float	sub;

	root = NAN;
	p = parm.y - 3 * pow(parm.x, 2) / 8;
	q = parm.z - parm.x * parm.y / 2 + pow(parm.x, 3) / 8;
	r = parm.w - parm.x * parm.z / 4 + pow(parm.x, 2) * parm.y / 16 - 3 * pow(parm.x, 4) / 256;
	sub = -parm.x / 4;
	if (fabs(p) < 0.001 && fabs(q) < 0.001 && fabs(r) < 0.001)
		return ((float4)(sub, 0, 0, 0));
	cub_eq.x = p;
	cub_eq.y = pow(p, 2) / 4 - r;
	cub_eq.z = -pow(q, 2) / 8;
	cub_res = cubic_solver(cub_eq);
	if (fabs(cub_res.x) > fmax(fabs(cub_res.y), fabs(cub_res.z)) && !isinf(cub_res.x))
		m = cub_res.x;
	else
	{
		if (fabs(cub_res.y) < fabs(cub_res.z) && !isinf(cub_res.z))
			m = fabs(cub_res.z);
		else if (!isinf(cub_res.y))
			m = fabs(cub_res.y);
		else
			m = fabs(cub_res.x);
	}
	if (m >= 0)
	{
		root2m = sqrt(2 * m);
		root.xy = quadratic_solver((float2)(root2m, p / 2 + m - q / (2 * root2m))) + sub;
		root.zw = quadratic_solver((float2)(-root2m, p / 2 + m + q / (2 * root2m))) + sub;
	}
	else
	{
		root2m = sqrt(-2 * m);
		w.y = -2 * p / root2m;
		w.x = -2 * p - 2 * m;
		tmp = length(w);
		z = sqrt(tmp) * (float2)(w.x + tmp, w.y) / length((float2)(w.x + tmp, w.y));
		if (fabs(fabs(z.y) - root2m) < 0.01)
			root.xy = (float2)(z.x, -z.x) / 2 + sub;
	}

	//float3	test2 = ((float3)(p, q, r));
	root = order(root);
	return (root);
}

float3					cubic_solver(float3 parm)
{
	float3	root;
	float	p;
	float	q;
	float	delta;
	float	root3;
	float	tmp1;
	float	tmp2;
	float	sub;

	root = NAN;
	p = parm.y - pow(parm.x, 2) / 3;
	q = 2 * pow(parm.x, 3) / 27 - parm.y * parm.x / 3 + parm.z;
	sub = -parm.x / 3;
	delta = pow(q, 2) / 4 + pow(p, 3) / 27;
	if (delta > 0)
		root.s0 = cbrt(-q / 2 + sqrt(delta)) + cbrt(-q / 2 - sqrt(delta)) + sub;
	else if (delta == 0)
	{
		root.s0 = -2 * cbrt(q / 2) + sub;
		root.s1 = cbrt(q / 2) + sub;
	}
	else
	{
		root3 = sqrt((float)3);
		tmp2 = sqrt(-p);
		tmp1 = (2 / root3) * tmp2;
		tmp2 = asin((3 * root3 * q) / (2 * pow(tmp2, 3))) / 3;
		root.s0 = tmp1 * sin(tmp2) + sub;
		root.s1 = -tmp1 * sin(tmp2 + M_PI_F / 3) + sub;
		root.s2 = tmp1 * cos(tmp2 + M_PI_F / 6) + sub;
	}
	return (order(root));
}

float2					quadratic_solver(float2 parm)
{
	float	delta;
	float2	root;

	root = NAN;
	delta = pow(parm.x, 2) - 4 * parm.y;
	if (delta < 0)
		return (root);
	else if (delta == 0)
	{
		root.s1 = -parm.x / 2;
		return (root);
	}
	root.s0 = (-parm.x - sqrt(delta)) / 2;
	root.s1 = (-parm.x + sqrt(delta)) / 2;
	return (order(root));
}

int			sphere_hit(float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float2	inter;

	a = pow(dir.x, 2) + pow(dir.y, 2) + pow(dir.z, 2);
	b = 2 * (ori.x * dir.x + ori.y * dir.y + ori.z * dir.z);
	c = pow(ori.x, 2) + pow(ori.y, 2) + pow(ori.z, 2) - 1;
	inter = quadratic_solver((float2)(b, c) / a);
	if (inter.x < 0.01)
		*t = inter.y;
	else
		*t = inter.x;
	if (*t < 0.01)
		return (0);
	return (1);
}

int			plane_hit(float3 ori, float3 dir, float *t)
{
	float	a;

	if (dir.z == 0)
		return (0);
	a = -ori.z / dir.z;
	if (a < 0.01)
		return (0);
	*t = a;
	return (1);
}

int			cylinder_hit(float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float2	inter;

	a = pow(dir.x, 2) + pow(dir.z, 2);
	b = 2 * (ori.x * dir.x + ori.z * dir.z);
	c = pow(ori.x, 2) + pow(ori.z, 2) - 1;
	inter = quadratic_solver((float2)(b, c) / a);
	if (inter.x < 0.01)
		*t = inter.y;
	else
		*t = inter.x;
	if (*t < 0.01)
		return (0);
	return (1);
}

int			cone_hit(float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float2	inter;

	a = pow(dir.x, 2) + pow(dir.z, 2) - pow(dir.y * 1, 2);
	b = 2 * (ori.x * dir.x + ori.z * dir.z - ori.y * dir.y);
	c = pow(ori.x, 2) + pow(ori.z, 2) - pow(ori.y * 1, 2);
	inter = quadratic_solver((float2)(b, c) / a);
	if (inter.x < 0.01)
		*t = inter.y;
	else
		*t = inter.x;
	if (*t < 0.01)
		return (0);
	return (1);
}

int			torus_hit(float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float	d;
	float	e;
	float4	inter;
	float	R;
	float	r;

	R = 2;
	r = 0.5;
	a = pow(dir.x, 4) + pow(dir.y, 4) + pow(dir.z, 4) + 2 * pow(dir.x, 2) * pow(dir.y, 2)
		+ 2 * pow(dir.x, 2) * pow(dir.z, 2) + 2 * pow(dir.y, 2) * pow(dir.z, 2);
	b = 4 * pow(dir.x, 3) * ori.x + 4 * pow(dir.y, 3) * ori.y + 4 * pow(dir.z, 3) * ori.z + 4 * pow(dir.x, 2) * dir.y * ori.y
		+ 4 * pow(dir.x, 2) * dir.z * ori.z + 4 * dir.x * ori.x * pow(dir.y, 2) + 4 * pow(dir.y, 2) * dir.z * ori.z
		+ 4 * dir.x * ori.x * pow(dir.z, 2) + 4 * dir.y * ori.y * pow(dir.z, 2);
	c = - 2 * pow(R, 2) * pow(dir.x, 2) - 2 * pow(R, 2) * pow(dir.y, 2) + 2 * pow(R, 2) * pow(dir.z, 2) - 2 * pow(r, 2) * pow(dir.x, 2)
		- 2 * pow(r, 2) * pow(dir.y, 2) - 2 * pow(r, 2) * pow(dir.z, 2) + 6 * pow(dir.x, 2) * pow(ori.x, 2) + 2 * pow(ori.x, 2) * pow(dir.y, 2)
		+ 8 * dir.x * ori.x * dir.y * ori.y + 2 * pow(dir.x, 2) * pow(ori.y, 2) + 6 * pow(dir.y, 2) * pow(ori.y, 2) + 2 * pow(ori.x, 2) * pow(dir.z, 2)
		+ 2 * pow(ori.y, 2) * pow(dir.z, 2) + 8 * dir.x * ori.x * dir.z * ori.z + 8 * dir.y * ori.y * dir.z * ori.z + 2 * pow(dir.x, 2) * pow(ori.z, 2)
		+ 2 * pow(dir.y, 2) * pow(ori.z, 2) + 6 * pow(dir.z, 2) * pow(ori.z, 2);
	d = - 4 * pow(R, 2) * dir.x * ori.x - 4 * pow(R, 2) * dir.y * ori.y + 4 * pow(R, 2) * dir.z * ori.z - 4 * pow(r, 2) * dir.x * ori.x
		- 4 * pow(r, 2) * dir.y * ori.y - 4 * pow(r, 2) * dir.z * ori.z + 4 * dir.x * pow(ori.x, 3) + 4 * pow(ori.x, 2) * dir.y * ori.y
		+ 4 * dir.x * ori.x * pow(ori.y, 2) + 4 * dir.y * pow(ori.y, 3) + 4 * pow(ori.x, 2) * dir.z * ori.z + 4 * pow(ori.y, 2) * dir.z * ori.z
		+ 4 * dir.x * ori.x * pow(ori.z, 2) + 4 * dir.y * ori.y * pow(ori.z, 2) + 4 * dir.z * pow(ori.z, 3);
	e = pow(R, 4) - 2 * pow(R, 2) * pow(ori.x, 2) - 2 * pow(R, 2) * pow(ori.y, 2) + 2 * pow(R, 2) * pow(ori.z, 2) + pow(r, 4) - 2 * pow(r, 2) * pow(R, 2)
		- 2 * pow(r, 2) * pow(ori.x, 2) - 2 * pow(r, 2) * pow(ori.y, 2) - 2 * pow(r, 2) * pow(ori.z, 2) + pow(ori.x, 4) + pow(ori.y, 4) + pow(ori.z, 4)
		+ 2 * pow(ori.x, 2) * pow(ori.y, 2) + 2 * pow(ori.x, 2) * pow(ori.z, 2) + 2 * pow(ori.y, 2) * pow(ori.z, 2);
	inter = quartic_solver((float4)(b, c, d, e) / a);
	if (inter.z < 0.01)
		*t = inter.w;
	else if (inter.y < 0.01)
		*t = inter.z;
	else if (inter.x < 0.01)
		*t = inter.y;
	else
		*t = inter.x;
	if (*t < 0.01)
		return (0);
	return (1);
}

float3					get_normal(int type, float16 transform, float16 rev_rot, float3 v, float3 dir)
{
	float3	ret;

	v = vec_mat_mult(transform, v);
	if (type == 0)
		ret = (float3)(-v.x, -v.y, -v.z);
	else if (type == 1)
		ret = (float3)(0, 0, 1);
	else if (type == 2)
		ret = (float3)(-v.x, 0, -v.z);
	else if (type == 3)
		ret = (float3)(-v.x, v.y, -v.z);
	else
		ret = (float3){0, 0, 1};
	ret = vec_mat_mult(rev_rot, ret);
	if (dot(dir, ret) < 0)
		ret *= -1;
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
	float			len;
	float			tmp;
	int				i;

	line = v2 - v1;
	i = -1;
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
			(obj_type[i] == 4 && torus_hit(ori_tmp, dir_tmp, &tmp) == 1)) &&
				tmp < len - 0.01)
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
	if (fabs(fmod(v.x + 0.2, (float)20)) < 0.03 || fabs(fmod(v.y + 0.2, (float)20)) < 0.03 || fabs(fmod(v.z + 0.2, (float)20)) < 0.03)
		color = 1 / color;
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
				(obj_type[i] == 4 && torus_hit(ori_tmp, dir_tmp, &tmp) == 1)) &&
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

	//quartic_solver((float4)(-0.1970370674, 148.5148515, -49752.85455, 6.25e6) / (float)9.802960494e-5, buf, buf_pos, img_size);
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
