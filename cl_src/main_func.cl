#include "clheader.h"

/*Interface*/
float		NearestItem(struct RT_Data *RT_Data, float3 *orig, float3 *dir, int *itemIndex);
float		NearestIllu(struct RT_Data *RT_Data, float3 *orig, float3 *dir, int *itemIndex);
float3	GetNormal(global struct item *item, float3 *orig, float3 *dir, float3 *point, float t);
float3	DiffLighting(struct RT_Data *RT_Data, float3 *orig, float3 *dir, float t, int itemIndex);
float3	CastRay(struct RT_Data *RT_Data, float3 *orig, float3 *dir);
float3	CalcDirect(int id, struct opt *opt);

float	NearestItem(struct RT_Data *RT_Data, float3 *orig, float3 *dir, int *itemIndex) {
	float t, tmin = INFINITY;
	for (int i = 0; i != RT_Data->itemNumber; i += 1) {
		if (RT_Data->item[i].type == PLANE) {
			t = PlaneIntersect(RT_Data->item + i, orig, dir);
		} else if (RT_Data->item[i].type == SPHERE) {
			t = SphereIntersect(RT_Data->item + i, orig, dir);
		} else if (RT_Data->item[i].type == CYLINDER) {
			//t = CylinderIntersect(RT_Data->item + i, orig, dir);
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
		if (t > 1e-4 && tmin > t) {
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
		if (t > 1e-4 && tmin > t) {
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
		//return CylinderNormal(item, orig, dir, point, t);
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
	float		diff = 0, shine = 0;
	float3	point = *orig + *dir * t;
	float3	normal = GetNormal(RT_Data->item + itemIndex, orig, dir, &point, t);

	point += normal * (float3)(1e-3);

	/* Light sources */
	for (int i = 0; i != RT_Data->illuNumber; i += 1) {
		/* Point light */
		if (RT_Data->illu[i].type == POINT)	{
			float3 illuVec = RT_Data->illu[i].center - point;
			float tIllu = NearestIllu(RT_Data, &point, &illuVec, NULL);
			float tItem = NearestItem(RT_Data, &point, &illuVec, NULL);
			if ((tItem > 0.f && tItem < 1.f) || (tIllu > 0.f && tIllu < 1.f)) {
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
				if ((tItem > 0.f && tItem < 1.f) || (tIllu > 0.f && tIllu < 1.f)) {
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
			diff += r0 / RT_Data->distributionSize;
			shine += r1;
		}
	}
	/* Global illumination */
	{
		float k = 0.45;
		float3 basis[3] = {
			normal,
			(float3){0, 0, 0},
			(float3){0, 0, 0}
		};
		BuildBasis(basis);
		float r0 = 0, r1 = 0;
		for (int i = 0; i != RT_Data->distributionSize; i += 1) {
			float3 illuVec = DecompByBasis(RT_Data->distribution[i].zxy, (float3){0, 0, 0}, basis);
			if (dot(illuVec, (float3){0, -1, 0}) < 1e-4) {
				continue;
			}
			if ((NearestIllu(RT_Data, &point, &illuVec, NULL) != INFINITY)
				|| (NearestItem(RT_Data, &point, &illuVec, NULL) != INFINITY)) {
				continue;
			}
			float	a = dot(normalize(illuVec), normal);
			if (a > 1e-4) {
				r0 += k * a;
				if (RT_Data->item[itemIndex].shineRatio) {
					float	b = dot(GetReflectVec(-normalize(illuVec), normal), -normalize(*dir));
					if (b > 1e-4) {
						r1 += k * pow(b, RT_Data->item[itemIndex].shineRatio);
					}
				}
			}
		}
		diff += r0 / RT_Data->distributionSize;
		shine += r1;
	}
	return RT_Data->item[itemIndex].color * clamp(0.f, 1.f, diff) + (float3){255, 255, 255} * clamp(0.f, 1.f, shine);
}

float3	CastRay(struct RT_Data *RT_Data, float3 *orig, float3 *dir) {
	int	itemIndex, illuIndex;
	float tIllu = INFINITY, tItem;
	tIllu = NearestIllu(RT_Data, orig, dir, &illuIndex);
	tItem = NearestItem(RT_Data, orig, dir, &itemIndex);
	if (tItem < tIllu) {
		return DiffLighting(RT_Data, orig, dir, tItem, itemIndex);
	} else if (tIllu < tItem) {
		return (float3){255, 255, 255} * RT_Data->illu[illuIndex].k;
	} else {
		return (float3){117, 187, 253};
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

kernel void	main_func(global int *pixel, global struct item *illu, global struct
	 item *item, struct opt opt, global float3 *distribution, int distributionSize) {
	int id = get_global_id(0);
	float3 origin = opt.center, direct = CalcDirect(id, &opt);

	struct	RT_Data RT_Data = {opt.illu_c, opt.item_c, illu, item, distribution, distributionSize};

	pixel[id] = RGBtoUint(CastRay(&RT_Data, &origin, &direct));
}
