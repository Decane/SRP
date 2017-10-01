   /*-----------------------------------------------------------.   
  /                        SweetCRT                            /
  '-----------------------------------------------------------*/
/*
SweetCRT - a Work in progress.
*/

#define scanline_strength 0.75

float4 SweetCRTPass( float4 colorInput, float2 tex )
{
   //
   float scanlines = frac(tex.y * (screen_size.y * 0.5)) - 0.49; //
   scanlines = 1.0 + scanlines * scanline_strength;
   
   colorInput.rgb = saturate(colorInput.rgb * scanlines);
   //colorInput.rgb = saturate(scanlines).xxx;
  
   return colorInput;
}