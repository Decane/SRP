  /*-----------------------------------------------------------.
 /                          Border                            /
'-----------------------------------------------------------*/
// Version 1.2

/*
Version 1.0 by Oomek
- Fixes light, one pixel thick border in some games when forcing MSAA like i.e. Dishonored

Version 1.1 by CeeJay.dk
- Optimized the shader. It still does the same but now it runs faster.

Version 1.2 by CeeJay.dk
- Added border_width and border_color features

Version 1.3 by CeeJay.dk
- Optimized the performance further
*/

#ifndef border_width
  #define border_width float2(1,0)
#endif
#ifndef border_color
  #define border_color float3(0, 0, 0)
#endif

float4 BorderPass( float4 colorInput, float2 tex )
{
float3 border_color_float = border_color / 255.0;

float2 border = (pixel * border_width); //Translate integer pixel width to floating point
float2 within_border = saturate((-tex * tex + tex) - (-border * border + border)); //becomes positive when inside the border and 0 when outside

colorInput.rgb = all(within_border) ?  colorInput.rgb : border_color_float ; //

return colorInput; //return the pixel

} 