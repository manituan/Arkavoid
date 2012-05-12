#include "D3DCore_impl.h"

//Graphics Methods
void D3DCore_Impl::Gfx_Begin()
{
}

void D3DCore_Impl::Gfx_End()
{
	mSwapChain->Present(0, 0);
}

void D3DCore_Impl::Gfx_Clear( DWORD Color )
{
	D3DXCOLOR mBGColor = D3DXCOLOR( Color );
	md3dDevice->ClearRenderTargetView(mRenderTargetView, mBGColor);
	md3dDevice->ClearDepthStencilView(mDepthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);
}

HBLENDSTATE* D3DCore_Impl::Gfx_CreateBlendMode( D3D10_BLEND_DESC* pBDesc )
{
	ID3D10BlendState *hBS;
	HR( md3dDevice->CreateBlendState( pBDesc, &hBS ) );

	mBlendStateList.push_back(hBS);
	return hBS;
}

void D3DCore_Impl::Gfx_FreeBlendMode( HBLENDSTATE* hBS)
{
	mBlendStateList.remove(hBS);
	ReleaseCOM( hBS );
}

void D3DCore_Impl::Gfx_SetBlendMode( HBLENDSTATE* hBS, const float* Factor )
{
	md3dDevice->OMSetBlendState( hBS, Factor, 0xFFFFFFFF );
}

bool D3DCore_Impl::initDirect3D()
{
	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width  = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// No multisampling.
	sd.SampleDesc.Count   = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = mhMainWnd;
	sd.Windowed     = true;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags        = 0;


	// Create the device.

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
    createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

	HR( D3D10CreateDeviceAndSwapChain(
			0,                 //default adapter
			md3dDriverType,
			0,                 // no software device
			createDeviceFlags, 
			D3D10_SDK_VERSION,
			&sd,
			&mSwapChain,
			&md3dDevice) );


	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the onResize method here to avoid code duplication.
	
	onResize();
	return true;
}

