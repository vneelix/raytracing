#include "clheader.h"

void	swap(float *a, float *b)
{
	float buff = *a;
	*a = *b;
	*b = buff;
}

float	IsOutside(float3 direct, float3 normal) {
	if (dot(normalize(direct), normal) > 0.f) {
		return -1;
	}
	return 1;
}

float3	GetReflectVec(float3 direct, float3 normal)
{
	return normalize(
		2 * normal * dot(-direct, normal) + direct
	);
}

float3	GetRefractVec(float3 direct, float3 normal, float etat)
{
	float cosi, eta, etai, k;

	etai = 1;
	cosi = dot(normalize(direct), normalize(normal));
	if (cosi < 0.001)
		cosi = -cosi;
	else {
		normal = -normal;
		swap(&etai, &etat);
	}
	eta = etai/ etat;
	k = 1 - eta * eta * (1 - cosi * cosi); 
    return (k < 0 ? 0 : eta * direct + (eta * cosi - sqrt(k)) * normal);
}

float	fresnel(float3 direct, float3 normal, float etat)
{
	float sint, cosi, etai;
	etai = 1;
	cosi = dot(normalize(direct), normalize(normal));
	if (cosi > 1e-3)
		swap(&etai, &etat);
	sint = etai / etat * sqrt(max(0.f, 1.f - cosi * cosi));
	if (sint >= 1.f) {
		return 1;
	} else {
		float cost, Rs, Rp;
		cost = sqrt(max(0.f, 1.f - sint * sint));
		cosi = fabs(cosi);
		Rs = (etat * cosi - etai * cost) / (etat * cosi + etai * cost);
		Rp = (etai * cosi - etat * cost) / (etai * cosi + etat * cost);
		return (Rs * Rs + Rp * Rp) / 2.f;
	}
}

uint	RGBtoUint(float3 rgb) {
	uint	color = 0;
	rgb.x = clamp(0.f, 255.f, rgb.x);
	rgb.y = clamp(0.f, 255.f, rgb.y);
	rgb.z = clamp(0.f, 255.f, rgb.z);
	color |= (uint)(rgb.z);
	color |= (uint)(rgb.y) << 8;
	color |= (uint)(rgb.x) << 16;
	return (color);
}

bool	SolveQuadratic(float a, float b, float c, float *t1, float *t2) {
	float d = b * b - 4 * a * c;
	if (d < 0) {
		return (false);
	} else {
		d = sqrt(d);
		*t1 = (-b + d) / (2 * a);
		*t2 = (-b - d) / (2 * a);
		if (*t1 > *t2) {
			swap(t1, t2);
		}
		return (true);
	}
}

float3	GetPointOnHemisphere(float k, float alpha) {
	k = clamp(0.0f, 1.f, k);
	alpha = clamp(0.0f, (float)(2 * M_PI), alpha);
	return (float3){
		sqrt(k) * cos(alpha),
		sqrt(k) * sin(alpha),
		sqrt(1.f - k)
	};
}

void		GetDistributionOnHemisphere(float3 *dist, int circleNumber, int circlePartNumber) {
	float delta_k = 1.f / circleNumber;
	float	delta_alpha = (2 * M_PI) / circlePartNumber;
	dist += 1;
	for (int i = 0; i != circleNumber; i += 1) {
		for (int j = 0; j != circlePartNumber; j += 1) {
			dist[i + circleNumber * j] = GetPointOnHemisphere(delta_k * (i + 1), delta_alpha * (j + 1));
		}
	}
	dist -= 1;
	dist[0] = GetPointOnHemisphere(0, 0);
}

void		PrintVec3f(float3 vec3f) {
	printf("%f,%f,%f\n", vec3f.x, vec3f.y, vec3f.z);
}
