#include "clheader.h"

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
			t = ConeIntersect(RT_Data->item + i, orig, dir);
		} else if (RT_Data->item[i].type == ELLIPSOID) {
			t = EllipsoidIntersect(RT_Data->item + i, orig, dir);
		} else if (RT_Data->item[i].type == PARABOLOID) {
			//t = ParaboloidIntersect(RT_Data->item + i, orig, dir);
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

bool	Shadow(struct RT_Data *RT_Data, float3 *orig, float3 *dir) {
	float t;
	for (int i = 0; i != RT_Data->itemNumber; i += 1) {
		if (!RT_Data->item[i].refractRatio && RT_Data->item[i].type == PLANE) {
			t = PlaneIntersect(RT_Data->item + i, orig, dir);
		} else if (!RT_Data->item[i].refractRatio && RT_Data->item[i].type == SPHERE) {
			t = SphereIntersect(RT_Data->item + i, orig, dir);
		} else if (!RT_Data->item[i].refractRatio && RT_Data->item[i].type == CYLINDER) {
			t = CylinderIntersect(RT_Data->item + i, orig, dir);
		} else if (!RT_Data->item[i].refractRatio && RT_Data->item[i].type == CONE) {
			t = ConeIntersect(RT_Data->item + i, orig, dir);
		} else if (!RT_Data->item[i].refractRatio && RT_Data->item[i].type == ELLIPSOID) {
			t = EllipsoidIntersect(RT_Data->item + i, orig, dir);
		} else if (!RT_Data->item[i].refractRatio && RT_Data->item[i].type == PARABOLOID) {
			//t = ParaboloidIntersect(RT_Data->item + i, orig, dir);
		} else {
			t = INFINITY;
		}
		if (t >= 0.f && t <= 1.f) {
			return true;
		}
	}
	return false;
}

float3	GetNormal(global struct item *item, float3 *orig, float3 *dir, float3 *point, float t) {
	if (item->type == PLANE) {
		return item->normal;
	} else if (item->type == SPHERE) {
		return normalize(*point - item->center);
	} else if (item->type == CYLINDER) {
		return CylinderNormal(item, orig, dir, point, t);
	} else if (item->type == CONE) {
		return ConeNormal(item, orig, dir, point, t);
	} else if (item->type == ELLIPSOID) {
		return EllipsoidNormal(item, orig, dir, point, t);
	} else if (item->type == PARABOLOID) {
		//return ParaboloidNormal(item, orig, dir, point, t);
	}
	return (float3){0, 0, 0};
}

float3	DiffLighting(struct RT_Data *RT_Data, float3 *orig,
		float3 *dir,float t, int itemIndex, struct node *node) {
	float	diff = 0.08, shine = 0;
	float3	point = *orig + *dir * t;
	float3	normal = GetNormal(RT_Data->item + itemIndex, orig, dir, &point, t);

	normal *= IsOutside(*dir, normal);
	CreateNode(node, &point, dir, &normal);
	point = point + normal * (float3)(1e-3);
	for (int i = 0; i != RT_Data->illuNumber; i += 1) {
/* Point light */
		if (RT_Data->illu[i].type == POINT)	{
			float3	illuVec = RT_Data->illu[i].center - point;
			if (Shadow(RT_Data, &point, &illuVec)) {
				continue;
			}
			float r0 = dot(normalize(illuVec), normal);
			if (r0 > 1e-4) {
				diff += RT_Data->illu[i].k * r0;
				if (RT_Data->item[itemIndex].shineRatio) {
					float r1 = dot(GetReflectVec(-normalize(illuVec), normal), -normalize(*dir));
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
				if (Shadow(RT_Data, &point, &illuVec)) {
					continue;
				}
				float a = dot(normalize(illuVec), normal);
				if (a > 1e-4) {
					r0 += RT_Data->illu[i].k * a;
					if (RT_Data->item[itemIndex].shineRatio) {
						float b = dot(GetReflectVec(-normalize(illuVec), normal), -normalize(*dir));
						if (b > 1e-4) {
							r1 += RT_Data->illu[i].k * pow(b, RT_Data->item[itemIndex].shineRatio);
						}
					}
				}
			}
			diff += r0 / RT_Data->distributionSize;
			shine += r1;
		}
	}
/* Global illumination */
		if (false)
		{
			float k = 0.55f;
			float count = 0;
			float3 basis[3] = {
				normal,
				(float3){0, 0, 0},
				(float3){0, 0, 0}
			};
			BuildBasis(basis);
			for (int i = 0; i != RT_Data->distributionSize; i += 1) {
				float3 illuVec = DecompByBasis(RT_Data->distribution[i].zxy, (float3){0, 0, 0}, basis);
				if (NearestItem(RT_Data, &point, &illuVec, NULL) != INFINITY) {
					continue;
				}
				count += 1.f;
			}
			diff += k * (count / (float)RT_Data->distributionSize);
		}
	return RT_Data->item[itemIndex].color * clamp(0.f, 1.f, diff) + (float3){255, 255, 255} * clamp(0.f, 1.f, shine);
}

float3	CastRay(struct RT_Data *RT_Data, float3 *orig, float3 *dir, struct node *node, global struct item **obj) {
	float	tIllu, tItem;
	int		itemIndex, illuIndex;

	tIllu = NearestIllu(RT_Data, orig, dir, &illuIndex);
	tItem = NearestItem(RT_Data, orig, dir, &itemIndex);
	if (tItem != INFINITY && (tItem < tIllu)) {
		if (obj != NULL) {*obj = RT_Data->item + itemIndex;}
		return DiffLighting(RT_Data, orig, dir, tItem, itemIndex, node);
	} else if (tIllu != INFINITY && (tIllu < tItem)) {
		if (obj != NULL) {*obj = NULL;}
		return (float3){255, 255, 255};
	} else {
		if (obj != NULL) {*obj = NULL;}
		return (float3){255, 255, 255};
	}
}

float3	CalcDirect(int id, struct opt *opt) {
	float3 dir = (float3){
		(id % opt->w - opt->w * 0.5f) / opt->w,
		((id - id % opt->w) / opt->w - opt->h * 0.5f) / opt->w,
		1.f
	};
	return normalize(dir);
}

kernel void	main_func(global int *pixel, global struct item *illu,
	global struct item *item, global camera *cam, global float3 *distribution,
		int distributionSize, global size_t *active_item_address, struct opt opt) {

	int id = get_global_id(0);
	float3 origin = cam->reper[0], direct = CalcDirect(id, &opt);

	{
		float3	basis[3] = {
			cam->reper[1],
			cam->reper[2],
			cam->reper[3]
		};
		direct = DecompByBasis(direct, (float3){0, 0, 0}, basis);
	}

	/*
	if (*active_item_address == 0) {
		direct = Rotation(direct, opt.x, opt.y, 0, 0);
	} else {
		global struct item *active_item = (global struct item*)(*active_item_address);
		origin = RotationAround(active_item->center, cam_reper[0], opt.x, opt.y, 0, 0);
		float3	basis[3];
		{
			basis[2] = normalize(active_item->center - origin);
			if (basis[2].x == 0) {
				basis[0] = (float3){1, 0, 0};
			} else if (basis[2].z == 0) {
				basis[0] = (float3){0, 0, 1};
			} else {
				basis[0] = normalize((float3){basis[2].x, 0, -pow(basis[2].x, 2.f) / basis[2].z});
			}
			basis[1] = normalize(cross(basis[0], basis[2]));

			basis[1] *= dot(basis[1], (float3){0, 1, 0}) < 0.f ? -1.f : 1.f;
			if (dot(basis[2], (float3){0, 0, 1}) > 0.f) {
				basis[0] *= dot(basis[0], (float3){1, 0, 0}) < 0.f ? -1.f : 1.f;
			} else {
				basis[0] *= dot(basis[0], (float3){1, 0, 0}) < 0.f ? 1.f : -1.f;
			}
		}
		direct = DecompByBasis(direct, (float3){0, 0, 0}, basis);
	}*/

	struct	RT_Data RT_Data = {opt.illu_c, opt.item_c, illu, item, distribution, distributionSize};

	if (false)
	{
		struct node root;
		float f[ITEM_NUMBER];
		float3 clr[COLOR_NUMBER];
		global struct item *obj[ITEM_NUMBER];
		clr[0] = CastRay(&RT_Data, &origin, &direct, &root, obj);
		if ((obj[0] != NULL) && (obj[0]->reflectRatio || obj[0]->refractRatio)) {
			GenerateTree(&RT_Data, &root, f, clr, obj);
			FoldTree(f, clr, obj);
		}
		pixel[id] = RGBtoUint(clr[0]);
	} else {
		pixel[id] = RGBtoUint(CastRay(&RT_Data, &origin, &direct, NULL, NULL));
	}
}
