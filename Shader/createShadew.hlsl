cbuffer MatrixBuffer
{
	float4x4 worldMatrix;
	float4x4 lightViewMatrix;
	float4x4 lightProjectionMatrix;
};


struct VertexInputType
{
	float4 position : POSITION;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 lightViewPosition : TEXCOORD1;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType vs_main(VertexInputType input)
{
	PixelInputType output;

	output.lightViewPosition = mul(input.position, worldMatrix);
	output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
	output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

	output.position = output.lightViewPosition;

	return output;
}


float4 ps_main(PixelInputType Input) : SV_TARGET
{
	float depth = Input.lightViewPosition.z / Input.lightViewPosition.w;
	return float4(depth.xxx, 1);
}