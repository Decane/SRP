   /*-----------------------------------------------------------.   
  /                          Monochrome                             /
  '-----------------------------------------------------------*/
/*
  by Christian Cann Schuldt Jensen ~ CeeJay.dk
  
  Monochrome removes color and makes everything black and white.
*/

float4 MonochromePass( float4 colorInput )
{
  //calculate monochrome
  colorInput.rgb = dot(Monochrome_conversion_values, colorInput.rgb);
	
  //Return the result
  return saturate(colorInput);
}