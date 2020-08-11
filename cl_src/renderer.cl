#include "clheader.clh"

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
		} else if (RT_Data->item[i].type == PARABOLOID) {
			t = ParaboloidIntersect(RT_Data->item + i, orig, dir);
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
		if (RT_Data->item[i].refractRatio == 0.f && RT_Data->item[i].type == PLANE) {
			t = PlaneIntersect(RT_Data->item + i, orig, dir);
		} else if (RT_Data->item[i].refractRatio == 0.f && RT_Data->item[i].type == SPHERE) {
			t = SphereIntersect(RT_Data->item + i, orig, dir);
		} else if (RT_Data->item[i].refractRatio == 0.f && RT_Data->item[i].type == CYLINDER) {
			t = CylinderIntersect(RT_Data->item + i, orig, dir);
		} else if (RT_Data->item[i].refractRatio == 0.f && RT_Data->item[i].type == CONE) {
			t = ConeIntersect(RT_Data->item + i, orig, dir);
		} else if (RT_Data->item[i].refractRatio == 0.f && RT_Data->item[i].type == PARABOLOID) {
			t = ParaboloidIntersect(RT_Data->item + i, orig, dir);
		} else {
			t = INFINITY;
		}
		if (t > 0.f && t <= 1.f) {
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
	} else if (item->type == PARABOLOID) {
		return ParaboloidNormal(item, orig, dir, point, t);
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
	point = point + normal * (float3)(1e-2);

	for (int i = 0; i != RT_Data->illuNumber; i += 1) {
/* Point light */
		if (RT_Data->illu[i].type == POINT
			|| (RT_Data->illu[i].type == SPHERE
				&& ((RT_Data->flags & FAST_RENDER) || !(RT_Data->flags & SOFT_SHADOWS)))) {
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
		if (RT_Data->illu[i].type == SPHERE
			&& !(RT_Data->flags & FAST_RENDER) && (RT_Data->flags & SOFT_SHADOWS)) {
			float3 basis[3] = {
				normalize(point - RT_Data->illu[i].center),
				(float3){0, 0, 0},
				(float3){0, 0, 0}
			};
			BuildBasis(basis);
			float r0 = 0, r1 = 0;
			float k = dot(GetReflectVec(basis[0], normal), -normalize(*dir));
			for (uint j = 0; j != RT_Data->soft_shadow_buffer_size; j += 1) {
				float3 illuVec = DecompByBasis(
					RT_Data->soft_shadow_buffer[j].zxy * RT_Data->illu[i].radius,
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
						if (dot(GetReflectVec(-illuVec, normal), -normalize(*dir)) > 1e-4)
							r1 += RT_Data->illu[i].k * pow(k, RT_Data->item[itemIndex].shineRatio);
					}
				}
			}
			diff += r0 / RT_Data->soft_shadow_buffer_size;
			shine += r1;
		}
	}
/* Global illumination */
	if (!(RT_Data->flags & FAST_RENDER) && (RT_Data->flags & AMBIENT)) {
		float k = 0.75f;
		float count = 0;
		float3 basis[3] = {
			normal,
			(float3){0, 0, 0},
			(float3){0, 0, 0}
		};
		BuildBasis(basis);
		for (uint i = 0; i != RT_Data->ambient_occlusion_buffer_size; i += 1) {
			float3 illuVec = DecompByBasis(RT_Data->ambient_occlusion_buffer[i].zxy, (float3){0, 0, 0}, basis);
			if (NearestItem(RT_Data, &point, &illuVec, NULL) != INFINITY) {
				continue;
			}
			count += 1.f;
		}
		diff += k * (count / (float)RT_Data->ambient_occlusion_buffer_size);
	}

	if (true) {
		return return_color_item(RT_Data->item + itemIndex, RT_Data->texture, normal, point).yzw
						* clamp(0.f, 1.f, diff) + (float3){255, 255, 255} * clamp(0.f, 1.f, shine);
	} else {
		return RT_Data->item[itemIndex].color
						* clamp(0.f, 1.f, diff) + (float3){255, 255, 255} * clamp(0.f, 1.f, shine);
	}
}

float3 Cast(struct RT_Data *RT_Data, float3 *orig,
	float3 *dir, struct node *node, global struct item **obj) {
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

float3	CastRay(struct RT_Data *RT_Data, float3 *orig,
	float3 *dir, struct node *node, global struct item **obj) {

	if (!(RT_Data->flags & FAST_RENDER)
		&& (RT_Data->flags & ALIASING)) {
		float3 axis;
		if (dir->x == 0) {
			axis = (float3){1, 0, 0};
		} else if (dir->z == 0) {
			axis = (float3){0, 0, 1};
		} else {
			axis = normalize((float3){dir->x, 0, -pow(dir->x, 2.f) / dir->z});
		}
		float3 color = (float3){0, 0, 0};
		color += Cast(RT_Data, orig, dir, node, obj);
		float3 ray[4] = {RotationAroundVector(axis, *dir, 0.00064f, 0)};
		for (int i = 1; i != 4; i += 1) {
			ray[i] = RotationAroundVector(*dir, ray[0], (M_PI / 4) * i, 0);
		}
		for (int i = 0; i != 4; i += 1) {
			color += Cast(RT_Data, orig, &ray[i], NULL, NULL);
		}
		return color / 5.f;
	} else {
		return Cast(RT_Data, orig, dir, node, obj);
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

kernel void	Renderer(global int *pixel, global struct item *illu,
	global struct item *item, global camera *cam, global float3 *soft_shadow_buffer,
		global float3 *ambient_occlusion_buffer, global size_t *active_item_address, global uint *textr, struct opt opt) {

	int id = get_global_id(0);
	float3 origin = cam->reper[0], direct = CalcDirect(id, &opt);

	{
		float3 basis[3] = {
			cam->reper[1],
			cam->reper[2],
			cam->reper[3]
		};
		direct = DecompByBasis(direct, (float3){0, 0, 0}, basis);
	}

	struct	RT_Data RT_Data = {opt.illu_c, opt.item_c, illu, item, textr,
		soft_shadow_buffer, opt.soft_shadow_buffer_size, ambient_occlusion_buffer, opt.ambient_occlusion_buffer_size, opt.flags};

	if (RT_Data.flags & RECURSION)
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
