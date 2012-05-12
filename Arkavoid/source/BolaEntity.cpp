#include "BolaEntity.h"
#include "Main.h"
#include "BolaState.h"
#include "BarraEntity.h"
#include "EntityManager.h"
#include "MainState.h"


int BolaEntity::Bola_ID = -1;

BolaEntity::BolaEntity():BaseGameEntity( 
	BaseGameEntity::GetNextValidID() ),
	mFSM(this)

	//da problemas ese mFSM (this)
{
	Bola_ID = ID();
	SetEntityType( Bola_TYPE );

	mBola = d3dcore->Resource_Load( L"img\\alienship.png" );
	
	
	//Use antes un random, pero salían dos naves cada vez.

	if(d3dcore->Random_Float(0.0f,10.0f)<5.0f)
	{
		mBolaDir.x=1.0f;
		mBolaDir.y=1.0f;
	}else
	{
		mBolaDir.x=-1.0f;
		mBolaDir.y=1.0f;
	}
		
	
	//mPos = getPosicion;
	
	std::vector<BaseGameEntity*>& Barras = EntityManager::Instance()->GetType( Barra_TYPE );
	
	mBolaPos = D3DXVECTOR3( ((BolaEntity *)Barras[0])->getBolaPos().x, ((BolaEntity *)Barras[0])->getBolaPos().y,0.1f);
	
	mBolaSpeed = 150.0f;
	mBolaRadio = 20.0f;

	BolaSprite.pTexture = mBola;
	BolaSprite.TexCoord.x = 0;
	BolaSprite.TexCoord.y = 0;
	BolaSprite.TexSize.x = 1.0f;
	BolaSprite.TexSize.y = 1.0f;
	BolaSprite.TextureIndex = 0;
	BolaSprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	mFSM.SetCurrentState(NormalMov::Instance());
	// estados de la entidad. La pelota por ejemplo, si conseguimos una poción, iría al doble de su velocidad... por ejemplo
}
	


	BolaEntity::~BolaEntity()
	{
	d3dcore->Resource_Free( mBola );
	}

void BolaEntity::Update( float delta )
{

	mFSM.Update();
	setBolaPrevPos (getBolaPos());
	

		//mBolaDir es un vector 2d de valores 1 y -1
		//necesito otra variable que cambie el angulo del vector mBolaPos.
		
		mBolaPos.x+= mBolaDir.x* mBolaSpeed * delta;
		mBolaPos.y+= mBolaDir.y* mBolaSpeed * delta;
	
			Colision();
	
		
	if (mBolaPos.y<-400.0f) 
		{
			MainState::Lifes_Count--;
			EntityManager::Instance()->Remove(this);
			
			//std::vector<BaseGameEntity*>& Barras = EntityManager::Instance()->GetType( Barra_TYPE );
			//mFSM es un StateMachine de tipo Bola, yo el que quiero es el de Barra, pero está privado. Tendría que hacer
			//un get o usar una variable global para los estados de la barra y luego según el valor de esa variable, en 
			//barraEntity se elegiría un estado u otro. Y así con todas las clases que tienen estados.
			//Barras.SetCurrentState(ControledByHuman::Instance());
			return;
		}
		

	D3DXMATRIX T, S;
	D3DXMatrixScaling(&S, 60.0f, 60.0f, 0.0f);
	D3DXMatrixTranslation(&T, mBolaPos.x, mBolaPos.y, 0.1f);	
	BolaSprite.matWorld = (S*T);
}

void BolaEntity::Colision()
{
	
		if (mBolaPos.x<-200.0f) 
		{
			//mBolaPos.x=-200.0f;
			
			if(( getBolaPrevPos().x>getBolaPos().x) & ( getBolaPrevPos().y < getBolaPos().y))
			{
			// Pared izq
			mBolaDir.x=1.0f;
			mBolaDir.y=1.0f;
		
			return;
			}

			if(( getBolaPrevPos().x>getBolaPos().x) & ( getBolaPrevPos().y > getBolaPos().y))
			{
			// Pared izq
			mBolaDir.x=1.0f;
			mBolaDir.y=-1.0f;
		
			return;
			}
			//estaría bien analizar si se ha chocado con la izq y luego meterle las colisiones de la pared izq, ya venga de arriba o abajo. Y así con el resto.
		
		} 
		if (mBolaPos.x>200.0f) 
		{
			//mBolaPos.x=200.0f;
			
			if(( getBolaPrevPos().x<getBolaPos().x) & ( getBolaPrevPos().y > getBolaPos().y))
			{
			// Pared derecha
			mBolaDir.x=-1.0f;
			mBolaDir.y=-1.0f;
		
			return;
			}

			if(( getBolaPrevPos().x<getBolaPos().x) & ( getBolaPrevPos().y < getBolaPos().y))
			{
			// Pared derecha
			mBolaDir.x=-1.0f;
			mBolaDir.y=1.0f;
		
			return;
			}

		} 

		if (mBolaPos.y>300.0f) 
		{
			//mBolaPos.y=300.0f;
			
			if(( getBolaPrevPos().x<getBolaPos().x) & ( getBolaPrevPos().y < getBolaPos().y))
			{
			// Pared izq desde abajo o brick por la derecha desde abajo.
			mBolaDir.x=1.0f;
			mBolaDir.y=-1.0f;
		
			return;
			}

			if(( getBolaPrevPos().x>getBolaPos().x) & ( getBolaPrevPos().y < getBolaPos().y))
			{
			// Pared izq desde abajo o brick por la derecha desde abajo.
			mBolaDir.x=-1.0f;
			mBolaDir.y=-1.0f;
		
			return;
			}

		
		} 

		

}

void BolaEntity::Render()
{
	d3dcore->G2d_Draw( &BolaSprite, 1 );
}

float BolaEntity::getSpeed()
{
	return mBolaSpeed;
}
void  BolaEntity::setSpeed(float speed)
{
	mBolaSpeed=speed;
}
