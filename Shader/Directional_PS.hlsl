

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////픽셀셰이더///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

cbuffer cbDirLightPS : register( b1 ) // Directional and ambient light constants
{
	float3 AmbientDown		: packoffset( c0 );
	float3 AmbientRange		: packoffset( c1 );
	float3 DirToLight		: packoffset( c2 );
	float3 DirLightColor	: packoffset( c3 );
}

float3 CalcAmbient(float3 normal, float3 diffuseColor)
{
	float up = normal.y * 0.5 + 0.5;//노말을 0~1범위로 변환 y축이 상하축과 일치한다고 가정

	float3 ambient = AmbientDown + up * AmbientRange;//엠비언트 값을 계산

	return ambient * diffuseColor;//엠비언트 값에 색을 곱한다
}

//방향광 계산
float3 CalcDirectional(float3 worldPosition, float3 worldNormal, float4 diffuseColor)
{
   // Phong diffuse
   float NDotL = dot(DirToLight, worldNormal);//노멀과 빛의 방향 벡터를 내적해 난반사광의 양을 구한다.
   float3 finalColor = DirLightColor.rgb * saturate(NDotL);//내적한 값의 0보다 작거나 1보다 큰 값은 필요없으므로 버리고
   
   // Blinn specular
   float3 ToEye = EyePosition.xyz - worldPosition;//카메라 위치와 월드 공간에서의 정점을 빼서 카메라 벡터를 구한다.
   ToEye = normalize(ToEye);//정규화
   float3 HalfWay = normalize(ToEye + DirToLight);//카메라벡터와 빛의 방향의 중간 벡터를 구하고 정규화
   float NDotH = saturate(dot(HalfWay, worldNormal));//중간벡터와 법선백터를 내적해서 코사인 값을 구하고 0~1범위로 만듬
   finalColor += DirLightColor.rgb * pow(NDotH, specExp) * specIntensity;//정반사광 계수로 퍼지는 정도를 정하고 세기를 곱해서 최종 정반사광의 양을 구함
   
   return finalColor * diffuseColor.rgb;//난반사광+정반사과 * 텍셀
}

float4 DirectionalLightPS(VS_OUTPUT In) : SV_TARGET0
{
	In.Normal = normalize(In.Normal);

	float4 diffuseColor = DiffuseTexture.Sample(LinearSampler, In.UV);//기본 텍스쳐의 텍셀을 얻어옴
	diffuseColor.rgb *= diffuseColor.rgb;//색을 제곱해서 선형공간으로 변환


	float3 finalColor = CalcAmbient(In.Normal, diffuseColor.rgb);

	// Calculate the directional light
	finalColor += CalcDirectional(In.WorldPos, In.Normal,diffuseColor);

	// Return the final color
	return float4(finalColor, 1.0);
}