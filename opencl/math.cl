
#include "math.clh"

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

float					angle(float2 v)
{
	return (acos(v.x / length(v)) * (v.y < 0 ? -1 : 1) + M_PI_F);
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

	out.x = (isnan(in.y) || in.x < in.y) ? in.x : in.y;
	out.y = (isnan(in.y) || in.x < in.y) ? in.y : in.x;
	return (out);
}

float3 __OVERLOAD__		order(float3 in)
{
	float3	out;

	out.x = (isnan(in.z) || in.x < in.z) ? ((isnan(in.y) || in.x < in.y) ? in.x : in.y) : ((isnan(in.y) || in.z < in.y) ? in.z : in.y);
	out.y = in.x < in.y ? (in.y < in.z ? in.y : in.x < in.z ? in.z : in.z) : (in.x < in.z ? in.x : in.y < in.z ? in.z : in.y);
	out.z = (isnan(in.z) || in.x < in.z) ? ((isnan(in.y) || in.z < in.y) ? in.y : in.z) : ((isnan(in.y) || in.x < in.y) ? in.y : in.x);
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
