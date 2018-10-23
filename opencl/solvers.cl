
#include "math.clh"

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
	cub_res = cubic_solver((float4)(1.f, cub_eq));
	if (fabs(cub_res.x) > fmax(fabs(cub_res.y), fabs(cub_res.z)) && !isinf(cub_res.x))
		m = cub_res.x;
	else
	{
		if (fabs(cub_res.z) > fabs(cub_res.y) && !isinf(cub_res.z))
			m = fabs(cub_res.z);
		else if (!isinf(cub_res.y) && !isnan(cub_res.y))
			m = fabs(cub_res.y);
		else
			m = fabs(cub_res.x);
	}
	if (m >= 0)
	{
		root2m = sqrt(2 * m);
		root.xy = quadratic_solver((float3)(1, root2m, p / 2 + m - q / (2 * root2m))) + sub;
		root.zw = quadratic_solver((float3)(1, -root2m, p / 2 + m + q / (2 * root2m))) + sub;
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
	root = order(root);
	return (root);
}

float3					cubic_solver(float4 parm)
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
	p = (parm.z * parm.x - parm.y * parm.y / 3) / parm.x / parm.x;
	q = (2 * parm.y * parm.y * parm.y / 27 - parm.x * parm.y * parm.z / 3 + parm.x * parm.x * parm.w) / (parm.x * parm.x * parm.x);
	sub = -parm.y / 3 / parm.x;
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

float2					quadratic_solver(float3 parm)
{
	float	delta;
	float2	root;

	root = NAN;
	delta = parm.y * parm.y - 4.f * parm.x * parm.z;
	if (delta < 0)
		return (root);
	root.s0 = (-parm.y + sqrt(delta)) / (2 * parm.x);
	root.s1 = (-parm.y - sqrt(delta)) / (2 * parm.x);
	return (order(root));
}
