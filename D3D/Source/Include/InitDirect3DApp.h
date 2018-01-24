#ifndef _INIT_DIRECT3D_APP_H_
#define _INIT_DIRECT3D_APP_H_

#include "D3Dapp.h"

class InitDirect3dApp : public d3dapp::D3DApp
{
public:
	InitDirect3dApp();
	~InitDirect3dApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();
};

#endif // _INIT_DIRECT3D_APP_H_
