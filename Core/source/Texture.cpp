#include "D3DCore_impl.h"

//Textures Methods
HTexture* D3DCore_Impl::Texture_Load( LPCWSTR FileName )
{
	ID3D10Texture2D* texture2D = NULL;
	ID3D10Resource* pD3D10Resource = NULL;

	// Loads the texture into a temporary ID3D10Resource object
	HR( D3DX10CreateTextureFromFile(md3dDevice, LPCWSTR(FileName),
									NULL, NULL, &pD3D10Resource, NULL) );

	// Translates the ID3D10Resource object into a ID3D10Texture2D object
	HR( pD3D10Resource ->QueryInterface(__uuidof( ID3D10Texture2D), (LPVOID*)&texture2D) );
	HR( pD3D10Resource ->Release() );

	mTextureList.push_back( texture2D );

	// returns the ID3D10Texture2D object
	return texture2D;
}

void D3DCore_Impl::Texture_Free( HTexture* Texture )
{
	mTextureList.remove( Texture );
	ReleaseCOM( Texture );
}

unsigned D3DCore_Impl::Texture_Width( HTexture* texture )
{
	if(texture == NULL ) return NULL;
	
	// Get the texture details
	D3D10_TEXTURE2D_DESC desc;
	texture->GetDesc( &desc );
	return desc.Width;
}

unsigned D3DCore_Impl::Texture_Height( HTexture* texture )
{
	if(texture == NULL ) return NULL;
	
	// Get the texture details
	D3D10_TEXTURE2D_DESC desc;
	texture->GetDesc( &desc );
	return desc.Height;
}

HResource* D3DCore_Impl::Resource_Load( LPCWSTR FileName )
{
	ID3D10ShaderResourceView *spriteTextureRV = NULL;

	D3DX10_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory( &loadInfo, sizeof(D3DX10_IMAGE_LOAD_INFO) );
	loadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	loadInfo.Format = DXGI_FORMAT_BC1_UNORM;

	ID3D10ShaderResourceView *pSRView = NULL;
	HR( D3DX10CreateShaderResourceViewFromFile( md3dDevice, FileName, &loadInfo, NULL, &spriteTextureRV, NULL ) );

	mResourceList.push_back( spriteTextureRV );

	return spriteTextureRV;
}

HResource* D3DCore_Impl::Resource_Create( HTexture* Texture )
{
	if(Texture == NULL ) return NULL;
	
	// Get the texture details
	D3D10_TEXTURE2D_DESC desc;
	Texture->GetDesc( &desc );
	
	// Create a shader resource view of the texture
	D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	// Clear out the shader resource view description structure
	ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
	// Set the texture format
	SRVDesc.Format = desc.Format;
	// Set the type of resource
	SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = desc.MipLevels;

	ID3D10ShaderResourceView *spriteTextureRV = NULL;
	HR( md3dDevice->CreateShaderResourceView(Texture, &SRVDesc, &spriteTextureRV) );

	mResourceList.push_back( spriteTextureRV );
	return spriteTextureRV;
}

void D3DCore_Impl::Resource_Free( HResource* resource )
{
	mResourceList.remove( resource );
	ReleaseCOM(resource);
}