#include "affine_transform.h"

float		Cofactor(float3 *m, int a, int b) {
	int k = 0;
	float arr[4];
	for (int i = 0; i != 3; i += 1) {
		if (i == a) {continue;}
		for (int j = 0; j != 3; j += 1) {
			if (j == b) {continue;}
			arr[k++] = m[i][j];
		}
	}
	return (arr[0] * arr[3] - arr[1] * arr[2]) * pow(-1.f, (float)(a + b));
}

float3	objToBasis(float3 obj, float3 orig, float3 *basis, int pointFlag) {
	float3 tMatrix[3] = {
		{Cofactor(basis, 0, 0), Cofactor(basis, 0, 1), Cofactor(basis, 0, 2)},
		{Cofactor(basis, 1, 0), Cofactor(basis, 1, 1), Cofactor(basis, 1, 2)},
		{Cofactor(basis, 2, 0), Cofactor(basis, 2, 1), Cofactor(basis, 2, 2)}
	};
	float3 newObj = {
		tMatrix[0].x * obj.x + tMatrix[0].y * obj.y + tMatrix[0].z * obj.z,
		tMatrix[1].x * obj.x + tMatrix[1].y * obj.y + tMatrix[1].z * obj.z,
		tMatrix[2].x * obj.x + tMatrix[2].y * obj.y + tMatrix[2].z * obj.z
	};
	if (pointFlag) {
		newObj.x += orig.x;
		newObj.y += orig.y;
		newObj.z += orig.z;
	}
	return newObj;
}

void		CreateBasis(float3 *basis) {
	if (basis[0].x == 0) {
		basis[1] = (float3){1, 0, 0};
	} else if (basis[0].y == 0) {
		basis[1] = (float3){0, 1, 0};
	} else if (basis[0].z == 0) {
		basis[1] = (float3){0, 0, 1};
	} else {
		basis[1] = normalize((float3){1, 1, (-basis[0].x - basis[0].y) / basis[0].z});
	}
	basis[2] = normalize(cross(basis[0], basis[1]));
}

float3	DecompByBasis(float3 obj, float3 orig, float3 *basis) {
	return (float3){
		basis[0] * obj.x +
		basis[1] * obj.y +
		basis[2] * obj.z
	} + orig;
}
