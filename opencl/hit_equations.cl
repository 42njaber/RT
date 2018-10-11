
#include "math.clh"
#include "hit_equations.clh"

#define TORUS_RADIUS	(float)2
#define TORUS_WIDTH		(float)1

int			sphere_hit(float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float2	inter;

	if (length(cross(ori, ori + dir)) > 1)
		;//return (0);
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

	if (length(cross(ori, ori + dir)) > 1)
		;//return (0);
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

int					torus_hit(float3 ori, float3 dir, float *t)
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

	dir_sqr_sum = pow(dir.x, 2) + pow(dir.y, 2) + pow(dir.z, 2);
	ori_sqr_sum = pow(ori.x, 2) + pow(ori.y, 2) + pow(ori.z, 2);
	dir_ori_sum = dir.x * ori.x + dir.y * ori.y + dir.z * ori.z;
	r_sqr_sum = pow(TORUS_RADIUS, 2) + pow(TORUS_WIDTH, 2);
	a = pow(dir_sqr_sum, 2);
	b = 4 * dir_sqr_sum * dir_ori_sum;
	c = 2 * dir_sqr_sum * (ori_sqr_sum - r_sqr_sum) + 4 * pow(dir_ori_sum, 2) + 4 * pow(TORUS_RADIUS, 2) * pow(dir.z, 2);
	d = 4 * (ori_sqr_sum - r_sqr_sum) * dir_ori_sum + 8 * pow(TORUS_RADIUS, 2) * ori.z * dir.z;
	e = pow(ori_sqr_sum - r_sqr_sum, 2) - 4 * pow(TORUS_RADIUS, 2) * (pow(TORUS_WIDTH, 2) - pow(ori.z, 2));
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

static int				moebius_in_band(float3 hit)
{
	float2	band_pos;

	band_pos = (float2)(length((float2)(hit.x, hit.y)) - 2, hit.z);
	if (length(band_pos) < 1 && fabs(fmod(angle(band_pos) + M_PI_F / 2, M_PI_F) - angle(hit.xy) / 2) < 0.01)
		return (1);
	return (0);
}

int						moebius_hit(float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float	d;
	float3	inter;

	a = pow(dir.x, 2) * dir.y + pow(dir.y, 3) - 2 * pow(dir.x, 2) * dir.z - 2 * pow(dir.y, 2) * dir.z + dir.y * pow(dir.z, 2);
	b = -4 * dir.x * dir.z + 2 * dir.x * dir.y * ori.x - 4 * dir.x * dir.z * ori.x + pow(dir.x, 2) * ori.y + 3 * pow(dir.y, 2) * ori.y
			- 4 * dir.y * dir.z * ori.y + pow(dir.z, 2) * ori.y - 2 * pow(dir.x, 2) * ori.z - 2 * pow(dir.y, 2) * ori.z + 2 * dir.y * dir.z * ori.z;
	c = -4 * dir.y - 4 * dir.z * ori.x + dir.y * pow(ori.x, 2) - 2 * dir.z * pow(ori.x, 2) + 2 * dir.x * ori.x * ori.y + 3 * dir.y * pow(ori.y, 2)
			- 2 * dir.z * pow(ori.y, 2) - 4 * dir.x * ori.z - 4 * dir.x * ori.x * ori.z - 4 * dir.y * ori.y * ori.z + 2 * dir.z * ori.y * ori.z + dir.y * pow(ori.z, 2);
	d = -4 * ori.y + pow(ori.x, 2) * ori.y + pow(ori.y, 3) - 4 * ori.x * ori.z - 2 * pow(ori.x, 2) * ori.z - 2 * pow(ori.y, 2) * ori.z + ori.y * pow(ori.z, 2);
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

float3					get_normal(int type, float16 transform, float16 rev_rot, float3 v, float3 dir)
{
	float3	ret;
	float	tmp;

	v = vec_mat_mult(transform, v);
	if (type == 0)
		ret = (float3)(-v.x, -v.y, -v.z);
	else if (type == 1)
		ret = (float3)(0, 0, 1);
	else if (type == 2)
		ret = (float3)(-v.x, 0, -v.z);
	else if (type == 3)
		ret = (float3)(-v.x, v.y, -v.z);
	else if (type == 4)
	{
		tmp = length((float2)(v.x, v.y));
		ret = (float3)((tmp - TORUS_RADIUS) * v.x / tmp, (tmp - TORUS_RADIUS) * v.y / tmp, v.z);
	}
	else if (type == 5)
	{
		tmp = angle(v.xy) / 2;
		ret = (float3)(normalize(v.xy) * cos(tmp), sin(tmp));
	}
	else
		ret = (float3){0, 0, 1};
	ret = vec_mat_mult(rev_rot, ret);
	if (dot(dir, ret) < 0)
		ret *= -1;
	return (normalize(ret));
}
