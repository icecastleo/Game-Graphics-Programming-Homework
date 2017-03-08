#pragma once

#include <wrl.h>

class SimpleVertexShader;
class SimplePixelShader;
class ID3D11ShaderResourceView;
class ID3D11SamplerState;

class Material
{
public:
	Material(SimpleVertexShader *vertexShader, SimplePixelShader *pixelShader, ID3D11ShaderResourceView *srv, ID3D11SamplerState *samplerState);
	~Material();

	SimpleVertexShader* getVertexShader();
	SimplePixelShader* getPixelShader();
	ID3D11ShaderResourceView* getShaderResourceView();
	ID3D11SamplerState* getSamplerState();

private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* samplerState;
};

