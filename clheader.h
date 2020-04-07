struct __attribute__ ((packed)) pref{
	float3	center;
	float3	vector;
	float	radius;
	float	min;
	float	max;
	float	k;
};
struct __attribute__ ((packed)) attr{
	float3	color;
	float	shine;
	float	reflect;
	float	refract;
};
struct __attribute__ ((packed)) item{
	struct pref	pref;
	struct attr	attr;
	int			type;
};

struct __attribute__ ((packed)) opt{
	int		w;
	int		h;
	int		illu_c;
	int		item_c;
	float	spin_x;
	float	spin_y;
	float3	center;
};

/*
**	surface func
*/
float	plane(__global struct item *item, float3 center, float3 direct);
float	sphere(__global struct item *item, float3 center, float3 direct);
float	cylinder(__global struct item *item, float3 center, float3 direct);
float3	cylinder_normal(__global struct item *item, float3 point, float3 center, float3 direct, float t);
float	cone(__global struct item *item, float3 center, float3 direct);
float3	cone_normal(__global struct item *item, float3 point, float3 center, float3 direct, float t);
float	paraboloid(__global struct item *item, float3 center, float3 direct);
float3	paraboloid_normal(__global struct item *item, float3 point, float3 center, float3 direct, float t);

/*
**	add func
*/
float	scalar_multiple(float3 a, float3 b);
float	get_rate(float a, float b);
float	calc_ratio(float3 a, float3 b);
float	angle(float3 a, float3 b);
float3	get_reflect_vec(float3 vec, float3 normal);
float3	get_refract_vec(float3 vec, float3 normal, float n1, float n2);
float3	rotation_x(float rad, float3 vec);
float3	rotation_y(float rad, float3 vec);
