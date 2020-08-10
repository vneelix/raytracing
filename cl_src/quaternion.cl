#include "quaternion.clh"

float4	HamiltonProduct(float4 q1, float4 q2) {
	return (float4){
		q1.x * q2.w + q1.w * q2.x + q1.y * q2.z - q1.z * q2.y,
		q1.y * q2.w + q1.w * q2.y + q1.z * q2.x - q1.x * q2.z,
		q1.z * q2.w + q1.w * q2.z + q1.x * q2.y - q1.y * q2.x,
		q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
	};
}

float4	ConjugatedQuat(float4 q) {
	return (float4){q.xyz * (-1), q.w};
}

float4	QuaternionNorm(float4 q) {
	return (
		q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w
	);
}

float4	ReverseQuat(float4 q) {
	return ConjugatedQuat(q) / QuaternionNorm(q);
}

float4	ToQuaternion(float x, float y, float z) {
	float cos_x = cos(x * 0.5f);
	float cos_y = cos(y * 0.5f);
	float cos_z = cos(z * 0.5f);

	float sin_x = sin(x * 0.5f);
	float sin_y = sin(y * 0.5f);
	float sin_z = sin(z * 0.5f);

	return (float4){
		sin_x * cos_y * cos_z - cos_x * sin_y * sin_z,
		cos_x * sin_y * cos_z + sin_x * cos_y * sin_z,
		cos_x * cos_y * sin_z - sin_x * sin_y * cos_z,
		cos_x * cos_y * cos_z + sin_x * sin_y * sin_z
	};
}

float3	Rotation(float3 object, float x, float y, float z, int reverse) {
	float4 quat = (float4){object, 0};
	float4 rotationQuat = reverse ? ReverseQuat(ToQuaternion(x, y, z)) : ToQuaternion(x, y, z);
	return HamiltonProduct(HamiltonProduct(rotationQuat, quat), ReverseQuat(rotationQuat)).xyz;
}

float3	RotationAround(float3 around, float3 object, float x, float y, float z, int reverse) {
	float4 quat = (float4){object - around, 0};
	float4 rotationQuat = reverse ? ReverseQuat(ToQuaternion(x, y, z)) : ToQuaternion(x, y, z);
	return HamiltonProduct(HamiltonProduct(rotationQuat, quat), ReverseQuat(rotationQuat)).xyz + around;
}

float3	RotationAroundVector(float3 vec3fAxis, float3 object, float angle, int reverse) {
	float4 quat = (float4){object, 0};
	float4 rotationQuat = (float4){vec3fAxis * sin(angle * 0.5f), cos(angle * 0.5f)};
	rotationQuat = reverse ? ReverseQuat(rotationQuat) : rotationQuat;
	return HamiltonProduct(HamiltonProduct(rotationQuat, quat), ReverseQuat(rotationQuat)).xyz;
}
