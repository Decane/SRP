  /*-------------------------.
  | :: Defining constants :: |
  '-------------------------*/

//These values are normally defined by the injector dlls, but not when analyzed by GPU Shaderanalyzer
//I need to ensure they always have a value to be able to compile them whenever I'm not using the injector.
#ifdef SMAA_PIXEL_SIZE
  #ifndef BUFFER_RCP_WIDTH
    #define BUFFER_RCP_WIDTH SMAA_PIXEL_SIZE.x
    #define BUFFER_RCP_HEIGHT SMAA_PIXEL_SIZE.y
    #define BUFFER_WIDTH (1.0 / SMAA_PIXEL_SIZE.x)
    #define BUFFER_HEIGHT (1.0 / SMAA_PIXEL_SIZE.y)
  #endif
#endif

#ifndef BUFFER_RCP_WIDTH
  #define BUFFER_RCP_WIDTH (1.0 / 1680)
  #define BUFFER_RCP_HEIGHT (1.0 / 1050)
  #define BUFFER_WIDTH 1680
  #define BUFFER_HEIGHT 1050
#endif

#define screen_size float2(BUFFER_WIDTH,BUFFER_HEIGHT)

#define px BUFFER_RCP_WIDTH
#define py BUFFER_RCP_HEIGHT

#define pixel float2(px,py)

// -- Define DirectX9 FXAA specific aliases --
#if FXAA_HLSL_3 == 1
  #define myTex2D(s,p) tex2D(s,p)

  //#define s0 colorTexG
  //#define s1 colorTexG //TODO make a nearest sampler if needed
#endif

// -- Define DirectX10/11 FXAA specific aliases --
#if FXAA_HLSL_4 == 1
  //#define myTex2D(s,p) s.SampleLevel(screenSampler, p, 0)
  #define myTex2D(s,p) s.Sample(screenSampler, p)

  #define s0 gLumaTexture
  #define s1 gLumaTexture //TODO make a nearest sampler if needed
#endif


// -- Define DirectX9 specific aliases --
#if SMAA_HLSL_3 == 1
  #define myTex2D(s,p) tex2D(s,p)

  #define s0 colorTexG
  #define s1 colorTexG //TODO make a nearest sampler if needed
#endif

// -- Define DirectX10/11 specific aliases --
#if SMAA_HLSL_4 == 1 || SMAA_HLSL_4_1 == 1
  //#define myTex2D(s,p) s.SampleLevel(LinearSampler, p, 0)
  #define myTex2D(s,p) s.Sample(LinearSampler, p)

  #define s0 colorTexGamma
  #define s1 colorTexGamma //TODO make a nearest sampler if needed
#endif


  /*------------------------------.
  | :: Include enabled shaders :: |
  '------------------------------*/

#if (USE_CUSTOM == 1)
  #include "SweetFX\Shaders\Custom.h"
  #define Need_sRGB 1
#endif

#if (USE_EXPLOSION == 1)
  #include "SweetFX\Shaders\Explosion.h"
  #define Need_sRGB 1
#endif

#if (USE_CARTOON == 1)
  #include "SweetFX\Shaders\Cartoon.h"
  #define Need_sRGB 1
#endif

#if (USE_ADVANCED_CRT == 1)
  #include "SweetFX\Shaders\AdvancedCRT.h"
  #define Need_sRGB 1
#endif

#if (USE_SWEETCRT == 1)
  #include "SweetFX\Shaders\SweetCRT.h"
  #define Need_sRGB 1
#endif

#if (USE_BLOOM == 1)
  #include "SweetFX\Shaders\Bloom.h"
  #define Need_sRGB 1
#endif

#if (USE_HDR == 1)
  #include "SweetFX\Shaders\HDR.h"
  #define Need_sRGB 1
#endif

#if (USE_LUMASHARPEN == 1)
  #include "SweetFX\Shaders\LumaSharpen.h"
  #define Need_sRGB 1
#endif

#if (USE_LEVELS == 1)
  #include "SweetFX\Shaders\Levels.h"
  #define Need_sRGB 1
#endif

#if (USE_TECHNICOLOR == 1)
  #include "SweetFX\Shaders\Technicolor.h"
  #define Need_sRGB 1
#endif

#if (USE_DPX == 1)
  #include "SweetFX\Shaders\DPX.h"
  #define Need_sRGB 1
#endif

#if (USE_MONOCHROME == 1)
  #include "SweetFX\Shaders\Monochrome.h"
  #define Need_sRGB 1
#endif

#if (USE_LIFTGAMMAGAIN == 1)
  #include "SweetFX\Shaders\LiftGammaGain.h"
  #define Need_sRGB 1
#endif

#if (USE_TONEMAP == 1)
  #include "SweetFX\Shaders\Tonemap.h"
  #define Need_sRGB 1
#endif

#if (USE_SEPIA == 1)
  #include "SweetFX\Shaders\Sepia.h"
  #define Need_sRGB 1
#endif

#if (USE_VIBRANCE == 1)
  #include "SweetFX\Shaders\Vibrance.h"
  #define Need_sRGB 1
#endif

#if (USE_CURVES == 1)
  #include "SweetFX\Shaders\Curves.h"
  #define Need_sRGB 1
#endif

#if (USE_VIGNETTE == 1)
  #include "SweetFX\Shaders\Vignette.h"
  #define Need_sRGB 1
#endif

#if (USE_DITHER == 1)
  #include "SweetFX\Shaders\Dither.h"
  #define Need_sRGB 1
#endif

#if (USE_BORDER == 1)
  #include "SweetFX\Shaders\Border.h"
  #define Need_sRGB 1
#endif

#if (USE_SPLITSCREEN == 1)
  #include "SweetFX\Shaders\Splitscreen.h"
  #define Need_sRGB 1
#endif

  /*--------------------.
  | :: Effect passes :: |
  '--------------------*/

float4 SweetFX_main(float2 tex, float4 FinalColor)
{
  /*--------------------------------------.
  | :: Linear to sRGB Gamma correction :: |
  '--------------------------------------*/
  
  //#if Need_sRGB

  // Linear to sRGB Gamma correction. Needed here because SMAA uses linear for it's final step while the other shaders use SRGB.
  #if (USE_SMAA_ANTIALIASING == 1 && Need_sRGB == 1)
    //FinalColor.rgb = (FinalColor.rgb <= 0.00304) ? saturate(FinalColor.rgb) * 12.92 : 1.055 * pow( saturate(FinalColor.rgb), 1.0/2.4 ) - 0.055; // Linear to SRGB
    FinalColor.rgb = (FinalColor.rgb <= 0.00304) ? saturate(FinalColor.rgb * 12.92) : 1.055 * saturate(pow(FinalColor.rgb, 1.0/2.4 )) - 0.055; // Linear to SRGB
  #endif

  /*--------------------.
  | :: Effect passes :: |
  '--------------------*/

  // Custom
  #if (USE_CUSTOM == 1)
    FinalColor = CustomPass(FinalColor,tex);
  #endif

  // Explosion
  #if (USE_EXPLOSION == 1)
    FinalColor = ExplosionPass(FinalColor,tex);
  #endif

  // Cartoon
  #if (USE_CARTOON == 1)
	FinalColor = CartoonPass(FinalColor,tex);
  #endif

  // Advanced CRT
  #if (USE_ADVANCED_CRT == 1)
	FinalColor = AdvancedCRTPass(FinalColor,tex);
  #endif
  
  // SweetCRT
  #if (USE_SWEETCRT == 1)
    FinalColor = SweetCRTPass(FinalColor,tex);
  #endif

  // Bloom
  #if (USE_BLOOM == 1)
	FinalColor = BloomPass (FinalColor,tex);
  #endif

  // HDR
  #if (USE_HDR == 1)
	FinalColor = HDRPass (FinalColor,tex);
  #endif

  // LumaSharpen
  #if (USE_LUMASHARPEN == 1)
	FinalColor = LumaSharpenPass(FinalColor,tex);
  #endif

  // Levels
  #if (USE_LEVELS == 1)
	FinalColor = LevelsPass(FinalColor);
  #endif

  // Technicolor
  #if (USE_TECHNICOLOR == 1)
	FinalColor = TechnicolorPass(FinalColor);
  #endif

  // DPX
  #if (USE_DPX == 1)
	FinalColor = DPXPass(FinalColor);
  #endif

  // Monochrome
  #if (USE_MONOCHROME == 1)
	FinalColor = MonochromePass(FinalColor);
  #endif

  // Lift Gamma Gain
  #if (USE_LIFTGAMMAGAIN == 1)
	FinalColor = LiftGammaGainPass(FinalColor);
  #endif

  // Tonemap
  #if (USE_TONEMAP == 1)
	FinalColor = TonemapPass(FinalColor);
  #endif

  // Vibrance
  #if (USE_VIBRANCE == 1)
	FinalColor = VibrancePass(FinalColor);
  #endif

  // Curves
  #if (USE_CURVES == 1)
	FinalColor = CurvesPass(FinalColor);
  #endif

  // Sepia
  #if (USE_SEPIA == 1)
    FinalColor = SepiaPass(FinalColor);
  #endif

  // Vignette
  #if (USE_VIGNETTE == 1)
	FinalColor = VignettePass(FinalColor,tex);
  #endif

  // Dither (should go near the end as it only dithers what went before it)
  #if (USE_DITHER == 1)
	FinalColor = DitherPass(FinalColor,tex);
  #endif

  // Border
  #if (USE_BORDER == 1)
    FinalColor = BorderPass(FinalColor,tex);
  #endif

  // Splitscreen
  #if (USE_SPLITSCREEN == 1)
	  FinalColor = SplitscreenPass(FinalColor,tex);
  #endif

  /*--------------------------------------.
  | :: sRGB to Linear Gamma correction :: |
  '--------------------------------------*/

  // sRGB to Linear gamma correction.
  #if (SMAA_HLSL_3 != 1 && FXAA_HLSL_3 != 1) //If we are not using DirectX 9.. (and therefore are using DX10/11)
  
    #if (USE_SMAA_ANTIALIASING == 1 && Need_sRGB != 1) //and if we used SMAA (and thus Linear) and we didn't earlier convert to sRGB
       //..then we are already linear and don't need to do anything
       
	  #else //if not, then we are currently sRGB and need to be Linear. - convert to Linear
      FinalColor.rgb = saturate(FinalColor.rgb);
      FinalColor.rgb = (FinalColor.rgb <= 0.03928) ? FinalColor.rgb / 12.92 : pow( (FinalColor.rgb + 0.055) / 1.055, 2.4 ); // sRGB to Linear
    
    #endif
   
  //But if we were using DirectX 9 then no matter what we should be sRGB at this point and don't need to do anything further.
   
  #endif

  // Return FinalColor
  FinalColor.a = 1.0; //Clear alpha channel to reduce filesize of screenshots that are converted to png and avoid problems when viewing the screenshots.
  return FinalColor;
}
