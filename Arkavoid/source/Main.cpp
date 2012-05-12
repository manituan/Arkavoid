#include "Main.h"
#include "MainState.h"

D3DCore* d3dcore;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	d3dcore = CreateD3DCore();

	d3dcore->System_SetState( SSTATE_TITLE, L"Arkavoid" );
	d3dcore->System_SetState( SSTATE_SPRITE, true );
	//d3dcore->System_SetState( SSTATE_ICON, MAKEINTRESOURCE(IDI_APP) );
	if( d3dcore->System_Init() )
	{
		
		d3dcore->GState_Push( new MainState );
		d3dcore->System_Run();

	}
	else
		MessageBox(NULL, L"ERROR AL CREAR EL CORE!!", L"ERROR!!!", MB_ICONEXCLAMATION);
	
	d3dcore->System_Shutdown();

	ReleaseD3DCore();
}