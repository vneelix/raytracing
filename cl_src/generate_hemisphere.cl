#include "./cl_inc/clheader.clh"

kernel void GenHemisphere(global float3 *pointArray, uint circleNumber, uint circlePartNumber) {
	size_t id = get_global_id(0);
	if (id == circleNumber * circlePartNumber) {
		pointArray[id] = GetPointOnHemisphere(0, 0);
	} else {
		float delta_k = 0.96f / circleNumber;
		float delta_alpha = (2 * M_PI) / circlePartNumber;
		int j = id % circlePartNumber;
		int i = (id - j) / circlePartNumber;
		pointArray[id] = GetPointOnHemisphere(delta_k * (i + 1), delta_alpha * (j + 1));
	}
}
