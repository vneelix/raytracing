#include "./cl_inc/clheader.clh"

kernel void	MoveOrigin(global camera *cam, global size_t *active_item_address, uint flags)
{
	global struct item *active_item;
	
	active_item = (global struct item*)*active_item_address;
	if (active_item != NULL) {
		float3 vec3f =
			active_item->center + cam->reper[3];
		vec3f = normalize((float3){vec3f.x, 0, vec3f.z}
					- (float3){active_item->center.x, 0, active_item->center.z});
		if (flags & (uint)1)
			active_item->center += vec3f;
		else if (flags & (uint)1 << 1)
			active_item->center -= vec3f;
		else if (flags & (uint)1 << 2)
			active_item->center -= cam->reper[1];
		else if (flags & (uint)1 << 3)
			active_item->center += cam->reper[1];
		else if (flags & (uint)1 << 4)
			active_item->center += (float3){0, -1, 0};
		else if (flags & (uint)1 << 5)
			active_item->center -= (float3){0, -1, 0};
	} else {
		if (flags & (uint)1)
			cam->reper[0] += cam->reper[3];
		else if (flags & (uint)1 << 1)
			cam->reper[0] -= cam->reper[3];
		else if (flags & (uint)1 << 2)
			cam->reper[0] -= cam->reper[1];
		else if (flags & (uint)1 << 3)
			cam->reper[0] += cam->reper[1];
		else if (flags & (uint)1 << 4)
			cam->reper[0] += (float3){0, -1, 0};
		else if (flags & (uint)1 << 5)
			cam->reper[0] -= (float3){0, -1, 0};
	}
}

kernel void	FindItem(global camera *cam, global struct item *item,
	global size_t *active_item_address, int x, int y, struct opt opt) {

	int		itemIndex = 0;
		float2 uv = ((float2){x, y} - (float2){opt.w, opt.h} * 0.5f) / opt.h;
		float3 origin = cam->reper[0], direct = normalize((float3){uv.x, uv.y, 1});
	struct	RT_Data RT_Data = {0, opt.item_c, NULL, item, NULL, 0};

	float3 basis[3] = {
		cam->reper[1],
		cam->reper[2],
		cam->reper[3]
	};
	direct = DecompByBasis(direct, (float3){0, 0, 0}, basis);

	if (NearestItem(&RT_Data, &origin, &direct, &itemIndex) != INFINITY) {
		if (true) {
			*cam = (camera){
				0, 0, 0,
				{cam->reper[0], cam->reper[1], cam->reper[2], cam->reper[3]},
				{cam->reper[0], cam->reper[1], cam->reper[2], cam->reper[3]}
			};
			cam->temp[3] = normalize(item[itemIndex].center - cam->temp[0]);
			if (cam->temp[3].x == 0) {
				cam->temp[1] = (float3){1, 0, 0};
			} else if (cam->temp[3].z == 0) {
				cam->temp[1] = (float3){0, 0, 1};
			} else {
				cam->temp[1] = normalize((float3){cam->temp[3].x,
					0, -pow(cam->temp[3].x, 2.f) / cam->temp[3].z});
			}
			cam->temp[2] = normalize(cross(cam->temp[1], cam->temp[3]));
			cam->temp[2] *= dot(cam->temp[2], (float3){0, 1, 0}) < 0.f ? -1.f : 1.f;
			if (dot(cam->temp[3], (float3){0, 0, 1}) > 0.f) {
				cam->temp[1] *= dot(cam->temp[1], (float3){1, 0, 0}) < 0.f ? -1.f : 1.f;
			} else {
				cam->temp[1] *= dot(cam->temp[1], (float3){1, 0, 0}) < 0.f ? 1.f : -1.f;
			}
			*cam = (camera){
				0, 0, 0,
				{cam->temp[0], cam->temp[1], cam->temp[2], cam->temp[3]},
				{cam->temp[0], cam->temp[1], cam->temp[2], cam->temp[3]}
			};
		}
		*active_item_address = (size_t)(item + itemIndex);
	} else {
		if (*active_item_address != 0x0) {
			*cam = (camera){
				0, 0, 0,
				{cam->reper[0], cam->reper[1], cam->reper[2], cam->reper[3]},
				{cam->reper[0], cam->reper[1], cam->reper[2], cam->reper[3]}
			};
		}
		*active_item_address = 0x0;
	}
}

kernel void	Rotate(global camera *cam, global size_t
	*active_item_address, float x, float y, float z, uint flags) {

	if (*active_item_address == 0x0) {
		cam->x += x;
		cam->y += y;
		cam->reper[3] = normalize(RotationAroundVector(cam->temp[1], cam->temp[3], cam->x, 0));
		cam->reper[3] = normalize(RotationAroundVector((float3){0, 1, 0}, cam->reper[3], cam->y, 0));
		if (cam->reper[3].x == 0) {
			cam->reper[1] = (float3){1, 0, 0};
		} else if (cam->reper[3].z == 0) {
			cam->reper[1] = (float3){0, 0, 1};
		} else {
			cam->reper[1] = normalize((float3){cam->reper[3].x,
				0, -pow(cam->reper[3].x, 2.f) / cam->reper[3].z});
		}
		cam->reper[2] = normalize(cross(cam->reper[1], cam->reper[3]));
		cam->reper[2] *= dot(cam->reper[2], (float3){0, 1, 0}) < 0.f ? -1.f : 1.f;
		if (dot(cam->reper[3], (float3){0, 0, 1}) > 0.f) {
			cam->reper[1] *= dot(cam->reper[1], (float3){1, 0, 0}) < 0.f ? -1.f : 1.f;
		} else {
			cam->reper[1] *= dot(cam->reper[1], (float3){1, 0, 0}) < 0.f ? 1.f : -1.f;
		}
	}

	global struct item *active_item = (global struct item*)*active_item_address;
	if (*active_item_address != 0x0) {
		{
			float angle_positive_axis = acos(
				dot(normalize(cam->reper[3]), (float3){0, 1, 0})
			);
			float angle_negative_axis = acos(
				dot(normalize(cam->reper[3]), (float3){0, -1, 0})
			);
			if ((x < 0.f && angle_positive_axis > (0.017453f * 5))
				|| (x > 0.f && angle_negative_axis > (0.017453f * 5))) {
				cam->x += x;
			}
			cam->y += y;
		}
		cam->reper[0] = RotationAroundVector(cam->temp[1],
			cam->temp[0] - active_item->center, cam->x, 0) + active_item->center;
		cam->reper[0] = RotationAroundVector((float3){0, 1, 0},
			cam->reper[0] - active_item->center, cam->y, 0) + active_item->center;
		cam->reper[3] = normalize(active_item->center - cam->reper[0]);
		if (cam->reper[3].x == 0) {
			cam->reper[1] = (float3){1, 0, 0};
		} else if (cam->reper[3].z == 0) {
			cam->reper[1] = (float3){0, 0, 1};
		} else {
			cam->reper[1] = normalize((float3){cam->reper[3].x,
				0, -pow(cam->reper[3].x, 2.f) / cam->reper[3].z});
		}
		cam->reper[2] = normalize(cross(cam->reper[1], cam->reper[3]));
		cam->reper[2] *= dot(cam->reper[2], (float3){0, 1, 0}) < 0.f ? -1.f : 1.f;
		if (dot(cam->reper[3], (float3){0, 0, 1}) > 0.f) {
			cam->reper[1] *= dot(cam->reper[1], (float3){1, 0, 0}) < 0.f ? -1.f : 1.f;
		} else {
			cam->reper[1] *= dot(cam->reper[1], (float3){1, 0, 0}) < 0.f ? 1.f : -1.f;
		}
	}
}

kernel void	ChangeColor(global size_t *active_item_address, float3 color)
{
	if (*active_item_address != 0x0) {
		((global struct item*)*active_item_address)->color = color;
	}
}
