   /*-----------------------------------------------------------.   
  /                          Levels                             /
  '------------------------------------------------------------/

by Christian Cann Schuldt Jensen ~ CeeJay.dk

Allows you to set a new black and a white level.
This increases contrast, but clips any colors outside the new range to either black or white
and so some details in the shadows or highlights can be lost.

The shader is very useful for expanding the 16-235 TV range to 0-255 PC range.
You might need it if you're playing a game meant to display on a TV with an emulator that does not do this.
But it's also a quick and easy way to uniformly increase the contrast of an image.

*/

#define black_point_float ( Levels_black_point / 255.0 )
#define white_point_float ( 255.0 / (Levels_white_point - Levels_black_point))

float4 LevelsPass( float4 colorInput )
{
  colorInput.rgb = colorInput.rgb * white_point_float - (black_point_float *  white_point_float);
  return colorInput;
}