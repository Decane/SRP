/*------------------------------------------------------------------------------
						FXAA SHADER
------------------------------------------------------------------------------*/

uniform extern texture gScreenTexture;
uniform extern texture gLumaTexture;

//Include SweetFX settings
#include "SweetFX_preset.txt"
#include "SweetFX/SweetFX_compatibility_settings.txt"

//Make sure shaders don't think SMAA is enabled since InjectFXAA injector can't do SMAA
#define USE_SMAA_ANTIALIASING 0 //disable SMAA

float fxaaQualitySubpix = fxaa_Subpix;
float fxaaQualityEdgeThreshold = fxaa_EdgeThreshold;
float fxaaQualityEdgeThresholdMin = fxaa_EdgeThresholdMin;

// Defines the API to use it with
#define FXAA_HLSL_3 1

// Include the FXAA shader
#if (USE_FXAA_ANTIALIASING == 1)
    #include "SweetFX\Shaders\Fxaa3_11.h"
#endif

// Define samplers
#define s0 lumaSampler //NearestScreenSampler
#define s1 LinearScreenSampler //LinearScreenSampler
//#define width BUFFER_WIDTH
//#define height BUFFER_HEIGHT
//#define px BUFFER_RCP_WIDTH
//#define py BUFFER_RCP_HEIGHT



//Definitions: BUFFER_WIDTH, BUFFER_HEIGHT, BUFFER_RCP_WIDTH, BUFFER_RCP_HEIGHT

sampler NearestScreenSampler = sampler_state
{
    Texture = <gScreenTexture>;
    MinFilter = NONE;
    MagFilter = NONE;
    MipFilter = NONE;
    AddressU = CLAMP; //MIRROR affects the edges more
    AddressV = CLAMP; //MIRROR affects the edges more
    SRGBTexture = FALSE;
};

sampler LinearScreenSampler = sampler_state
{
    Texture = <gScreenTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = CLAMP; //MIRROR affects the edges more
    AddressV = CLAMP; //MIRROR affects the edges more
    SRGBTexture = FALSE;
};


sampler screenSampler = sampler_state
{
    Texture = <gScreenTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = BORDER;
    AddressV = BORDER;
    SRGBTexture = FALSE;
};
sampler lumaSampler = sampler_state
{
    Texture = <gLumaTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = BORDER;
    AddressV = BORDER;
    SRGBTexture = FALSE;
};

//Include the main SweetFX control shader
#include "SweetFX/Shaders/Main.h"

// FXAA Shader Function
float4 LumaShader( float2 Tex : TEXCOORD0 ) : COLOR0
{
#if(USE_FXAA_ANTIALIASING == 1) //TODO this should probably be moved up so the entire function won't run if AA is not enabled. Get it working first before trying this though.
    float4 c0 = FxaaPixelShader(
		// pos, Output color texture
		Tex,
		// tex, Input color texture
		screenSampler,
		// fxaaQualityRcpFrame, gets coordinates for screen width and height, xy
		float2(BUFFER_RCP_WIDTH, BUFFER_RCP_HEIGHT),
		//fxaaConsoleRcpFrameOpt2, gets coordinates for screen width and height, xyzw
		float4(-2.0*BUFFER_RCP_WIDTH,-2.0*BUFFER_RCP_HEIGHT,2.0*BUFFER_RCP_WIDTH,2.0*BUFFER_RCP_HEIGHT),
		// Choose the amount of sub-pixel aliasing removal
		fxaaQualitySubpix,
		// The minimum amount of local contrast required to apply algorithm
		fxaaQualityEdgeThreshold,
		// Trims the algorithm from processing darks
		fxaaQualityEdgeThresholdMin
	);
#else
	float4 c0 = tex2D(screenSampler,Tex);
#endif
    return c0;
}

float4 main( float2 Tex : TEXCOORD0 ) : COLOR0
{
  float4 c0 = tex2D(lumaSampler,Tex);
	c0 = SweetFX_main(Tex,c0); // Add the other effects
	c0.w = 1.0;
  return saturate(c0);
}

//What Shader Model should we use?
#if (SweetFX_shader_model == 0)
  #define vertex_shader_model vs_2_0
  #define pixel_shader_model  ps_2_0
  
#elif (SweetFX_shader_model == 1)
  #define vertex_shader_model vs_2_0
  #define pixel_shader_model  ps_2_a
  
#elif (SweetFX_shader_model == 2)
  #define vertex_shader_model vs_2_0
  #define pixel_shader_model  ps_2_b
  
#else
  #define vertex_shader_model vs_3_0
  #define pixel_shader_model  ps_3_0
  
#endif

technique PostProcess1
{
    pass p1
    {
        PixelShader = compile pixel_shader_model LumaShader();
    }
}

technique PostProcess2
{
    pass p1
    {
        PixelShader = compile pixel_shader_model main();
    }
}