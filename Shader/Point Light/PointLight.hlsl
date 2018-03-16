#include "ForwardLightCommon.hlsl"

/////////////////////////////////////////////////////////////////////////////
// Constant Buffers
/////////////////////////////////////////////////////////////////////////////

cbuffer PointLightConstants : register(b1) // Point light constants
{
	float3 PointLightPosition	: packoffset(c0);
	float PointLightRangeRcp : packoffset(c0.w);
	float3 PointLightColor		: packoffset(c1);
}


float3 CalcPoint(float3 worldPosition, float3 worldNormal, float4 diffuseColor)
{
	float3 ToLight = PointLightPosition.xyz - worldPosition;
	float3 ToEye = EyePosition.xyz - worldPosition;
	float DistToLight = length(ToLight);

	// Phong diffuse
	ToLight /= DistToLight; // Normalize
	float NDotL = saturate(dot(ToLight, worldNormal));
	float3 finalColor = PointLightColor.rgb * NDotL;

	// Blinn specular
	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + ToLight);
	float NDotH = saturate(dot(HalfWay, worldNormal));
	finalColor += PointLightColor.rgb * pow(NDotH, specExp) * specIntensity;

	// Attenuation
	float DistToLightNorm = 1.0 - saturate(DistToLight * PointLightRangeRcp);
	float Attn = DistToLightNorm * DistToLightNorm;
	finalColor *= diffuseColor * Attn;

	return finalColor;
}


float4 PointLightPS(VS_OUTPUT In) : SV_TARGET0
{
	In.Normal = normalize(In.Normal);

	float4 diffuseColor = DiffuseTexture.Sample(LinearSampler, In.UV);//기본 텍스쳐의 텍셀을 얻어옴
	diffuseColor.rgb *= diffuseColor.rgb;//색을 제곱해서 선형공간으로 변환

									 // Calculate the point light color
	float3 finalColor = CalcPoint(In.WorldPos, In.Normal,diffuseColor);

	// Return the final color
	return float4(finalColor, 1.0);
}