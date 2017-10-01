   /*-----------------------------------------------------------.   
  /                         Explosion                           /
  '-----------------------------------------------------------*/
/* 

*/

float4 ExplosionPass( float4 colorInput, float2 tex )
{

  // -- pseudo random number generator --
  float2 sine_cosine;
  sincos(dot(tex, float2(12.9898,78.233)),sine_cosine.x,sine_cosine.y);
  sine_cosine = sine_cosine * 43758.5453 + tex;
  float2 noise = frac(sine_cosine);

  tex = (-Explosion_Radius * pixel) + tex; //Slightly faster this way because it can be calculated while we calculate noise.
  
  colorInput.rgb = myTex2D(s0, (2.0 * Explosion_Radius * pixel) * noise + tex).rgb;
  
 
  return colorInput;
}