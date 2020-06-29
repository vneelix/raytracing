#ifndef CLHEADER_H
#define CLHEADER_H

#include "quaternion.h"
#include "affine_transform.h"

#define DPH 6
#define CLR (1 << DPH + 1) - 2
#define STE (1 << DPH) / 2

enum __attribute__ ((packed)) iType {
	POINT,
	PLANE,
	SPHERE,
	CYLINDER,
	CONE,
	ELLIPSOID,
	PARABOLOID,
	BOX
};

struct RT_Data {
	int illuNumber;
	int itemNumber;
	__global struct item *illu;
	__global struct item *item;
	__global float3 *distribution;
	int distributionSize;
};

struct __attribute__ ((packed)) item {
	/*
	** Item params
	*/
	enum iType	type;
	float3			center;
	float3			normal;
	float3			vector;
	float3			vMin, vMax;
	float				radius, min, max, k;
	/*
	** Item properties
	*/
	float3			color;
	float				reflectRatio, refractRatio, shineRatio;
	/*
	** Euler angles
	*/
	float				x, y, z;
};

struct __attribute__ ((packed)) opt {
	int		w;
	int		h;
	int		illu_c;
	int		item_c;
	float	spin_x;
	float	spin_y;
	float3	center;
};

/*
** Intersect
*/
float	PlaneIntersect(__global struct item *item, float3 *orig, float3 *dir);
float	SphereIntersect(__global struct item *item, float3 *orig, float3 *dir);
float	CylinderIntersect(__global struct item *item, float3 *orig, float3 *dir);
float	ConeIntersect(__global struct item *item, float3 *orig, float3 *dir);
float	EllipsoidIntersect(__global struct item *item, float3 *orig, float3 *dir);
float	ParaboloidIntersect(__global struct item *item, float3 *orig, float3 *dir);
float	BoxIntersect(__global struct item *item, float3 *orig, float3 *dir);
/*
**
*/

/*
** Normal
*/
float3 CylinderNormal(__global struct item *item, float3 *orig, float3 *dir, float3 *point, float t);
float3 ConeNormal(__global struct item *item, float3 *orig, float3 *dir, float3 *point, float t);
float3 EllipsoidNormal(__global struct item *item, float3 *orig, float3 *dir, float3 *point, float t);
float3 ParaboloidNormal(__global struct item *item, float3 *orig, float3 *dir, float3 *point, float t);
float3 BoxNormal(__global struct item *item, float3 *point);
/*
**
*/


/*
**	add func
*/
void	swap(float *a, float *b);
float	IsOutside(float3 direct, float3 normal);
float	DotProduct(float3 a, float3 b);
float3 CrossProduct(float3 a, float3 b);
float	get_rate(float a, float b);
float	calc_ratio(float3 a, float3 b);
float	angle(float3 a, float3 b);
float3	GetReflectVec(float3 direct, float3 normal);
float3	GetRefractVec(float3 direct, float3 normal, float etat);
float	fresnel(float3 direct, float3 normal, float etat);
float3	rotation_x(float rad, float3 vec);
float3	rotation_y(float rad, float3 vec);
float	minimal_param(__global struct item *item,
			int count, float3 center, float3 direct, int *item_index);
uint	RGBtoUint(float3 rgb);

bool	SolveQuadratic(float a, float b, float c, float *t1, float *t2);

void PrintVec3f(float3 vec3f) {
	printf("%f,%f,%f\n", vec3f.x, vec3f.y, vec3f.z);
}

float3	GetPointOnHemisphere(float k, float alpha);
void		GetDistributionOnHemisphere(float3 *dist, uint circleNumber, int circlePartNumber);
#endif
