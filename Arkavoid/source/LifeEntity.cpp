
#include "Main.h"
#include "EntityManager.h"
#include "LifeEntity.h"




LifeEntity::LifeEntity(float x, float y):BaseGameEntity( 
	BaseGameEntity::GetNextValidID() ),
	mFSM(this)

	//da problemas ese mFSM (this)
{
	
	SetEntityType( Life_TYPE );

	mLife = d3dcore->Resource_Load( L"img\\paddle.png" );
	mLifePos = D3DXVECTOR3( x, y, 0.1f ); 

	//simplificar esta clase para que deje de ser una entidad y solo sirva para pintarse.

	LifeSprite.pTexture = mLife;
	LifeSprite.TexCoord.x = 0;
	LifeSprite.TexCoord.y = 0;
	LifeSprite.TexSize.x = 1.0f;
	LifeSprite.TexSize.y = 1.0f;
	LifeSprite.TextureIndex = 0;
	LifeSprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);



}

LifeEntity::~LifeEntity()
{
	d3dcore->Resource_Free( mLife );
	
}

void LifeEntity::Update( float delta )
{

	mFSM.Update();


	D3DXMATRIX T, S;
	D3DXMatrixScaling(&S, 20.0f, 20.0f, 0.0f);
	D3DXMatrixTranslation(&T, mLifePos.x, mLifePos.y, 0.4f);	
	LifeSprite.matWorld = (S*T);



};

void LifeEntity::Render()
{
	d3dcore->G2d_Draw( &LifeSprite, 1 );
}