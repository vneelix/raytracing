#include "clheader.clh"

float3	color_plane(__global struct item *item, __global uint *texture, float3 vector_p, float3 point_p)
{
	float	v;
	float	u;
	float3	temp;
	float3	vector_y;
	float3	op;

	op = point_p - item->center;
	vector_y = normalize(cross(item->textr.vector_n, item->textr.vector_e));
	v = dot(op, vector_y);
	u = dot(op, item->textr.vector_e);
	temp.x = u;
	temp.y = v;
	return (temp);
} 

float3	color_cone(__global struct item *item, __global uint *texture, float3 vector_p, float3 point_p)
{
	float	v;
	float	u;
	float3	op;
	float	opi;
	float3	vector_y;
	float3	temp;

	op = point_p - item->center;
	opi = length(op) * cos (atan(item->k ));
	if (dot(item->textr.vector_n, normalize(op)) < 0)
		opi = -opi;
	op = item->center + (item->textr.vector_n * opi);
	vector_y = point_p - op;
	temp.z = length(vector_y);
	v = (opi - item->min) / (item->max - item->min);
	vector_y = normalize(vector_y);
	u = acos((float)dot(item->textr.vector_e, vector_y)) / (2 * M_PI);
	temp.x = u;
	temp.y = v;
	return (temp);
} 

float3	color_cil(__global struct item *item, __global uint *texture, float3 point_p)
{
	float	hp;
	float	v;
	float	u;
	float3	op;
	float3	opi;
	float3	vector_y;
	float3	temp;

	op = point_p - item->center;
	hp = sqrt((length(op) * length(op) + (float)1e-1) - item->radius * item->radius);
	if (dot(normalize(item->textr.vector_n), normalize(op)) < 0)
		hp = -hp;
	opi = item->textr.vector_n * hp;
	opi = opi + item->center;
	vector_y =  opi - point_p;
	vector_y = normalize(vector_y);
	v = (hp - item->min) / (item->max - item->min);
	u = (acos((float)dot(normalize(item->textr.vector_e), vector_y))) / (2 * M_PI);
	temp.x = u;
	temp.y = v;
	return (temp);
} 

float3	color_sphere(__global struct item *item, __global uint *texture, float3 vector_p)
{
	float	phi;
	float	v;
	float	u;
	float3	temp;

	vector_p = normalize(vector_p);
	phi = acos((float)dot(vector_p, item->textr.vector_n));
	v = phi / M_PI;
	float theta = (acos((float)dot(vector_p, item->textr.vector_e) / sin(phi)) ) / (2 * M_PI);
	if (dot(cross(item->textr.vector_n, item->textr.vector_e), vector_p) < 0)
		u = theta;
	else
		u = 1 - theta;
	temp.x = u;
	temp.y = v;
	return (temp);
} 

float4	color_to_float4(uint pixel)
{
	return (float4) (pixel >> 24 & 255, pixel >> 16 & 255, pixel >> 8 & 255, pixel & 255);
}

float4	get_texture_color(__global struct item *item, __global uint *texture, float3 vector_p, float3 point_p, int n_texture, float3 default_color, bool calc_transparency)
{
	int		x;
	int		y;
	int		index;
	float3	temp;

	if (n_texture < 0)
		return ((float4)(1 - item->refractRatio, default_color));

	index = texture[n_texture];
	uint w = texture[index], h = texture[index + 1];

	if (item->type == SPHERE)
	{
		temp = color_sphere(item, texture, vector_p);
		temp.x = (int) (w * temp.x);
		temp.y = (int) (h * temp.y);
		if (((int)item->textr.texture_tilling) == 1)
		{
			float s = 5.0;
			int jump = ((int)((temp.x / item->textr.texture_scale) / s) + (int)((temp.y / item->textr.texture_scale)/ s)) % 2;
			if (jump == 0)	
				return((float4)(1 - item->refractRatio, item->color[0],item->color[1], item->color[2]));
			else
				return((float4)(1 - item->refractRatio, 255.0, 255.0, 255.0)); 	
		}
		else if (((int)item->textr.texture_tilling) == 2)
		{
			if (sin(temp.x / item->textr.texture_scale) > 0)	
				return((float4)(1 - item->refractRatio, item->color[0],item->color[1], item->color[2]));
			else
				return((float4)(1 - item->refractRatio, 255.0, 255.0, 255.0)); 
		}
		x = temp.x;
		y = temp.y;
	}
	else if (item->type == PLANE)
	{
		temp = color_plane(item, texture, vector_p, point_p);
		temp.x = (int) (temp.x * item->textr.texture_scale) % w;
		temp.y = (int) (temp.y * item->textr.texture_scale) % h;
		if (((int)item->textr.texture_tilling) == 1)
			{
				float s = 5.0;
				int jump = ((int)((temp.x) / s) + (int)((temp.y) / s)) % 2;
				if (jump == 0)	
					return((float4)(1 - item->refractRatio, item->color[0],item->color[1], item->color[2]));
				else
					return((float4)(1 - item->refractRatio, 255.0, 255.0, 255.0)); 	
			}
			else if (((int)item->textr.texture_tilling) == 2)
			{
				// int jump = ((int)temp.x) % 2;
				if (sin(temp.x) > 0)	
					return((float4)(1 - item->refractRatio, item->color[0],item->color[1], item->color[2]));
				else
					return((float4)(1 - item->refractRatio, 255.0, 255.0, 255.0)); 
			}
		x = temp.x;
		y = temp.y;
	}
	else if (item->type == CYLINDER)
	{
		temp = color_cil(item, texture, point_p);
		if (((int)item->textr.texture_tilling) == 1)
		{
			float s = 5.0;
			int jump = ((int)((temp.x * 100) / s) + (int)((temp.y * 100)/ s)) % 2;
			if (jump == 0)	
				return((float4)(1 - item->refractRatio, item->color[0],item->color[1], item->color[2]));
			else
				return((float4)(1 - item->refractRatio, 255.0, 255.0, 255.0)); 	
		}
		else if (((int)item->textr.texture_tilling) == 2)
		{
			// Space Filling Stripes
			int jump = ((int)(temp.x * 100)) % 2;
			if (jump == 0) 	// if (sin(temp.x *36) > 0)	
				return((float4)(1 - item->refractRatio, item->color[0], item->color[1], item->color[2]));
			else
				return((float4)(1 - item->refractRatio, 255.0, 255.0, 255.0)); 
		}
		else
		{		
			x = (int) (w * temp.x) % w;
			y = (int) (h * temp.y) % h;
		}
	
	}
	else if (item->type == CONE)
	{
		temp = color_cone(item, texture, vector_p, point_p);
		temp.x = (int) (w * temp.x) % w;
		temp.y = (int) (h * temp.y) % h;
		if (((int)item->textr.texture_tilling) == 1)
		{
			float s = 5.0;
			int jump = ((int)((temp.x / item->textr.texture_scale) / s) + (int)((temp.y / item->textr.texture_scale)/ s)) % 2;
			if (jump == 0)	
				return((float4)(1 - item->refractRatio, item->color[0], item->color[1], item->color[2]));
			else
				return((float4)(1 - item->refractRatio, 255.0, 255.0, 255.0)); 	
		}
		else if (((int)item->textr.texture_tilling) == 2)
		{
			if (sin(temp.x / item->textr.texture_scale) > 0)	
				return((float4)(1 - item->refractRatio, item->color[0],item->color[1], item->color[2]));
			else
				return((float4)(1 - item->refractRatio, 255.0, 255.0, 255.0)); 
		}
		x = temp.x;
		y = temp.y;
	}
	else
		return ((float4)(1 - item->refractRatio, default_color));
	uint pixel = texture[index + w*y + x + 2];
	float4 color = color_to_float4(pixel);
	if (calc_transparency) {
		float k = color.s0 / 255.0f;
		return ((float4)(k, color.s123 * k));
	}

	return ((float4)(1 - item->refractRatio, color.s123));
}

float4	return_color_item(__global struct item *item, __global uint *texture, float3 vector_p, float3 point_p)
{
	return get_texture_color(item, texture, vector_p, point_p, item->textr.n_texture, item->color, true);
}

float3	get_normal_by_texture(__global struct item *item, __global uint *texture, float3 vector_p, float3 point_p)
{
	float4 color = get_texture_color(item, texture, vector_p, point_p, item->textr.normal_map, (128), false);
	color /= 255.f;
	color.y = 0.5 - color.y;
	color.z = 0.5 - color.z;
	color.w = 1.f - color.w;
	color.yzw = vector_p - color.yzw;
	return normalize(color.yzw);
}

float3	get_mapped_normal(__global struct item *item, __global uint *texture, float3 normal, float3 point)
{
	if (item->textr.normal_map < 0) 
		return normal;
	return normalize(get_normal_by_texture(item, texture, normal, point));
}
