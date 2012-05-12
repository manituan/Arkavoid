#include "D3DCore_impl.h"

//Sprite Methods
void D3DCore_Impl::G2d_Begin()
{
	HR( mSprite->Begin(D3DX10_SPRITE_SORT_TEXTURE | D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT) );
}

void D3DCore_Impl::G2d_End()
{
	HR( mSprite->Flush() );
	HR( mSprite->End() );
}

void D3DCore_Impl::G2d_Flush()
{
	HR( mSprite->Flush() );
}

void D3DCore_Impl::G2d_SetView( float x, float y )
{	
	D3DXMATRIX matView;
	D3DXMatrixTranslation(&matView, -x, -y, 0.0f);
	HR( mSprite->SetViewTransform( &matView) );
}

void D3DCore_Impl::G2d_Draw( D3DX10_SPRITE* buffer, unsigned size)
{
	HR( mSprite->DrawSpritesBuffered( buffer, size ) );
}