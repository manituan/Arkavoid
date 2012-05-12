
#include "BarraEntity.h"
#include "Main.h"
#include "BarraState.h"
#include "EntityManager.h"
#include "BolaEntity.h"



int BarraEntity::Barra_ID = -1;

BarraEntity::BarraEntity():BaseGameEntity( 
	BaseGameEntity::GetNextValidID() ),
	mFSM(this)

	//da problemas ese mFSM (this)
{
	Barra_ID = ID();
	SetEntityType( Barra_TYPE );

	mBarra = d3dcore->Resource_Load( L"img\\paddle.png" );

	mBarraPos = D3DXVECTOR3( 0.0f, -300.0f, 0.1f ); 

	mBarraSpeed = 20.0f;

	setSize(60.0f);

	BarraSprite.pTexture = mBarra;
	BarraSprite.TexCoord.x = 0;
	BarraSprite.TexCoord.y = 0;
	BarraSprite.TexSize.x = 1.0f;
	BarraSprite.TexSize.y = 1.0f;
	BarraSprite.TextureIndex = 0;
	BarraSprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	mFSM.SetCurrentState(ControledByHuman::Instance());
	// estados de la entidad. La pelota por ejemplo, si conseguimos una poción, iría al doble de su velocidad... por ejemplo


}

BarraEntity::~BarraEntity()
{
	//d3dcore->Resource_Free( mBarra );
}

void BarraEntity::Update( float delta )
{

	mFSM.Update();

	// Update position and speed based on time.
	mBarraPos.x += mDir * mBarraSpeed;
	
	//Debería hacer una variable global o llamar a clientWith y demás para saber qué tamaño tiene la ventana y hacer los movimientos de la barra
	//acordes a ella.
	
	D3DXMATRIX T, S;
	D3DXMatrixScaling(&S, getSize()+20.0f, 20.0f, 0.0f);
	D3DXMatrixTranslation(&T, mBarraPos.x, mBarraPos.y, 0.4f);	
	BarraSprite.matWorld = (S*T);
	
	//Camera Update
	mCameraPos.x = 0.0f;
	mCameraPos.y=  0.0f;

	d3dcore->G2d_SetView(mCameraPos.x,mCameraPos.y);

	Colision();
	
};


void BarraEntity::Colision()
{
	
	std::vector<BaseGameEntity*>& Bolas = EntityManager::Instance()->GetType( Bola_TYPE );
	for( unsigned idx = 0; idx < Bolas.size(); idx++)
	{

		D3DXVECTOR3 BolaPos= ((BolaEntity *)Bolas[idx])->getBolaPos();
		D3DXVECTOR3 Prev= ((BolaEntity *)Bolas[idx])->getBolaPrevPos();	

		float DistanceX = abs(BolaPos.x-mBarraPos.x) ;
		float DistanceY = abs(BolaPos.y-mBarraPos.y);
		
		//cambiar esto para hacerlo sin valor absoluto.
		if(DistanceY<((BolaEntity *)Bolas[idx])->getmBolaRadio())
		{
			
			if(DistanceX<(getSize()/6))
			{
				
				if(( Prev.x<BolaPos.x) & ( Prev.y > BolaPos.y))
				{
				// Pared izq desde abajo o brick por la derecha desde abajo.
				((BolaEntity *)Bolas[idx])->setmBolaDir(1.0f,1.0f);
			
		
				return;
				}

				if(( Prev.x>BolaPos.x) & ( Prev.y > BolaPos.y))
				{
				// Pared izq desde abajo o brick por la derecha desde abajo.
				((BolaEntity *)Bolas[idx])->setmBolaDir(-1.0f,1.0f);
		
				return;
				}

			}

			if((DistanceX>=(getSize()/6))&(DistanceX<=getSize()))//si la resta de las x en valor absoluto es < que getSize()/6 entonces
			{
			
				
				if(( Prev.x<BolaPos.x) & ( Prev.y > BolaPos.y))
				{
				// Pared izq desde abajo o brick por la derecha desde abajo.
				((BolaEntity *)Bolas[idx])->setmBolaDir(-1.5f,1.5f);
				
		
				return;
				}

				if(( Prev.x>BolaPos.x) & ( Prev.y > BolaPos.y))
				{
				// Pared izq desde abajo o brick por la derecha desde abajo.
				((BolaEntity *)Bolas[idx])->setmBolaDir(1.5f,1.5f);
		
				return;
				}
			}
		}		
	} 

		
}
void BarraEntity::Render()
{
	d3dcore->G2d_Draw( &BarraSprite, 1 );
}

float BarraEntity::getSpeed()
{
	return mBarraSpeed;
}
void  BarraEntity::setSpeed(float speed)
{
	mBarraSpeed=speed;
}




