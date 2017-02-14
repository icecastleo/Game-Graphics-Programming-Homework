#pragma once

class SimpleVertexShader;
class SimplePixelShader;

class Material
{
public:
	Material(SimpleVertexShader *vertexShader, SimplePixelShader *pixelShader);
	~Material();

	SimpleVertexShader* getVertexShader();
	SimplePixelShader* getPixelShader();

private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
};

