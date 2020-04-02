#include "rtv1.h"

double		angle_vec(t_vector v1, t_vector v2)
{
	return acos(scalar_multiple(v1, v2) / (len_vector(v1) * len_vector(v2)));
}

t_vector	normalize_vec(t_vector v)
{
	return (divide_vector_by_num(v, len_vector(v)));
}

t_vector	get_reflect_vec(t_vector d, t_vector n)
{
	double		k;
	t_vector	reflect_v;

	k = (-1.0) / scalar_multiple(d, n);
	reflect_v = multiple_vector_by_num(d, k);
	reflect_v = add_vector_to_vector(reflect_v, n);
	reflect_v = add_vector_to_vector(n, reflect_v);
	reflect_v = normalize_vec(reflect_v);
	return (reflect_v);
}

t_vector	get_refract_vec(t_vector d, t_vector n, double n1, double n2)
{
	double		k;
	t_vector	refract_v;
	double		scalar;

	if ((scalar = scalar_multiple(d, n)) == 0.0)
		return ((t_vector){0, 0, 0});
	k = (-1.0) / scalar;
	refract_v = multiple_vector_by_num(d, k);
	refract_v = add_vector_to_vector(refract_v, n);
	if ((k = angle_vec(multiple_vector_by_num(d, -1.0), n)) == 0.0)
		return (normalize_vec(d));
	k = (n1 / n2) * sin(k);
	k = tan(asin(k)) / len_vector(refract_v);
	refract_v = multiple_vector_by_num(refract_v, k);
	refract_v = add_vector_to_vector(
		multiple_vector_by_num(n, -1.0), refract_v);
	refract_v = normalize_vec(refract_v);
	return (refract_v);
}
