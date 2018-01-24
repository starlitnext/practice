#ifndef _D3D_APP_H_
#define _D3D_APP_H_

#include <windows.h>
#include <string>

#include "Common.h"
#include "GameTimer.h"
#include "D3DHeaders.h"

NS_D3DAPP_BEGIN

class D3D_DLL D3DApp
{
public:

	// This is just used to forward windows messages from a global window
	// procedure to our member function window procedure because we cannot
	// assign a member function to WNDCLASS::lpfnWndProc.
	static D3DApp*	gd3dApp;
	static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	D3DApp();
	virtual ~D3DApp();

	HINSTANCE AppInst() const;
	HWND MainWnd() const;
	float AspectRatio() const;

	int Run();

	// Framework methods. Derived client class overrides these
	// methods to implement special application requirements.

	virtual bool Init();
	virtual void OnResize();
	virtual void UpdateScene(float dt) = 0;
	virtual void DrawScene() = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Convenience overrides for handling mouse input.
	virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
	virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y) { }

protected:
	bool InitMainWindow();
	bool InitDirect3D();

	void CalculateFrameStatus();

protected:
	HINSTANCE			mhAppInst;			// Application instance handle
	HWND				mhMainWnd;			// main window handle
	bool				mAppPaused;			// is the application paused
	bool				mMinimized;			// is the application minimized
	bool				mMaximized;			// is the application maximized
	bool				mResizing;			// are the resize bars being dragged
	UINT				m4xMsaaQuality;		// quality level of 4X MSAA

	// Used to keep track of the "delta-time" and game time
	GameTimer			mTimer;

	// the D3D11 device, the swap chain for page flipping,
	// the 2D texture for the depth/stencil buffer,
	// the render target and depth/stencil views,
	// and the viewport.
	ID3D11Device*				md3dDevice;
	ID3D11DeviceContext*		md3dImmediateContext;
	IDXGISwapChain*				mSwapChain;
	ID3D11Texture2D*			mDepthStencilBuffer;
	ID3D11RenderTargetView*		mRenderTargetView;
	ID3D11DepthStencilView*		mDepthStencilView;
	D3D11_VIEWPORT				mScreenViewport;

	// The following variables are initialized in the D3DApp constructor
	// to default values. However, you can override the values in the 
	// derived class constructor to pick difference defaults.

	// Window title/caption. D3DApp defaults to "D3D11 Application"
	std::wstring				mMainWndCaption;

	// Hardware device or reference device? D3DApp defaults to
	// D3D_DRIVER_TYPE_HARDWARE.
	D3D_DRIVER_TYPE				md3dDriverType;

	// Initial size of the window's client area. D3DApp defaults to
	// 800x600. Note, however, that these values change at runtime
	// to reflect the current client area size as the window is resized.
	int							mClientWidth;
	int							mClientHeight;

	// True to use 4X MSAA. The default is false.
	bool						mEnable4xMsaa;
};

NS_D3DAPP_END

#endif // _D3D_APP_H_
