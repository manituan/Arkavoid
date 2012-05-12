#include "Main.h"
#include "MainState.h"
#include "EntityManager.h"
#include "BarraEntity.h"
#include "BrickEntity.h"
#include "BolaEntity.h"
#include "PowerUpEntity.h"
#include "D3DCore.h"
#include <iostream>
using namespace std;

int MainState::Lifes_Count=0;

MainState::MainState()
{
	mFondo = NULL;
	mBarra = NULL;
	AlphaOn=NULL;
	m_Sound=0;
	Lifes_Count=3;
}
void MainState::initialize()
{
	mLife1 = d3dcore->Resource_Load( L"img\\paddle.png");
	mLife2 = d3dcore->Resource_Load( L"img\\paddle.png" );
	mLife3 = d3dcore->Resource_Load( L"img\\paddle.png");

		Life1Sprite[0].pTexture = mLife1;
		Life1Sprite[0].TexCoord.x = 0.0f;
		Life1Sprite[0].TexCoord.y = 0.0f;
		Life1Sprite[0].TexSize.x = 1.0f;
		Life1Sprite[0].TexSize.y = 1.0f;
		Life1Sprite[0].TextureIndex = 0;
		Life1Sprite[0].ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		Life2Sprite[0].pTexture = mLife2;
		Life2Sprite[0].TexCoord.x = 0.0f;
		Life2Sprite[0].TexCoord.y = 0.0f;
		Life2Sprite[0].TexSize.x = 1.0f;
		Life2Sprite[0].TexSize.y = 1.0f;
		Life2Sprite[0].TextureIndex = 0;
		Life2Sprite[0].ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		Life3Sprite[0].pTexture = mLife3;
		Life3Sprite[0].TexCoord.x = 0.0f;
		Life3Sprite[0].TexCoord.y = 0.0f;
		Life3Sprite[0].TexSize.x = 1.0f;
		Life3Sprite[0].TexSize.y = 1.0f;
		Life3Sprite[0].TextureIndex = 0;
		Life3Sprite[0].ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXMATRIX A, B,C,D;
	D3DXMatrixScaling(&D, 20.0f, 20.0f, 0.0f);
	D3DXMatrixTranslation(&A, 400.0f, 300.0f, 0.4f);	
	Life1Sprite[0].matWorld = (D*A);
	
	D3DXMatrixTranslation(&B, 440.0f, 300.0f, 0.4f);	
	Life2Sprite[0].matWorld = (D*B);

	D3DXMatrixTranslation(&C, 480.0f, 300.0f, 0.4f);	
	Life3Sprite[0].matWorld = (D*C);


	mFondo = d3dcore->Resource_Load( L"img\\bkgd1.bmp" );

	//mMusic = d3dcore_Impl.Effect_Load(L"audio\\alex.wav");

		BkgdSprite[0].pTexture = mFondo;
		BkgdSprite[0].TexCoord.x = 0.0f;
		BkgdSprite[0].TexCoord.y = 0.0f;
		BkgdSprite[0].TexSize.x = 1.0f;
		BkgdSprite[0].TexSize.y = 1.0f;
		BkgdSprite[0].TextureIndex = 0;
		BkgdSprite[0].ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	D3DXMATRIX T, S;
	D3DXMatrixScaling(&S, 500.0f, 800.0f, 0.0f);
	D3DXMatrixTranslation(&T, 0.0f, 0.0f, 10.0f);	
	BkgdSprite[0].matWorld = (S*T);	

	

	m_Sound = new SoundClass;
	
	//m_Sound->Initialize(d3dcore->System_GetState(hwnd));

	// Initialize the text object.
	
	
	mBorde = d3dcore->Resource_Load( L"img\\Borde.png" );
	

		BordeSprite[0].pTexture = mBorde;
		BordeSprite[0].TexCoord.x = 0.0f;
		BordeSprite[0].TexCoord.y = 0.0f;
		BordeSprite[0].TexSize.x = 1.0f;
		BordeSprite[0].TexSize.y = 1.0f;
		BordeSprite[0].TextureIndex = 0;
		BordeSprite[0].ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	D3DXMATRIX U,V;
	D3DXMatrixScaling(&V, 600.0f, 800.0f, 0.0f);
	D3DXMatrixTranslation(&U, 0.0f, 0.0f, 9.0f);	
	BordeSprite[0].matWorld = (V*U);	

	EntityManager::Instance()->Add( new BarraEntity () );

	float countx=-150.0f;
	float county=100.0f;
	for (int i=0;i<6;i++)
	{
		
		for (int j=0;j<6;j++)
		{
			EntityManager::Instance()->Add(new BrickEntity(i*60+countx,j*30+county, true));
			
		}
		

	}


	D3D10_BLEND_DESC bd;
	ZeroMemory( &bd, sizeof( D3D10_BLEND_DESC) );
	bd.BlendEnable[0] = TRUE;
	bd.SrcBlend = D3D10_BLEND_SRC_ALPHA;	
	bd.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	bd.BlendOp = D3D10_BLEND_OP_ADD;

	bd.SrcBlendAlpha = D3D10_BLEND_ZERO;
	bd.DestBlendAlpha = D3D10_BLEND_ZERO;
	bd.BlendOpAlpha = D3D10_BLEND_OP_ADD;

	bd.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
	AlphaOn=d3dcore->Gfx_CreateBlendMode(&bd);

	

}

void MainState::shutdown()
{
	d3dcore->Resource_Free( mFondo );
	d3dcore->Resource_Free(mBorde);
	d3dcore->Resource_Free(mLife1);
	d3dcore->Resource_Free(mLife2);
	d3dcore->Resource_Free(mLife3);
	EntityManager::Instance()->RemoveType(2);
	EntityManager::Instance()->Remove( BarraEntity::Barra_ID );
	EntityManager::Instance()->Remove(BrickEntity::Brick_ID);
	EntityManager::Instance()->Remove(BolaEntity::Bola_ID);
	EntityManager::Instance()->Remove(PowerUpEntity::PowerUp_ID);
	if(m_Sound)
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}
}

bool MainState::update( float delta )
{
	
	if( d3dcore->Input_keyDown( DIK_ESCAPE ) )
		return false;
	
	if(BrickEntity::Brick_Count==0)
	{
		return false;
	}
	if (Lifes_Count==0)
	{
		return false;
	}

	EntityManager::Instance()->Update(delta);

	
	return true;
}

void MainState::render()
{
	d3dcore->Gfx_Begin();
	d3dcore->Gfx_Clear( 0xFF0000FF );

	d3dcore->G2d_Begin();
	d3dcore->Gfx_SetBlendMode(AlphaOn,0);
	d3dcore->G2d_Draw(BordeSprite,1);
	d3dcore->G2d_Draw( BkgdSprite, 1 );

	EntityManager::Instance()->Render();

	if(Lifes_Count==3)
	{
		d3dcore->G2d_Draw(Life1Sprite);
		d3dcore->G2d_Draw(Life2Sprite);
		d3dcore->G2d_Draw(Life3Sprite);
	}

		if(Lifes_Count==2)
	{
		d3dcore->G2d_Draw(Life1Sprite);
		d3dcore->G2d_Draw(Life2Sprite);
	}

		if(Lifes_Count==1)
	{
		d3dcore->G2d_Draw(Life1Sprite);
	}
	d3dcore->G2d_End();

	d3dcore->Gfx_End();
}