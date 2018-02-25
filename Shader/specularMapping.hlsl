////////////////////////////////////VERTEX SHADER//////////////////////////////////////////////
float4x4 gWorldViewProjectionMatrix;
float4x4 gWorldInvMatrix;


float4 gWorldLightPosition;
float4 gWorldCameraPosition;


struct VS_INPUT
{
   float4 mPosition : POSITION;
   float3 mNormal: NORMAL;
   float2 mUV:TEXCOORD0;
};


struct VS_OUTPUT
{
   float4 mPosition : POSITION;
   float2 mUV:TEXCOORD0;
   float3 mDiffuse : TEXCOORD1;
   float3 mViewDir: TEXCOORD2;
   float3 mReflection: TEXCOORD3;
};


VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;


   Output.mPosition = mul( Input.mPosition, gWorldViewProjectionMatrix );


   float3 localLightPosition = mul(gWorldLightPosition, gWorldInvMatrix);

   float3 lightDir = Output.mPosition.xyz - localLightPosition.xyz;
   lightDir = normalize(lightDir);

   float3 viewDir = normalize(Output.mPosition.xyz - gWorldCameraPosition.xyz);
   Output.mViewDir = viewDir;


   Output.mDiffuse = dot(-lightDir, Input.mNormal);
   Output.mReflection = reflect(lightDir, Input.mNormal);
   Output.mUV = Input.mUV;

   return Output;
}

///////////////////////////////PIXEL SHADER////////////////////////////////////////////
struct PS_INPUT
{
   float2 mUV : TEXCOORD0;
   float3 mDiffuse : TEXCOORD1;
   float3 mViewDir: TEXCOORD2;
   float3 mReflection: TEXCOORD3;
};

sampler2D DiffuseSampler;
sampler2D SpecularSampler;

float3 gLightColor;

float4 ps_main(PS_INPUT Input) : COLOR
{
   float4 albedo = tex2D(DiffuseSampler, Input.mUV);
   float3 diffuse = gLightColor * albedo.rgb * saturate(Input.mDiffuse);
   
   float3 reflection = normalize(Input.mReflection);
   float3 viewDir = normalize(Input.mViewDir); 
   float3 specular = 0;
   if ( diffuse.x > 0 )
   {
      specular = saturate(dot(reflection, -viewDir ));
      specular = pow(specular, 20.0f);
      
      float4 specularIntensity  = tex2D(SpecularSampler, Input.mUV);
      specular *= specularIntensity.rgb * gLightColor;
   }

   float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;
   
   return float4(ambient + diffuse + specular, 1);
}