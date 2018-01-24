#ifndef _COMMON_H_
#define _COMMON_H_

#define			NS_D3DAPP_BEGIN			namespace d3dapp {
#define			NS_D3DAPP_END			}
#define			USING_NS_D3DAPP			using namespace d3dapp

#if defined(_USERDLL) || defined(_WINDLL)
#define			D3D_DLL					_declspec(dllexport)
#else
#define			D3D_DLL					_declspec(dllimport)
#endif

#endif _COMMON_H_
