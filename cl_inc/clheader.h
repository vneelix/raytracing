#ifndef CLHEADER_H
#define CLHEADER_H

#include "quaternion.h"
#include "affine_transform.h"

#define DPH 2

#define NULL (void*)(0)

enum iType {
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
	global struct item *illu;
	global struct item *item;
	/* Monte Carlo data */
	global float3 *distribution;
	int distributionSize;
	/* Tree Data */
	struct node *node;
};

struct item {
	/*
	** Item params
	*/
	enum iType	type;
	float3	center;
	float3	normal;
	float3	vector;
	float3	vMin, vMax;
	float	radius, min, max, k;
	/*
	** Item properties
	*/
	float3	color;
	float	reflectRatio, refractRatio, shineRatio;
	/*
	** Euler angles
	*/
	float	x, y, z;
};

struct node {
	float3	orig;
	float3	reflectVec3f;
	float3	refractVec3f;
	float	fresnelRatio;
	global struct item *item;
};

struct tree {
	float3 color[2 * ((1 << DPH) - 1) + 1];
	global struct item *item[1 << (DPH - 1) + 1];
};

struct opt {
	int		w;
	int		h;
	int		illu_c;
	int		item_c;
	float	spin_x;
	float	spin_y;
	float3	center;
};

float	PlaneIntersect(global struct item *item, float3 *orig, float3 *dir);
float	SphereIntersect(global struct item *item, float3 *orig, float3 *dir);
float	CylinderIntersect(global struct item *item, float3 *orig, float3 *dir);
float	ConeIntersect(global struct item *item, float3 *orig, float3 *dir);
float	EllipsoidIntersect(global struct item *item, float3 *orig, float3 *dir);
float	ParaboloidIntersect(global struct item *item, float3 *orig, float3 *dir);
float	BoxIntersect(global struct item *item, float3 *orig, float3 *dir);
float3	CylinderNormal(global struct item *item, float3 *orig, float3 *dir, float3 *point, float t);
float3	ConeNormal(global struct item *item, float3 *orig, float3 *dir, float3 *point, float t);
float3	EllipsoidNormal(global struct item *item, float3 *orig, float3 *dir, float3 *point, float t);
float3	ParaboloidNormal(global struct item *item, float3 *orig, float3 *dir, float3 *point, float t);
float3	BoxNormal(global struct item *item, float3 *point);

void	swap(float *a, float *b);
float	IsOutside(float3 direct, float3 normal);
float3	GetReflectVec(float3 direct, float3 normal);
float3	GetRefractVec(float3 direct, float3 normal, float etat);
float	FresnelRatio(float3 direct, float3 normal, float etat);
uint	RGBtoUint(float3 rgb);
bool	SolveQuadratic(float a, float b, float c, float *t1, float *t2);
float3	GetPointOnHemisphere(float k, float alpha);
void	GetDistributionOnHemisphere(float3 *dist, int circleNumber, int circlePartNumber);
#endif
