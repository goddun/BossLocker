cbuffer cbPerObjectVS : register( b0 )
{
    float4x4 WorldViewProjection	: packoffset( c0 );
	float4x4 World					: packoffset( c4 );
}


cbuffer cbPerObjectPS : register( b0 ) // Model constants
{
    float3 EyePosition	: packoffset( c0 ); //카메라 위치, 월드공간에서의
	float specExp		: packoffset( c0.w );// 정반사광 계수, 높을수록 정반사광이 적게 퍼짐
	float specIntensity	: packoffset( c1 );//정반사광 강도
}


Texture2D    DiffuseTexture	: register( t0 );//기본 텍스쳐
SamplerState LinearSampler	: register( s0 );//선형 샘플러

struct VS_INPUT
{
    float4 Position	: POSITION;		// vertex position 
    float3 Normal	: NORMAL;		// vertex normal
    float2 UV		: TEXCOORD0;	// vertex texture coords 
};

struct VS_OUTPUT
{
    float4 Position	: SV_POSITION;	// vertex position 
    float2 UV		: TEXCOORD0;	// vertex texture coords
	float3 Normal	: TEXCOORD1;	// vertex normal
	float3 WorldPos	: TEXCOORD2;	// vertex world position
};



float4 DepthPrePassVS(float4 Position : POSITION) : SV_POSITION
{
	return mul( Position, WorldViewProjection );
}

VS_OUTPUT RenderSceneVS( VS_INPUT input )
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;
    
    Output.Position = mul( input.Position, WorldViewProjection );//투영공간으로 변환
    
	Output.WorldPos = mul(input.Position, World).xyz;//월드공간에서의 정점 위치

    Output.UV = input.UV; //UV는 단순히 전달.

	Output.Normal = mul(input.Normal, (float3x3)World);//노말은 월드공간으로 변환해서 전달.
    
    return Output;    
}