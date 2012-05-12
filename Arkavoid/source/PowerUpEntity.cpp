#include "PowerUpEntity.h"
#include "BarraEntity.h"
#include "EntityManager.h"
#include "D3DCore.h"
int PowerUpEntity::PowerUp_ID = -1;

PowerUpEntity::PowerUpEntity(float x, float y):BaseGameEntity( 
	BaseGameEntity::GetNextValidID() ),
	mFSM(this)

	//da problemas ese mFSM (this)
{
	PowerUp_ID = ID();
	SetEntityType( PowerUp_TYPE );
	
	mPowerUp_tipo=d3dcore->Random_Int (0,3);
	
	mPowerUp = d3dcore->Resource_Load( L"img\\alienship.png" );
	
	mPowerUpPos=D3DXVECTOR3 (x,y,0.1f);
	//Use antes un random, pero salían dos naves cada vez.

	mPowerUpSpeed = 50.0f;
	mPowerUpRadio = 10.0f;

	PowerUpSprite.pTexture = mPowerUp;
	PowerUpSprite.TexCoord.x = 0;
	PowerUpSprite.TexCoord.y = 0;
	PowerUpSprite.TexSize.x = 1.0f;
	PowerUpSprite.TexSize.y = 1.0f;
	PowerUpSprite.TextureIndex = 0;
	PowerUpSprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


}
	


	PowerUpEntity::~PowerUpEntity()
	{
	d3dcore->Resource_Free( mPowerUp );
	}

void PowerUpEntity::Update( float delta )
{

	mFSM.Update();
	setPowerUpPrevPos (getPowerUpPos());
	

		//mPowerUpDir es un vector 2d de valores 1 y -1
		//necesito otra variable que cambie el angulo del vector mPowerUpPos.
		
		mPowerUpPos.x+= 0.0f; 
		mPowerUpPos.y+= -1.0f * mPowerUpSpeed * delta;
		
			Colision();
	
		
	if (mPowerUpPos.y<-400.0f) 
		{
			
			EntityManager::Instance()->Remove(this);
			return;
		}
		

	D3DXMATRIX T, S;
	D3DXMatrixScaling(&S, 60.0f, 60.0f, 0.0f);
	D3DXMatrixTranslation(&T, mPowerUpPos.x, mPowerUpPos.y, 0.1f);	
	PowerUpSprite.matWorld = (S*T);
}

void PowerUpEntity::Colision()
{
	std::vector<BaseGameEntity*>& Barras = EntityManager::Instance()->GetType( Barra_TYPE );
	for( unsigned idx = 0; idx < Barras.size(); idx++)
	{

		D3DXVECTOR3 BarraPos= ((BarraEntity *)Barras[idx])->getPosition();
		float Size=((BarraEntity *)Barras[idx])->getSize();
		float DistanceX = abs(BarraPos.x-mPowerUpPos.x) ;
		float DistanceY = abs(BarraPos.y-mPowerUpPos.y);
		
		//cambiar esto para hacerlo sin valor absoluto.
		if(DistanceY<20.0f)
		{
			
			if(DistanceX<(Size))
			{
				/*std::vector<BaseGameEntity*>& powerup = EntityManager::Instance()->GetType( PowerUp_TYPE );
				
				for( idx = 0; idx < powerup.size(); idx++)
					{
						std::vector<BaseGameEntity*>& barras = EntityManager::Instance()->GetType( Barra_TYPE );
						((BarraEntity *)barras[idx])->mFSM.SetCurrentState());
						
					}
				*/
				EntityManager::Instance()->Remove(this);
				//Aquí cambiaría el estado de la barra con un random de entre varios estados.
				//Anda, podría pasarle un int y allí haría un switch en update según el int que sea.
				//BarraEntity::mFSM; 
				return;

			}

			
		}
					
	} 

			
		
		


}

void PowerUpEntity::Render()
{
	d3dcore->G2d_Draw( &PowerUpSprite, 1 );
}

float PowerUpEntity::getSpeed()
{
	return mPowerUpSpeed;
}
void  PowerUpEntity::setSpeed(float speed)
{
	mPowerUpSpeed=speed;
}