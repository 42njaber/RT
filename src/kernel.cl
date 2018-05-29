
#include "structs.h"

uchar4 __OVERLOAD__ mix(uchar4 c1, uchar4 c2, float gradiant);
void					put_px(int2 px, uchar4 color, __global uchar4 *buf, int2 img_size);
uchar4					choose_color(float gradiant, int iter, int iter_max, float shift, char palette);
float2					c_mult(float2 z1, float2 z2);
float2					c_pow(float2 z, int pow);
uchar4					sky(float gradiant, float shift);
uchar4					neon(float gradiant, float shift);
uchar4					black_white(float gradiant, float shift);
int						sphere_hit(t_obj *obj, float3 ori, float3 dir, float *t)
int						plane_hit(float3 ori, float3 dir, float *t)
int						cylinder_hit(t_obj *obj, float3 ori, float3 dir, float *t)
int						cone_hit(t_obj *obj, float3 ori, float3 dir, float *t)

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

int			sphere_hit(t_obj *obj, float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float	d;

	a = sqr(dir.x) + sqr(dir.y) + sqr(dir.z);
	b = 2 * (ori.x * dir.x + ori.y * dir.y + ori.z * dir.z);
	c = sqr(ori.x) + sqr(ori.y) + sqr(ori.z) - sqr(obj->size);
	d = sqr(b) - 4 * a * c;
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

int			cylindre_hit(t_obj *obj, float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float	d;

	a = sqr(dir.x) + sqr(dir.z);
	b = 2 * (ori.x * dir.x + ori.z * dir.z);
	c = sqr(ori.x) + sqr(ori.z) - sqr(obj->size);
	d = sqr(b) - 4 * a * c;
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

int			cone_hit(t_obj *obj, float3 ori, float3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float	d;

	a = sqr(dir.x) + sqr(dir.z) - sqr(dir.y * obj->size);
	b = 2 * (ori.x * dir.x + ori.z * dir.z - ori.y * dir.y * sqr(obj->size));
	c = sqr(ori.x) + sqr(ori.z) - sqr(ori.y * obj->size);
	d = sqr(b) - 4 * a * c;
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

__kernel void			process_image(
						   __global uchar4	*buf,
						   __global uchar4	*buf,
						   __global uchar4	*buf,
						   __global uchar4	*buf,
						   __global uchar4	*buf,
						   __global uchar4	*buf,
						   __global uchar4	*buf,
						   uint		pixel_sz,
						   uint		line,
						   int2		img_size
						  )
{
	int		id;

	id = get_global_id(0);
	buf[id] = (uchar4)(0xFF, 0xAA, 0x00, 0x00);
}

__kernel void	clear_buf(__global uchar4 *buf,
							uchar4 val)
{
	int		id;

	id = get_global_id(0);
	buf[id] = (uchar4)(val.r, val.g, val.b, 0x00);
}
