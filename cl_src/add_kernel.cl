#include "clheader.h"

kernel void	MoveOrigin(global float3 *cam_reper, uchar flags, struct opt opt) {
	float3	dir[2] = {
		normalize(Rotation((float3){0, 0, 1}, opt.spin_x, opt.spin_y, 0, 0)),
		normalize(Rotation((float3){1, 0, 0}, opt.spin_x, opt.spin_y, 0, 0)),
	};
	if (flags & (uchar)1) {
		*cam_reper += dir[0];
	} else if (flags & (uchar)1 << 1) {
		*cam_reper -= dir[0];
	} else if (flags & (uchar)1 << 2) {
		*cam_reper -= dir[1];
	} else if (flags & (uchar)1 << 3) {
		*cam_reper += dir[1];
	} else if (flags & (uchar)1 << 4) {
		*cam_reper += (float3){0, -1, 0};
	} else if (flags & (uchar)1 << 5) {
		*cam_reper -= (float3){0, -1, 0};
	}
}

kernel void	FindItem(global float3 *cam_reper, global struct item *item,
	global size_t *active_item_address, int x, int y, struct opt opt) {

	int		itemIndex = 0;
	float3	origin = *cam_reper;
	float3	direct = CalcDirect(x + opt.w * y, &opt);
	struct	RT_Data RT_Data = {0, opt.item_c, NULL, item, NULL, 0};

	global struct item *active_item = (global struct item*)*active_item_address;

	if (false/* active_item != NULL */) {
		origin = RotationAround(active_item->center, *cam_reper, opt.spin_x, opt.spin_y, 0, 0);
		direct = normalize(active_item->center - origin);
	} else {
		direct = Rotation(direct, opt.spin_x, opt.spin_y, 0, 0);
	}

	if (active_item == NULL && NearestItem(&RT_Data, &origin, &direct, &itemIndex) != INFINITY) {
		/* if (active_item != NULL) {
			*cam_reper = RotationAround(active_item->center, *cam_reper, opt.spin_x, opt.spin_y, 0, 0);
		} */
		*active_item_address = (size_t)(item + itemIndex);
	} else {
		/* if (active_item != NULL) {
			*cam_reper = RotationAround(active_item->center, *cam_reper, opt.spin_x, opt.spin_y, 0, 0);
		} */
		*active_item_address = 0;
	}
}
