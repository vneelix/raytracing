#include "./cl_inc/clheader.clh"

void	CreateNode(struct node *node, float3 *orig, float3 *dir, float3 *normal) {
	if (!node) { return; }
	node->dir = *dir;
	node->orig = *orig;
	node->normal = *normal;
}

void	GenerateTree(struct RT_Data *RT_Data, struct node *root, float *f, float3 *clr, global struct item **obj) {
	int offset = 0;
	int	objIndex = 0;
	int pairIndex = 1;
	int	nodeNumber = 2;
	struct node node[1 << DPH];

	node[0] = *root;
	for (int i = 0; i != DPH; i += 1) {
		for (int j = 0, k = 0; j != nodeNumber; j += 2, k += 1) {
			int currNode = offset + k, nextNode = (offset ? 0 : 1 << (DPH - 1)) + j;
			float3 orig, reflectVec3f, refractVec3f;
			if ((obj[objIndex] != NULL) && ((obj[objIndex]->reflectRatio) || obj[objIndex]->refractIndex)) {
				orig = node[currNode].orig + node[currNode].normal * (float)1e-2;
				reflectVec3f = GetReflectVec(node[currNode].dir, node[currNode].normal);
				clr[pairIndex] = CastRay(RT_Data, &orig, &reflectVec3f, (i != DPH - 1) ? node + nextNode : NULL, (i != DPH - 1) ? obj + pairIndex : NULL);
			}
			if ((obj[objIndex] != NULL) && obj[objIndex]->refractIndex) {
				if (objIndex < ITEM_NUMBER) {
					f[objIndex] = FresnelRatio(node[currNode].dir, node[currNode].normal, obj[objIndex]->refractIndex);
				}
				orig = node[currNode].orig - node[currNode].normal * (float)1e-2;
				refractVec3f = GetRefractVec(node[currNode].dir, node[currNode].normal, obj[objIndex]->refractIndex);
				clr[pairIndex + 1] = CastRay(RT_Data, &orig, &refractVec3f, (i != DPH - 1) ? node + nextNode + 1 : NULL, (i != DPH - 1) ? obj + pairIndex + 1 : NULL);
			}
			pairIndex += 2;
			objIndex += 1;
		}
		offset = offset ? 0 : 1 << (DPH - 1);
		nodeNumber <<= 1;
	}
}

void	FoldTree(float *f, float3 *clr, global struct item **obj) {
	for (int i = DPH - 1; i != 0; i -= 1) {
		int curr = (1 << i) - 1;
		for (int j = 0; j != 1 << i; j += 1) {
			int next = (1 << (i + 1)) - 1 + 2 * j;
			if ((obj[curr] != NULL) && obj[curr]->reflectRatio) {
				clr[curr] = clr[curr] * (1.f - obj[curr]->reflectRatio) + clr[next] * obj[curr]->reflectRatio;
			}
			if ((obj[curr] != NULL) && obj[curr]->refractIndex) {
				clr[curr] = clr[curr] * (1.f - obj[curr]->refractRatio) + (clr[next] * f[curr] + clr[next + 1] * (1.f - f[curr])) * obj[curr]->refractRatio;
			}
			curr += 1;
		}
	}
	if ((obj[0] != NULL) && obj[0]->reflectRatio) {
		clr[0] = clr[0] * (1.f - obj[0]->reflectRatio) + clr[1] * obj[0]->reflectRatio;
	}
	if ((obj[0] != NULL) && obj[0]->refractIndex) {
		clr[0] = clr[0] * (1.f - obj[0]->refractRatio) + (clr[1] * f[0] + clr[2] * (1.f - f[0])) * obj[0]->refractRatio;
	}
}
