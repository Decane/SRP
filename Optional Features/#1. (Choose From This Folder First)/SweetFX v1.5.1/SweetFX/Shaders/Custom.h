   /*-----------------------------------------------------------.
  /                           Custom                            /
  '-----------------------------------------------------------*/
/*

*/

#define CustomLuma float3(0.2126, 0.7152, 0.0722)

float4 CustomPass( float4 colorInput, float2 tex )
{

  //just some example code. 
  float luma = dot(colorInput.rgb,CustomLuma); //Calculate luma
  float3 chroma = colorInput.rgb - luma; //Calculate chroma
  float3 color = 1.0 - luma; //invert the luma
  
  color = color + chroma; //add the chroma back in
  
  colorInput.rgb = lerp(colorInput.rgb, color, custom_strength); //Adjust the strength of the effect

  return saturate(colorInput);
}