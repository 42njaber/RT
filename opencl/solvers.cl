
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
	cub_res = cubic_solver(cub_eq);
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