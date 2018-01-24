
#include "InitDirect3DApp.h"

USING_NS_D3DAPP;

InitDirect3dApp::InitDirect3dApp()
{

}

InitDirect3dApp::~InitDirect3dApp()
{

}

bool InitDirect3dApp::Init()
{
	if (!D3DApp::Init())
		return false;
	return true;
}

void InitDirect3dApp::OnResize()
{
	D3DApp::OnResize();
}

void InitDirect3dApp::UpdateScene(float dt)
{

}

void InitDirect3dApp::DrawScene()
{
	assert(md3dImmediateContext);
	assert(mSwapChain);

	// Clear the back buffer blue.
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView,
		reinterpret_cast<const float*>(&Colors::Blue));

	// Clear the depth buffer to 1.0f and the stencil buffer to 0.
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Present the back buffer to the screen.
	HR(mSwapChain->Present(0, 0));
}

int main()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	InitDirect3dApp app;
	if (!app.Init())
		return 0;

	return app.Run();
}