
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: WORLDPOS;
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
	float padding;
};

struct PointLight 
{
	float4 PointLightColor;
	float3 PointLightPosition;
	float padding;
	float3 CameraPosition;
};

cbuffer externalData : register(b0)
{
	DirectionalLight dirLight;
	PointLight  pointLight;
};

Texture2D diffuseTexture  : register(t0);
SamplerState basicSampler : register(s0);

float4 getDirLightColor(DirectionalLight light, VertexToPixel input) {
	float lightAmount = saturate(dot(input.normal, normalize(-light.Direction)));
	return lightAmount * light.DiffuseColor;
}

float4 getPointLightColor(PointLight light, VertexToPixel input) {
	float lightAmount = saturate(dot(input.normal, normalize(light.PointLightPosition - input.worldPos)));
	return lightAmount * light.PointLightColor;
}

// Specular highlight for point light
float getBlinnSpecular(PointLight light, VertexToPixel input) {
	float specular = saturate(dot(input.normal, normalize(light.PointLightPosition - input.worldPos + light.CameraPosition - input.worldPos)));
	return pow(specular, 8);
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering

	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	// Renormalize interpolated normals
	input.normal = normalize(input.normal);
	
	//return float4(input.normal, 1);
	//return float4(0.1, 0.1, 0.1, 1);

	float4 light = getDirLightColor(dirLight, input)
		+ dirLight.AmbientColor
		+ getPointLightColor(pointLight, input)
		+ getBlinnSpecular(pointLight, input);

	return surfaceColor * light;
	//return surfaceColor;
	//return light;
}