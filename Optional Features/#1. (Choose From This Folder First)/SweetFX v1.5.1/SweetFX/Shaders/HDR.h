/*------------------------------------------------------------------------------
						HDR
------------------------------------------------------------------------------*/

float4 HDRPass( float4 colorInput, float2 Tex )
{
	float3 c_center = myTex2D(s0, Tex).rgb; //reuse SMAA center sample or lumasharpen center sample?
	//float3 c_center = colorInput.rgb; //or just the input?
	
	//float3 bloom_sum1 = float3(0.0, 0.0, 0.0); //don't initialize to 0 - use the first tex2D to do that
	//float3 bloom_sum2 = float3(0.0, 0.0, 0.0); //don't initialize to 0 - use the first tex2D to do that
	//Tex += float2(0, 0); // +0 ? .. oh riiiight - that will surely do something useful
	
	float radius1 = 0.793;
	float3 bloom_sum1 = myTex2D(s0, Tex + float2(1.5, -1.5) * radius1).rgb;
	bloom_sum1 += myTex2D(s0, Tex + float2(-1.5, -1.5) * radius1).rgb; //rearrange sample order to minimize ALU and maximize cache usage
	bloom_sum1 += myTex2D(s0, Tex + float2(1.5, 1.5) * radius1).rgb;
	bloom_sum1 += myTex2D(s0, Tex + float2(-1.5, 1.5) * radius1).rgb;
	
	bloom_sum1 += myTex2D(s0, Tex + float2(0, -2.5) * radius1).rgb;
	bloom_sum1 += myTex2D(s0, Tex + float2(0, 2.5) * radius1).rgb;
	bloom_sum1 += myTex2D(s0, Tex + float2(-2.5, 0) * radius1).rgb;
	bloom_sum1 += myTex2D(s0, Tex + float2(2.5, 0) * radius1).rgb;
	
	bloom_sum1 *= 0.005;
	
	float3 bloom_sum2 = myTex2D(s0, Tex + float2(1.5, -1.5) * radius2).rgb;
	bloom_sum2 += myTex2D(s0, Tex + float2(-1.5, -1.5) * radius2).rgb;
	bloom_sum2 += myTex2D(s0, Tex + float2(1.5, 1.5) * radius2).rgb;
	bloom_sum2 += myTex2D(s0, Tex + float2(-1.5, 1.5) * radius2).rgb;


	bloom_sum2 += myTex2D(s0, Tex + float2(0, -2.5) * radius2).rgb;	
	bloom_sum2 += myTex2D(s0, Tex + float2(0, 2.5) * radius2).rgb;
	bloom_sum2 += myTex2D(s0, Tex + float2(-2.5, 0) * radius2).rgb;
	bloom_sum2 += myTex2D(s0, Tex + float2(2.5, 0) * radius2).rgb;

	bloom_sum2 *= 0.010;
	
	float dist = radius2 - radius1;
	
	float3 HDR = (c_center + (bloom_sum2 - bloom_sum1)) * dist;
	float3 blend = HDR + colorInput.rgb;
	colorInput.rgb = HDR + pow(blend, HDRPower); // pow - don't use fractions for HDRpower
	
	return saturate(colorInput);
}