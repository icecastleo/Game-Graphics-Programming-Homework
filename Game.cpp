#include "Game.h"
#include "Vertex.h"
#include <iostream>
#include <typeinfo>
#include <math.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore( 
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	camera = new Camera();

	prevMousePos.x = MINLONG32;

	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	//// Release meshes
	for (auto mesh : meshes) {
		delete mesh;
	}
	//meshes.clear();

	for (auto entity : entities) {
		delete entity;
	}

	delete camera;
	delete material;
	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	light.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	light.DiffuseColor = XMFLOAT4(0, 0, 1, 1);
	light.Direction = XMFLOAT3(1, -1, 0);

	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	material = new Material(vertexShader, pixelShader);

	//CreateMatrices();
	camera->setAspectRatio((float)width / height);
	
	CreateBasicGeometry();

	//Entity *e1 = new Entity(meshes[0], material);
	//e1->setPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//entities.push_back(e1);

	//Entity *e2 = new Entity(meshes[0], material);
	//e2->setPosition(XMFLOAT3(3.0f, 0.0f, 0.0f));
	//entities.push_back(e2);

	//Entity *e3 = new Entity(meshes[1], material);
	//e3->setPosition(XMFLOAT3(0.0f, 2.0f, 0.0f));
	//entities.push_back(e3);

	//Entity *e4 = new Entity(meshes[2], material);
	//e4->setPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//entities.push_back(e4);

	//Entity *e5 = new Entity(meshes[2], material);
	//e5->setPosition(XMFLOAT3(3.0f, 0.0f, 0.0f));
	//entities.push_back(e5);

	Entity *e1 = new Entity(meshes[0], material);
	e1->setPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	entities.push_back(e1);

	Entity *e2 = new Entity(meshes[1], material);
	e2->setPosition(XMFLOAT3(3.0f, 0.0f, 0.0f));
	entities.push_back(e2);

	Entity *e3 = new Entity(meshes[2], material);
	e3->setPosition(XMFLOAT3(-3.0f, 0.0f, 0.0f));
	entities.push_back(e3);

	Entity *e4 = new Entity(meshes[3], material);
	e4->setPosition(XMFLOAT3(0.0f, 2.0f, 0.0f));
	entities.push_back(e4);

	Entity *e5 = new Entity(meshes[4], material);
	e5->setPosition(XMFLOAT3(3.0f, 2.0f, 0.0f));
	entities.push_back(e5);

	Entity *e6 = new Entity(meshes[5], material);
	e6->setPosition(XMFLOAT3(-3.0f, 2.0f, 0.0f));
	entities.push_back(e6);

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");		

	pixelShader = new SimplePixelShader(device, context);
	if(!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))	
		pixelShader->LoadShaderFile(L"PixelShader.cso");

	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
//void Game::CreateMatrices()
//{
//	//// Create the View matrix
//	//// - In an actual game, recreate this matrix every time the camera 
//	////    moves (potentially every frame)
//	//// - We're using the LOOK TO function, which takes the position of the
//	////    camera and the direction vector along which to look (as well as "up")
//	//// - Another option is the LOOK AT function, to look towards a specific
//	////    point in 3D space
//	//XMVECTOR pos = XMVectorSet(1.5f, 0, -10, 0);
//	//XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
//	//XMVECTOR up  = XMVectorSet(0, 1, 0, 0);
//	//XMMATRIX V   = XMMatrixLookToLH(
//	//	pos,     // The position of the "camera"
//	//	dir,     // Direction the camera is looking
//	//	up);     // "Up" direction in 3D space (prevents roll)
//	//XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!
//	
//	// Create the Projection matrix
//	// - This should match the window's aspect ratio, and also update anytime
//	//   the window resizes (which is already happening in OnResize() below)
//	//XMMATRIX P = XMMatrixPerspectiveFovLH(
//	//	0.25f * 3.1415926535f,		// Field of View Angle
//	//	(float)width / height,		// Aspect ratio
//	//	0.1f,						// Near clip plane distance
//	//	100.0f);					// Far clip plane distance
//	//XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
//}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	//// Create some temporary variables to represent colors
	//// - Not necessary, just makes things more readable
	//XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	//XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	//// Set up the vertices of the triangle we would like to draw
	//// - We're going to copy this array, exactly as it exists in memory
	////    over to a DirectX-controlled data structure (the vertex buffer)
	//Vertex vertices[] = 
	//{
	//	{ XMFLOAT3(+0.0f, +1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },
	//	{ XMFLOAT3(+1.5f, -1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },
	//	{ XMFLOAT3(-1.5f, -1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },
	//};

	//// Set up the indices, which tell us which vertices to use and in which order
	//// - This is somewhat redundant for just 3 vertices (it's a simple example)
	//// - Indices are technically not required if the vertices are in the buffer 
	////    in the correct order and each one will be used exactly once
	//// - But just to see how it's done...
	//UINT indices[] = { 0, 1, 2 };

	//meshes.push_back(new Mesh(vertices, 3, indices, 3, device));

	//// Additional meshes
	//Vertex vertices2[] =
	//{
	//	{ XMFLOAT3(+3.0f, +1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },
	//	{ XMFLOAT3(+5.0f, +1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },
	//	{ XMFLOAT3(+5.0f, -1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },
	//	{ XMFLOAT3(+3.0f, -1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },
	//};

	//UINT indices2[] = { 0, 1, 2, 0, 2, 3 };

	//meshes.push_back(new Mesh(vertices2, 4, indices2, 6, device));

	//Vertex vertices3[] =
	//{
	//	{ XMFLOAT3(-1.5f, -1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },
	//	{ XMFLOAT3(-3.0f, +1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },
	//	{ XMFLOAT3(+0.0f, +1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },
	//};

	//UINT indices3[] = { 0, 1, 2 };

	//meshes.push_back(new Mesh(vertices3, 3, indices3, 3, device));

	meshes.push_back(new Mesh("Assets/Models/cone.obj", device));
	meshes.push_back(new Mesh("Assets/Models/cube.obj", device));
	meshes.push_back(new Mesh("Assets/Models/cylinder.obj", device));
	meshes.push_back(new Mesh("Assets/Models/helix.obj", device));
	meshes.push_back(new Mesh("Assets/Models/sphere.obj", device));
	meshes.push_back(new Mesh("Assets/Models/torus.obj", device));
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	camera->setAspectRatio((float)width / height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	camera->update(deltaTime, totalTime);

	for (auto &entity : entities) {
		//entity->Move(sin(totalTime * 5) / 500, 0.0f, 0.0f);
		//entity->setScale(XMFLOAT3(1.0f, (sin(totalTime) + 2) / 3, 1.0f));
		entity->setRotation(XMFLOAT3(totalTime, totalTime, 0.0f));
		entity->Update();
	}

	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	for (auto &entity : entities) {

		// FIXME: or make entity->material to public?
		material->getPixelShader()->SetData(
			"light",  // The name of the (eventual) variable in the shader
			&light,   // The address of the data to copy
			sizeof(DirectionalLight)); // The size of the data to copy

		// shaders
		entity->PrepareMaterial(camera->getViewMatrix(), camera->getProjectionMatrix());

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		ID3D11Buffer *vertexBuffer = entity->getMesh()->GetVertexBuffer();

		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(entity->getMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(
			entity->getMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);
	}

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if(prevMousePos.x != MINLONG32)
		camera->rotateByMouse(x - prevMousePos.x, y - prevMousePos.y);

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion