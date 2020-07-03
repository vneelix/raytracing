#include "clheader.h"

/*Interface*/
float	NearestItem(struct RT_Data *RT_Data, float3 *orig, float3 *dir, int *itemIndex);
float	NearestIllu(struct RT_Data *RT_Data, float3 *orig, float3 *dir, int *itemIndex);
float3	GetNormal(global struct item *item, float3 *orig, float3 *dir, float3 *point, float t);
float3	DiffLighting(struct RT_Data *RT_Data, float3 *orig, float3 *dir, float t, int itemIndex);
float3	CastRay(struct RT_Data *RT_Data, float3 *orig, float3 *dir);
float3	CalcDirect(int id, struct opt *opt);
void	GenerateTree(struct RT_Data *RT_Data, struct tree *tree, struct node *firstNode);
void	CreateNode(struct node *node, global struct item *item, float3 *orig, float3 *dir, float3 *normal);

float	NearestItem(struct RT_Data *RT_Data, float3 *orig, float3 *dir, int *itemIndex) {
	float t, tmin = INFINITY;
	for (int i = 0; i != RT_Data->itemNumber; i += 1) {
		if (RT_Data->item[i].type == PLANE) {
			t = PlaneIntersect(RT_Data->item + i, orig, dir);
		} else if (RT_Data->item[i].type == SPHERE) {
			t = SphereIntersect(RT_Data->item + i, orig, dir);
		} else if (RT_Data->item[i].type == CYLINDER) {
			t = CylinderIntersect(RT_Data->item + i, orig, dir);
		} else if (RT_Data->item[i].type == CONE) {
			//t = ConeIntersect(RT_Data->item + i, orig, dir);
		} else if (RT_Data->item[i].type == ELLIPSOID) {
			//t = EllipsoidIntersect(RT_Data->item + i, orig, dir);
		} else if (RT_Data->item[i].type == PARABOLOID) {
			//t = ParaboloidIntersect(RT_Data->item + i, orig, dir);
		} else if (RT_Data->item[i].type == BOX) {
			//t = BoxIntersect(RT_Data->item + i, orig, dir);
		} else {
			t = INFINITY;
		}
		if (t >= 0.f && tmin > t) {
			tmin = t;
			if (itemIndex) {
				*itemIndex = i;
			}
		}
	}
	return (tmin);
	return 0;
}

float	NearestIllu(struct RT_Data *RT_Data, float3 *orig, float3 *dir, int *itemIndex) {
	float t, tmin = INFINITY;
	for (int i = 0; i != RT_Data->illuNumber; i += 1) {
		if (RT_Data->illu[i].type == SPHERE) {
			t = SphereIntersect(RT_Data->illu + i, orig, dir);
		} else {
			t = INFINITY;
		}
		if (t >= 0.f && tmin > t) {
			tmin = t;
			if (itemIndex) {
				*itemIndex = i;
			}
		}
	}
	return (tmin);
}

float3	GetNormal(global struct item *item, float3 *orig, float3 *dir, float3 *point, float t) {
	if (item->type == PLANE) {
		return item->normal;
	} else if (item->type == SPHERE) {
		return normalize(*point - item->center);
	} else if (item->type == CYLINDER) {
		return CylinderNormal(item, orig, dir, point, t);
	} else if (item->type == CONE) {
		//return ConeNormal(item, orig, dir, point, t);
	}	else if (item->type == ELLIPSOID) {
		//return EllipsoidNormal(item, orig, dir, point, t);
	} else if (item->type == PARABOLOID) {
		//return ParaboloidNormal(item, orig, dir, point, t);
	} else if (item->type == BOX) {
		//return BoxNormal(item, point);
	}
	return (float3){0, 0, 0};
}

float3	DiffLighting(struct RT_Data *RT_Data, float3 *orig, float3 *dir, float t, int itemIndex) {
	float	diff = 0.08, shine = 0;
	float3	point = *orig + *dir * t;
	float3	normal = GetNormal(RT_Data->item + itemIndex, orig, dir, &point, t);

	normal *= IsOutside(*dir, normal);
	if (RT_Data->node != NULL) {
		CreateNode(RT_Data->node, RT_Data->item + itemIndex, &point, dir, &normal);
	}
	point = point + normal * (float3)(1e-3);
	for (int i = 0; i != RT_Data->illuNumber; i += 1) {
/* Point light */
		if (RT_Data->illu[i].type == POINT)	{
			float3	illuVec = RT_Data->illu[i].center - point;
			float	tIllu = NearestIllu(RT_Data, &point, &illuVec, NULL);
			float	tItem = NearestItem(RT_Data, &point, &illuVec, NULL);
			if ((tItem >= 0.f && tItem <= 1.f) || (tIllu >= 0.f && tIllu <= 1.f)) {
				continue;
			}
			float	r0 = dot(normalize(illuVec), normal);
			if (r0 > 1e-4) {
				diff += RT_Data->illu[i].k * r0;
				if (RT_Data->item[itemIndex].shineRatio) {
					float	r1 = dot(GetReflectVec(-normalize(illuVec), normal), -normalize(*dir));
					if (r1 > 1e-4) {
						shine += RT_Data->illu[i].k * pow(r1, RT_Data->item[itemIndex].shineRatio);
					}
				}
			}
		}
/* Spheric light */
		if (RT_Data->illu[i].type == SPHERE) {
			float3 basis[3] = {
				normalize(point - RT_Data->illu[i].center),
				{0, 0, 0},
				{0, 0, 0}
			};
			BuildBasis(basis);
			float r0 = 0, r1 = 0;
			for (int j = 0; j != RT_Data->distributionSize; j += 1) {
				float3 illuVec = DecompByBasis(
					RT_Data->distribution[j].zxy * RT_Data->illu[i].radius,
					RT_Data->illu[i].center,
					basis
				) - point;
				float tIllu = INFINITY, tItem = NearestItem(RT_Data, &point, &illuVec, NULL);
				if ((tItem >= 0.f && tItem <= 1.f) || (tIllu >= 0.f && tIllu <= 1.f)) {
					continue;
				}
				float	a = dot(normalize(illuVec), normal);
				if (a > 1e-4) {
					r0 += RT_Data->illu[i].k * a;
					if (RT_Data->item[itemIndex].shineRatio) {
						float	b = dot(GetReflectVec(-normalize(illuVec), normal), -normalize(*dir));
						if (b > 1e-4) {
							r1 += RT_Data->illu[i].k * pow(b, RT_Data->item[itemIndex].shineRatio);
						}
					}
				}
			}
			shine += r1;
			diff += r0 / RT_Data->distributionSize;
		}
	}
/* Global illumination */
		{
			float k = 0.8f;
			float3 basis[3] = {
				normal,
				(float3){0, 0, 0},
				(float3){0, 0, 0}
			};
			BuildBasis(basis);
			float r0 = 0;
			for (int i = 0; i != RT_Data->distributionSize; i += 1) {
				float3 illuVec = DecompByBasis(RT_Data->distribution[i].zxy, (float3){0, 0, 0}, basis);
				if ((NearestIllu(RT_Data, &point, &illuVec, NULL) != INFINITY)
					|| (NearestItem(RT_Data, &point, &illuVec, NULL) != INFINITY)) {
					continue;
				}
				float	a = dot(normalize(illuVec), normal);
				if (a > 1e-4) {
					r0 += k * a;
				}
			}
			diff += r0 / RT_Data->distributionSize;
		}
	return RT_Data->item[itemIndex].color * clamp(0.f, 1.f, diff) + (float3){255, 255, 255} * clamp(0.f, 1.f, shine);
}

float3	CastRay(struct RT_Data *RT_Data, float3 *orig, float3 *dir) {
	float	tIllu, tItem;
	int		itemIndex, illuIndex;

	tIllu = NearestIllu(RT_Data, orig, dir, &illuIndex);
	tItem = NearestItem(RT_Data, orig, dir, &itemIndex);
	if (tItem != INFINITY && (tItem < tIllu)) {
		return DiffLighting(RT_Data, orig, dir, tItem, itemIndex);
	} else if (tIllu != INFINITY && (tIllu < tItem)) {
		return (float3){255, 255, 255} * RT_Data->illu[illuIndex].k;
	} else {
		return (float3){255, 255, 255};
	}
}

float3	CalcDirect(int id, struct opt *opt) {
	float3 origin = (float3){
		(id % opt->w - opt->w * 0.5f) / opt->w,
		((id - id % opt->w) / opt->w - opt->h * 0.5f) / opt->w,
		1
	};
	return normalize(Rotation(origin, opt->spin_x, opt->spin_y, 0, 0));
}

/* void	create_set(__global struct item *illu, __global struct item *item,
			struct opt *opt, float3 *color, struct state *ptr, __global struct item **obj, float *arr)
{
	int f = 0;
	int	num = 2;
	int	obj_id = -1;
	int	color_id = 0;
	struct state state[STE * 2];

	float out;
	state[0] = *ptr;
	for (int i = 0; i != DPH; i += 1)
	{
		for (int j = 0, k = 0; j != num; j += 2, k += 1)
		{
			if (obj[obj_id] != NULL)
				out = outside(state[f + k].direct, state[f + k].normal);
			if (i != (DPH - 1) && obj[obj_id] != NULL && obj[obj_id]->attr.refract != 0.f)
				arr[color_id] = fresnel(state[f + k].direct, state[f + k].normal, obj[obj_id]->attr.refract);
			if (obj[obj_id] != NULL && (obj[obj_id]->attr.reflect != 0.f || obj[obj_id]->attr.refract != 0.f))
				color[color_id] = traceray(illu, item, opt, state[f + k].point + state[f + k].normal * 0.001f * out, get_reflect_vec(
					state[f + k].direct, state[f + k].normal), (i != (DPH - 1) ? state + (f == 0 ? STE : 0) + j : NULL), (i != (DPH - 1) ? obj + color_id : NULL));
			if (obj[obj_id] != NULL && obj[obj_id]->attr.refract != 0.f)
				color[color_id + 1] = traceray(illu, item, opt, state[f + k].point - state[f + k].normal * 0.001f * out, get_refract_vec(state[f + k].direct, state[f 
					+ k].normal, obj[obj_id]->attr.refract), (i != (DPH - 1) ? state + (f == 0 ? STE : 0) + j + 1 : NULL), (i != (DPH - 1) ? obj + color_id + 1 : NULL));
			color_id += 2;
			obj_id += 1;
		}
		f = (f == 0 ? STE : 0);
		num <<= 1;
	}
}

void	collapse_set(float3 *color, __global struct item **obj, float *k)
{
	if (DPH != 1)
	{
		int active, next;
		for (int i = (DPH - 1); i != 0; i -= 1)
		{
			for (int j = 0, active = (1 << i) - 2, next = (1 << (i + 1)) - 2; active != next; j += 2, active += 1)
			{
				if (obj[active] != NULL && obj[active]->attr.reflect != 0.f)
					color[active] = color[active] * (1.f - obj[active]->attr.reflect) + color[next + j] * obj[active]->attr.reflect;
				else if (obj[active] != NULL && obj[active]->attr.refract != 0.f)
					color[active] = color[next + j] * k[active] + color[next + j + 1] * (1.f - k[active]);
			}
		}
	}
	if (obj[-1] != NULL && obj[-1]->attr.reflect != 0.f)
		color[-1] = color[-1] * (1.f - obj[-1]->attr.reflect) + color[0] * obj[-1]->attr.reflect;
	else if (obj[-1] != NULL && obj[-1]->attr.refract != 0.f)
		color[-1] = color[0] * k[-1] + color[1] * (1.f - k[-1]);
} */
void	CreateNode(struct node *node, global struct item *item, float3 *orig, float3 *dir, float3 *normal) {
	node->orig = *orig;
	node->reflectVec3f = GetReflectVec(*dir, *normal);
	if (item->refractRatio != 0.f) {
		node->fresnelRatio = FresnelRatio(*dir, *normal, item->refractRatio);
		node->refractVec3f = GetRefractVec(*dir, *normal, item->refractRatio);
	}
	node->item = item;
}

void	GenerateTree(struct RT_Data *RT_Data, struct tree *tree) {
	//struct node node[1 << DPH];
	int	nodeNumber = 2;
	for (int i = 0; i != DPH; i += 1) {
		for (int j = 0; j != nodeNumber; j += 1) {
			
		}
	}
}

kernel void	main_func(global int *pixel, global struct item *illu, global struct
	item *item, struct opt opt, global float3 *distribution, int distributionSize) {
	int id = get_global_id(0);
	float3 origin = opt.center, direct = CalcDirect(id, &opt);

	struct	RT_Data RT_Data = {opt.illu_c, opt.item_c, illu, item, distribution, distributionSize};

	{
		struct tree tree;
		GenerateTree(&RT_Data, &tree, NULL);
	}

	pixel[id] = RGBtoUint(CastRay(&RT_Data, &origin, &direct));
}
