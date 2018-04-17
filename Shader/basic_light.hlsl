struct VSInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
};

struct VSOutput
{
	float4 positionCS : SV_POSITION;
	float3 cameraPosition : CAMERA_POSITION;
	float4 position : POSITION0;

	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float3 color : COLOR;
	
	float4 lightViewPosition:POSITION1;
};

cbuffer VSSystemUniforms : register(b0)
{
	float4x4 SUNNY_ProjectionMatrix;
	float4x4 SUNNY_ViewMatrix;
	float4x4 SUNNY_ModelMatrix;
	float3	 SUNNY_CameraPosition;
	float4x4 lightViewMatrix;// 추가
	float4x4 lightProjectionMatrix;//추가
};

VSOutput VSMain(in VSInput input)
{
	float3x3 wsTransform = (float3x3)SUNNY_ModelMatrix;

	VSOutput output;
	output.position = mul(input.position, SUNNY_ModelMatrix);
	output.positionCS = mul(output.position, mul(SUNNY_ViewMatrix, SUNNY_ProjectionMatrix));

	output.lightViewPosition = mul(output.position, lightViewMatrix);//추가
	output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);//추가

	output.normal = mul(input.normal, wsTransform);
	output.binormal = mul(input.binormal, wsTransform);
	output.tangent = mul(input.tangent, wsTransform);
	output.uv = input.uv;
	output.color = float3(1.0f, 1.0f, 1.0f);
	output.shadowCoord = float4(0.0f, 0.0f, 0.0f, 0.0f); 

	output.cameraPosition = SUNNY_CameraPosition;

	return output;
}

struct Light
{
	float4 color;
	float3 position;
	float p0;
	float3 ambientDown;
	float p1;
	float3 ambientRange;
	float intensity;
};

struct Attributes
{
	float3 position;
	float2 uv;
	float3 normal;
	float3 binormal;
	float3 tangent;
};



cbuffer PSSystemUniforms : register(b0)
{
	Light SUNNY_Light;
	float4 SUNNY_Color;
	float  SUNNY_HasTexture;
};


float3 CalcAmbient(float3 normal, float3 diffuseColor)
{
	float up = normal.y * 0.5 + 0.5;

	float3 ambient = SUNNY_Light.ambientDown + up * SUNNY_Light.ambientRange;

	return ambient * diffuseColor;
}

float3 CalcDirectional(float3 worldPosition, float3 worldNormal, float4 diffuseColor, float3 EyePosition)
{
	
	float NDotL = dot(SUNNY_Light.position, worldNormal);
	float3 finalColor = SUNNY_Light.color.rgb * saturate(NDotL);

																
	float3 ToEye = EyePosition.xyz - worldPosition;
	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + SUNNY_Light.position);
	float NDotH = saturate(dot(HalfWay, worldNormal));
	finalColor += SUNNY_Light.color.rgb * pow(NDotH, 250.0f) * SUNNY_Light.intensity;

	return finalColor * diffuseColor.rgb;
}


Texture2D textures : register(t0);
Texture2D shadowMap: register(t2);//추가
SamplerState samplers : register(s0);

float4 PSMain(in VSOutput input) : SV_TARGET
{
	float3 normal = normalize(input.normal);

	float4 texColor = (float4)SUNNY_Color;

	if (SUNNY_HasTexture >= 1)
	{
		texColor = textures.Sample(samplers, input.uv);
	}

	texColor.rgb *= texColor.rgb; // 더 진해진다.

	float3 finalColor = CalcAmbient(normal, texColor.rgb);
	finalColor += CalcDirectional(input.position, normal, texColor, input.cameraPosition);
	//-----------------------------추가------------------------------------------------
	float shadowDepth = tex2D(ShadowSampler, uv).r;
	float2 shadowMapUV = input.lightViewPosition.xy / input.lightViewPosition.w;
	shadowMapUV.y = -shadowMapUV.y;
	shadowMapUV = shadowMapUV * 0.5 + 0.5;

	float shadowDepth = tex2D(samplers, shadowMapUV).r;
	float currentDepth = input.position.z / input.position / w;

	if (currentDepth > shadowDepth + 0.000125f)
	{
		finalColor.rgb *= 0.5f;
	}
	//------------------------------추가------------------------------------------------
	return float4(finalColor, texColor.a);
}