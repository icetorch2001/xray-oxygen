#include "common.h"

float NormalizeDepth(float Depth)
{
	return saturate(Depth / 100.f);
}

float4 main(p_screen I) : SV_Target
{
#if SM_2_0
	float3 	scene 		= tex2D(s_image, I.tc0.xy * screen_res.xy).xyz;
	float 	sceneDepth 	= tex2D(s_position, I.tc0.xy * screen_res.xy).z;
#else
	float4 	scene 		= s_image.Load(int3(I.tc0.xy * screen_res.xy, 0), 0);
	float 	sceneDepth 	= s_position.Load(int3(I.tc0.xy * screen_res.xy, 0), 0).z;
#endif
	float 	RESDepth 	= NormalizeDepth(sceneDepth) * 1000;
	
	float4 	outColor 	= float4(scene.xyz*(1 - RESDepth),RESDepth);
	return outColor;
}