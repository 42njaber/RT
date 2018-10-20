
#include "math.clh"
#include "hit_equations.clh"

#define TORUS_RADIUS	(float)2
#define TORUS_WIDTH		(float)1

bool			sphere_hit(float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float2	inter;

	a = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
	b = 2 * (ori.x * dir.x + ori.y * dir.y + ori.z * dir.z);
	c = ori.x * ori.x + ori.y * ori.y + ori.z * ori.z - 1;
	inter = quadratic_solver((float2)(b, c) / a);
	if (inter.x < 0.01)
		*t = inter.y;
	else
		*t = inter.x;
	if (*t < 0.01)
		return (0);
	return (1);
}

bool			plane_hit(float3 ori, float3 dir, float *t)
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

bool			cylinder_hit(float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float2	inter;

	a = dir.x * dir.x + dir.z * dir.z;
	b = 2 * (ori.x * dir.x + ori.z * dir.z);
	c = ori.x * ori.x + ori.z * ori.z - 1;
	inter = quadratic_solver((float2)(b, c) / a);
	if (inter.x < 0.01)
		*t = inter.y;
	else
		*t = inter.x;
	if (*t < 0.01)
		return (0);
	return (1);
}

bool			cone_hit(float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float2	inter;

	a = dir.x * dir.x + dir.z * dir.z - dir.y * 1 * dir.y * 1;
	b = 2 * (ori.x * dir.x + ori.z * dir.z - ori.y * dir.y);
	c = ori.x * ori.x + ori.z * ori.z - ori.y * 1 * ori.y * 1;
	inter = quadratic_solver((float2)(b, c) / a);
	if (inter.x < 0.01)
		*t = inter.y;
	else
		*t = inter.x;
	if (*t < 0.01)
		return (0);
	return (1);
}

bool					torus_hit(float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float	d;
	float	e;
	float4	inter;
	float	dir_sqr_sum;
	float	ori_sqr_sum;
	float	dir_ori_sum;
	float	r_sqr_sum;

	dir_sqr_sum = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
	ori_sqr_sum = ori.x * ori.x + ori.y * ori.y + ori.z * ori.z;
	dir_ori_sum = dir.x * ori.x + dir.y * ori.y + dir.z * ori.z;
	r_sqr_sum = TORUS_RADIUS * TORUS_RADIUS + TORUS_WIDTH * TORUS_WIDTH;
	a = dir_sqr_sum * dir_sqr_sum;
	b = 4 * dir_sqr_sum * dir_ori_sum;
	c = 2 * dir_sqr_sum * (ori_sqr_sum - r_sqr_sum) + 4 * dir_ori_sum * dir_ori_sum + 4 * TORUS_RADIUS * TORUS_RADIUS * dir.z * dir.z;
	d = 4 * (ori_sqr_sum - r_sqr_sum) * dir_ori_sum + 8 * TORUS_RADIUS * TORUS_RADIUS * ori.z * dir.z;
	e = (ori_sqr_sum - r_sqr_sum) * (ori_sqr_sum - r_sqr_sum) - 4 * TORUS_RADIUS * TORUS_RADIUS * (TORUS_WIDTH * TORUS_WIDTH - ori.z * ori.z);
	inter = quartic_solver((float4)(b, c, d, e) / a);
	*t = NAN;
	if (inter.x >= 0.01)
		*t = inter.x;
	else if (inter.y >= 0.01)
		*t = inter.y;
	else if (inter.z >= 0.01)
		*t = inter.z;
	else
		*t = inter.w;
	if (*t >= 0.01)
		return (1);
	return (0);
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

bool					moebius_hit(float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float	d;
	float3	inter;

	a = dir.x * dir.x * dir.y + dir.y * dir.y * dir.y - 2 * dir.x * dir.x * dir.z - 2 * dir.y * dir.y * dir.z + dir.y * dir.z * dir.z;
	b = -4 * dir.x * dir.z + 2 * dir.x * dir.y * ori.x - 4 * dir.x * dir.z * ori.x + dir.x * dir.x * ori.y + 3 * dir.y * dir.y * ori.y
			- 4 * dir.y * dir.z * ori.y + dir.z * dir.z * ori.y - 2 * dir.x * dir.x * ori.z - 2 * dir.y * dir.y * ori.z + 2 * dir.y * dir.z * ori.z;
	c = -4 * dir.y - 4 * dir.z * ori.x + dir.y * ori.x * ori.x - 2 * dir.z * ori.x * ori.x + 2 * dir.x * ori.x * ori.y + 3 * dir.y * ori.y * ori.y
			- 2 * dir.z * ori.y * ori.y - 4 * dir.x * ori.z - 4 * dir.x * ori.x * ori.z - 4 * dir.y * ori.y * ori.z + 2 * dir.z * ori.y * ori.z + dir.y * ori.z * ori.z;
	d = -4 * ori.y + ori.x * ori.x * ori.y + ori.y * ori.y * ori.y - 4 * ori.x * ori.z - 2 * ori.x * ori.x * ori.z - 2 * ori.y * ori.y * ori.z + ori.y * ori.z * ori.z;
//	if (fabs(a) < 1e-6)
//		inter.xy = quadratic_solver((float2)(c, d) / b);
//	else
		inter = cubic_solver((float3)(b, c, d) / a);
	*t = NAN;
	if (inter.x >= 0.01 && moebius_in_band(ori + dir * inter.x))
		*t = inter.x;
	else if (inter.y >= 0.01 && moebius_in_band(ori + dir * inter.y))
		*t = inter.y;
	else if (moebius_in_band(ori + dir * inter.z))
		*t = inter.z;
	if (*t >= 0.01)
		return (1);
	return (0);
}

float3					get_normal(__global t_obj *obj, float3 v, float3 dir)
{
	float3	ret;
	float	tmp;

	v = vec_mat_mult(obj->transform, v);
	if (obj->type == 0)
		ret = (float3)(-v.x, -v.y, -v.z);
	else if (obj->type == 1)
		ret = (float3)(0, 0, 1);
	else if (obj->type == 2)
		ret = (float3)(-v.x, 0, -v.z);
	else if (obj->type == 3)
		ret = (float3)(-v.x, v.y, -v.z);
	else if (obj->type == 4)
	{
		tmp = length((float2)(v.x, v.y));
		ret = (float3)((tmp - TORUS_RADIUS) * v.x / tmp, (tmp - TORUS_RADIUS) * v.y / tmp, v.z);
	}
	else if (obj->type == 5)
	{
		tmp = angle(v.xy) / 2;
		ret = (float3)(normalize(v.xy) * cos(tmp), sin(tmp));
	}
	else
		ret = (float3){0, 0, 1};
	ret = vec_mat_mult(obj->rev_norm, ret);
	if (dot(dir, ret) < 0)
		ret *= -1;
	return (normalize(ret));
}
