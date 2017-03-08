#include "Material.h"
#include "SimpleShader.h"

Material::Material(SimpleVertexShader * vertexShader, SimplePixelShader * pixelShader, ID3D11ShaderResourceView * srv, ID3D11SamplerState * samplerState)
	:vertexShader(vertexShader), pixelShader(pixelShader), srv(srv), samplerState(samplerState) {};

Material::~Material()
{
}

SimpleVertexShader * Material::getVertexShader()
{
	return vertexShader;
}

SimplePixelShader *Material::getPixelShader()
{
	return pixelShader;
}

ID3D11ShaderResourceView* Material::getShaderResourceView()
{
	return srv;
}

ID3D11SamplerState* Material::getSamplerState()
{
	return samplerState;
}
