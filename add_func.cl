float	scalar_multiple(float3 a, float3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

float	get_rate(float a, float b)
{
	if (a < 0 && b < 0)
		return (INFINITY);
	if ((a < b && a > 0 && b > 0) || (a > b && a > 0 && b < 0))
		return (a);
	return (b);
}

float	calc_ratio(float3 a, float3 b)
{
	return (scalar_multiple(normalize(a), normalize(b)));
}

float	angle(float3 a, float3 b)
{
	return (acos(scalar_multiple(normalize(a), normalize(b))));
}

float3	get_reflect_vec(float3 vec, float3 normal)
{
	float3	ret;

	normal *= scalar_multiple(-vec, normal);
	return (normalize(2*normal + vec));
}

float3	get_refract_vec(float3 vec, float3 normal, float n1, float n2)
{
	float	a, b;
	float3	ret;

	a = angle(-vec, normal);
	b = asin((n1 / n2) * sin(a));
	ret = normalize(vec + normal * scalar_multiple(-vec, normal)) * tan(b);
	ret = normalize(-normal + ret);
	return (ret);
}

float3 rotation_x(float rad, float3 vec)
{
  float3	ret;

  ret.x = vec.x;
  ret.y = vec.y * cos(rad) - vec.z * sin(rad);
  ret.z = vec.y * sin(rad) + vec.z * cos(rad);
  return (ret);
}

float3 rotation_y(float rad, float3 vec)
{
  float3	ret;

  ret.x = vec.x * cos(rad) + vec.z * sin(rad);
  ret.y = vec.y;
  ret.z = vec.x * -(sin(rad)) + vec.z * cos(rad);
  return (ret);
}
