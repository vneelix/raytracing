#include "./cl_inc/clheader.clh"

kernel void	PostProcessing(global uint *pixel, uint w, uint h, uchar flag) {
	size_t id = get_global_id(0);

	uint2	fragCoord = (uint2){id % w, (id - id % w) / w};
	float2	uv = (float2){fragCoord.x, fragCoord.y} / (float2){w, h};

	float	r = (pixel[id] & ((uint)255 << 16)) >> 16;
	float	g = (pixel[id] & ((uint)255 << 8)) >> 8;
	float	b = (pixel[id] & (uint)255);

	if (flag & (1 << 0)) { //gray
		float gray_mix = r * 0.33f + g * 0.33f + b * 0.33f;
		pixel[id] = RGBtoUint((float3){gray_mix, gray_mix, gray_mix});
	} else if (flag & (1 << 1)) { //wave effect
		float stripes = 80;
		float waves = 120;
		float wobble = 0.5f * sin(uv.y * stripes * M_PI * 2.0f + cos(uv.x * M_PI * 2.0f * waves));
		float result = wobble + dot((float3){1.f, 1.f, 1.f}, (float3){0.213f, 0.715f, 0.072f} * normalize((float3){r, g, b}));
		pixel[id] = RGBtoUint((float3){r, g, b} * result);
	} else if (flag & (1 << 2)) { //grain
		float strength = 32.0;
		float x = (uv.x + 4.0f ) * (uv.y + 4.0f ) * 230;
		float grain_ratio = (fmod((fmod(x, 13.0f) + 1.0f) * (fmod(x, 123.0f) + 1.0f), 0.01f) - 0.005f) * strength;

		grain_ratio = 1.f - grain_ratio;
		pixel[id] = RGBtoUint((float3){r, g, b} * grain_ratio);
	} else if (flag & (1 << 3)) { //lcd effect
		float	pb = 0.4;
		float3	lcdColor = (float3){pb,pb,pb};
		int		px = (int)fmod(fragCoord.x, 3.0f);

		if (px == 1) {
			lcdColor.x = 1.0f;
		}
		else if (px == 2) {
			lcdColor.y = 1.0f;
		}
		else {
			lcdColor.z = 1.0f;
		}	
    	float sclV = 0.24;
    	if ((int)(fmod(fragCoord.y, 3.0f)) == 0) {
			lcdColor.xyz = (float3){sclV,sclV,sclV};
		}
		pixel[id] = RGBtoUint((float3){r, g, b} * lcdColor.xyz);
	} else if (flag & (1 << 4)) { //pixels
		float2 center = floor(((float2){(float)fragCoord.x, (float)fragCoord.y} / 6.0f)) * 6.0f + 3.0f;
		center = clamp((float2){0, 0}, (float2){w, h}, center);
		r = (pixel[(int)center.x + w * (int)center.y] & ((uint)255 << 16)) >> 16;
		g = (pixel[(int)center.x + w * (int)center.y] & ((uint)255 << 8)) >> 8;
		b = (pixel[(int)center.x + w * (int)center.y] & (uint)255);
		float l = max(0.1f, dot((float3){r, g, b}, (float3){0.2125f, 0.7154f, 0.0721f}));
		float dist = distance(center, (float2){fragCoord.x, fragCoord.y}) / 8.0f;
		float alpha = smoothstep(1.0f, 0.5f, dist / l);
		pixel[id] = RGBtoUint((float3){r, g, b} * alpha);
	}
}
