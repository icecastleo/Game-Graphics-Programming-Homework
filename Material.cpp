#include "Material.h"

Material::Material(SimpleVertexShader * vertexShader, SimplePixelShader * pixelShader) :vertexShader(vertexShader), pixelShader(pixelShader) {};

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
