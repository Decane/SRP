/*------------------------------------------------------------------------------
						Cartoon
------------------------------------------------------------------------------*/

#ifndef CartoonEdgeSlope //for backwards compatiblity with settings preset from earlier versions of SweetFX
  #define CartoonEdgeSlope 1.5 
#endif

float4 CartoonPass( float4 colorInput, float2 Tex )
{
  float3 CoefLuma2 = float3(0.2126, 0.7152, 0.0722);  //Values to calculate luma with
  
  float diff1 = dot(CoefLuma2,myTex2D(s0, Tex + pixel).rgb);
  diff1 = dot(float4(CoefLuma2,-1.0),float4(myTex2D(s0, Tex - pixel).rgb , diff1));
  
  float diff2 = dot(CoefLuma2,myTex2D(s0, Tex +float2(pixel.x,-pixel.y)).rgb);
  diff2 = dot(float4(CoefLuma2,-1.0),float4(myTex2D(s0, Tex +float2(-pixel.x,pixel.y)).rgb , diff2));
    
  float edge = dot(float2(diff1,diff2),float2(diff1,diff2));
  
  colorInput.rgb =  pow(edge,CartoonEdgeSlope) * -CartoonPower + colorInput.rgb;
	
  return saturate(colorInput);
}
