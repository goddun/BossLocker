/////////////////////////////////VERTEX SHADER//////////////////////////////////////

float4x4 gWorldViewProjectionMatrix;
float4x4 gWorldInverseMatrix;
float4 gWorldLightPosition;


struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 mNormal:NORMAL;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float3 Diffuse:TEXCOORD1;
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, gWorldViewProjectionMatrix );
   
   float3 gLocalLightPosition = mul(gWorldLightPosition,gWorldInverseMatrix);
   float3 lightDir = normalize(Input.Position.xyz - gLocalLightPosition);
   
   Output.Diffuse = dot(-lightDir,normalize(Input.mNormal));
   
   return( Output ); 
}
////////////////////////////PIXEL SHADER//////////////////////////////////////
float3 gSurfaceColor;


struct PS_INPUT
{
   float3 mDiffuse:TEXCOORD1;
};


float4 ps_main(PS_INPUT Input) : COLOR0
{
   
   float3 diffuse = saturate(Input.mDiffuse);
   
   diffuse = ceil(diffuse*5)/5.0f;
   
   return( float4(gSurfaceColor*diffuse.xyz,1 ) );
   
}