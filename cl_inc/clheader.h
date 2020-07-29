#ifndef CLHEADER_H
#define CLHEADER_H

#include "quaternion.h"
#include "affine_transform.h"

#define DPH 4
#define COLOR_NUMBER 2 * ((1 << DPH) - 1) + 1
#define ITEM_NUMBER 2 * ((1 << (DPH - 1)) - 1) + 1

#ifndef NULL
	#define NULL (void*)(0)
#endif
#ifndef M_PI
	#define	M_PI (float)3.14159265358979323846264338327950288
#endif

typedef struct	camera {
	float	x;
	float	y;
	float	z;
	float3	temp[4];
	float3	reper[4];
}				camera;

enum iType {
	POINT,
	PLANE,
	SPHERE,
	CYLINDER,
	CONE,
	ELLIPSOID,
	PARABOLOID,
};

struct RT_Data {
	int illuNumber;
	int itemNumber;
	global struct item *illu;
	global struct item *item;
	/* Monte Carlo data */
	global float3 *distribution;
	int distributionSize;
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

struct node
{
	float3	dir;
	float3	orig;
	float3	normal;
};

struct opt{
	int		w;
	int		h;
	int		illu_c;
	int		item_c;
	float3	center;
	/* Euler angles */
	float	x, y, z;
};

float	PlaneIntersect(global struct item *item, float3 *orig, float3 *dir);
float	SphereIntersect(global struct item *item, float3 *orig, float3 *dir);
float	CylinderIntersect(global struct item *item, float3 *orig, float3 *dir);
float	ConeIntersect(global struct item *item, float3 *orig, float3 *dir);
float	EllipsoidIntersect(global struct item *item, float3 *orig, float3 *dir);
float	ParaboloidIntersect(global struct item *item, float3 *orig, float3 *dir);
float3	CylinderNormal(global struct item *item, float3 *orig, float3 *dir, float3 *point, float t);
float3	ConeNormal(global struct item *item, float3 *orig, float3 *dir, float3 *point, float t);
float3	EllipsoidNormal(global struct item *item, float3 *orig, float3 *dir, float3 *point, float t);
float3	ParaboloidNormal(global struct item *item, float3 *orig, float3 *dir, float3 *point, float t);

float	NearestItem(struct RT_Data *RT_Data, float3 *orig, float3 *dir, int *itemIndex);
float	NearestIllu(struct RT_Data *RT_Data, float3 *orig, float3 *dir, int *itemIndex);
float3	GetNormal(global struct item *item, float3 *orig, float3 *dir, float3 *point, float t);
float3	DiffLighting(struct RT_Data *RT_Data, float3 *orig, float3 *dir, float t, int itemIndex, struct node *node);
float3	CastRay(struct RT_Data *RT_Data, float3 *orig, float3 *dir, struct node *node, global struct item **obj);
float3	CalcDirect(int id, struct opt *opt);
bool	Shadow(struct RT_Data *RT_Data, float3 *orig, float3 *dir);

void	CreateNode(struct node *node, float3 *orig, float3 *dir, float3 *normal);
void	GenerateTree(struct RT_Data *RT_Data, struct node *root, float *f, float3 *clr, global struct item **obj);
void	FoldTree(float *f, float3 *clr, global struct item **obj);

void	swap(float *a, float *b);
float	IsOutside(float3 direct, float3 normal);
float3	GetReflectVec(float3 direct, float3 normal);
float3	GetRefractVec(float3 direct, float3 normal, float etat);
float	FresnelRatio(float3 direct, float3 normal, float etat);
uint	RGBtoUint(float3 rgb);
bool	SolveQuadratic(float a, float b, float c, float *t1, float *t2);
float3	GetPointOnHemisphere(float r1, float r2);
void	GetDistributionOnHemisphere(float3 *dist, int circleNumber, int circlePartNumber);
#endif
