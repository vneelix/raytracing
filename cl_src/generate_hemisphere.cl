#include "clheader.h"

kernel void genHemisphere(global float3 *pointArray, uint circleNumber, uint circlePartNumber) {
	size_t id = get_global_id(0);
	if (id == circleNumber * circlePartNumber) {
		pointArray[id] = GetPointOnHemisphere(0, 0);
	} else {
		float delta_k = 0.98f / circleNumber;
		float delta_alpha = (2 * M_PI) / circlePartNumber;
		int j = id % circlePartNumber;
		int i = (id - j) / circlePartNumber;
		pointArray[id] = GetPointOnHemisphere(delta_k * (i + 1), delta_alpha * (j + 1));
	}
}
