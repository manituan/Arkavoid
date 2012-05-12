#include "BrickEntity.h"
#include "Main.h"
#include "EntityManager.h"
#include "BolaEntity.h"
#include "BrickState.h"
#include "PowerUpEntity.h"
#include "MainState.h"

int BrickEntity::Brick_ID = -1;
int BrickEntity::Brick_Count=0;

BrickEntity::BrickEntity(float x, float y,bool up):BaseGameEntity( 
	BaseGameEntity::GetNextValidID() ),
	mFSM(this)

	//da un warning! ese mFSM (this)
{
	Brick_ID = ID();
	Brick_Count++;
	SetEntityType( Brick_TYPE );
	
	mBrick = d3dcore->Resource_Load( L"img\\Brick.png" );

	mBrickPos=D3DXVECTOR3 (x,y,0.0f);
	
	setUp(up);

	SizeX=60.0f;
	SizeY=30.0f;


	BrickSprite.pTexture = mBrick;
	BrickSprite.TexCoord.x = 0;
	BrickSprite.TexCoord.y = 0;
	BrickSprite.TexSize.x = 1.0f;
	BrickSprite.TexSize.y = 1.0f;
	BrickSprite.TextureIndex = 0;
	BrickSprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	mFSM.SetCurrentState(NormalMode::Instance());
	// estados de la entidad. La pelota por ejemplo, si conseguimos una poción, iría al doble de su velocidad... por ejemplo


}

BrickEntity::~BrickEntity()
{
	d3dcore->Resource_Free( mBrick );
	
}

void BrickEntity::Update( float delta )
{

	mFSM.Update();
	

	//Debería hacer una variable global o llamar a clientWith y demás para saber qué tamaño tiene la ventana y hacer los movimientos de la Brick
	//acordes a ella.
	
	D3DXMATRIX T, S;
	D3DXMatrixScaling(&S, SizeX, SizeY, 0.0f);
	D3DXMatrixTranslation(&T, mBrickPos.x, mBrickPos.y, 0.4f);	
	BrickSprite.matWorld = (S*T);


	std::vector<BaseGameEntity*>& Bolas = EntityManager::Instance()->GetType( Bola_TYPE );
	for( unsigned idx = 0; idx < Bolas.size(); idx++)
	{

		

		D3DXVECTOR3 BolaPos = ((BolaEntity *)Bolas[idx])->getBolaPos();
		D3DXVECTOR3 Prev= ((BolaEntity *)Bolas[idx])->getBolaPrevPos();
		float Radio = ((BolaEntity *)Bolas[idx])->getmBolaRadio();

		//lo de los valores absolutos no sirve cuando está uno a la izq del eje de coordenadas
		//y el otro al otro lado, da igual para las y que para las x. Por eso se daba la vuelta 
		//a la mitad de la pantalla. Tengo que hacer la tira de ifs...
		//1º caso, que de en las esquinas. 
		//En el caso de las X, absBolaPos.x - absBrickPos.x está entre 
		//el tamaño x de la barra/2 y el tamaño de la barra/2+bolaradio.
		//En el caso de las Y´s, absBolaPos.y-absBrickPos.y es la resta 
		//del tamaño y de la barra/2 y el tamaño de la barra/2 +bolaradio.
		// el resultado de distanceX es la distancia de BolaPos.x a uno de los lados del brick. 
		//Por lo que creo que si es menor que el radio de la bola entonces si servirá.
		//No, como el resultado es el mismo, voy a hacer solo un if, pero con 4 OR.
		//No es setmBolaDir, sino hay que multiplicar por -1 las dos componentes de mBolaDir.
		//El valor absoluto si que va, lo que pasa es que hay que hacerlo una vez restado, no abs de resta de abs...
		//esto esta mal. No es un solo if, necesitas 4. Porque cada esquina manda la bola en una dirección diferente, no en *-1-1

		//He cambiado los remove y los this fuera y ahora rompen todos los bricks verticales, no solo los superiores.
		//Está claro que es cosa de esos if.. Trata a los power up como bolas? Y hace la colisión?

		//Aquí empiezan los 4 if de las esquinas de los bricks
		if (((BolaPos.x>=(mBrickPos.x-(getSizeX()/2)-Radio)) & (BolaPos.x<(mBrickPos.x-(getSizeX()/2))) & (BolaPos.y<=(mBrickPos.y+(getSizeY()/2)+Radio)) & (BolaPos.y>(mBrickPos.y+(getSizeY()/2)))))
		//esquina segundo cuadrante
		{
			if(getUp()==true)
			{
				EntityManager::Instance()->Add(new PowerUpEntity(mBrickPos.x,mBrickPos.y));
			}
			EntityManager::Instance()->Remove(this);
			Brick_Count--;
			
			((BolaEntity *)Bolas[idx])->setmBolaDir(-1.0f,1.0f);
			return;
		}

		if(((BolaPos.x>=(mBrickPos.x-(getSizeX()/2)-Radio)) & (BolaPos.x<(mBrickPos.x-(getSizeX()/2))) & (BolaPos.y>=(mBrickPos.y-(getSizeY()/2)-Radio)) & (BolaPos.y<(mBrickPos.y-(getSizeY()/2)))))
		//esquina tercer cuadrante
		{
			if(getUp()==true)
			{
				EntityManager::Instance()->Add(new PowerUpEntity(mBrickPos.x,mBrickPos.y));
			}
			EntityManager::Instance()->Remove(this);
			Brick_Count--;
			
			((BolaEntity *)Bolas[idx])->setmBolaDir(-1.0f,-1.0f);
			return;
		}
		if(((BolaPos.x<=(mBrickPos.x+(getSizeX()/2)+Radio)) & (BolaPos.x>(mBrickPos.x+(getSizeX()/2))) & (BolaPos.y>=(mBrickPos.y-(getSizeY()/2)-Radio)) & (BolaPos.y<(mBrickPos.y-(getSizeY()/2)))))
		//esquina cuarto cuadrante
		{
			if(this->getUp()==true)
			{
				EntityManager::Instance()->Add(new PowerUpEntity(mBrickPos.x,mBrickPos.y));
			}
			EntityManager::Instance()->Remove(this);
			Brick_Count--;
			
			((BolaEntity *)Bolas[idx])->setmBolaDir(1.0f,-1.0f);
			return;
		}
		if(((BolaPos.x<=(mBrickPos.x+(getSizeX()/2)+Radio)) & (BolaPos.x>(mBrickPos.x+(getSizeX()/2))) & (BolaPos.y<=(mBrickPos.y+(getSizeY()/2)+Radio)) & (BolaPos.y>(mBrickPos.y+(getSizeY()/2)))))
		//Esquina primer cuadrante
		{
			if(getUp()==true)
			{
				EntityManager::Instance()->Add(new PowerUpEntity(mBrickPos.x,mBrickPos.y));
			}
			EntityManager::Instance()->Remove(this);
			Brick_Count--;
			
			((BolaEntity *)Bolas[idx])->setmBolaDir(1.0f,1.0f);
			return;
		}

		//Aquí empiezan 4 if de los 4 lados de los bricks

		if(((BolaPos.x>=(mBrickPos.x+(getSizeX()/2)+Radio+10)) & (BolaPos.x<(mBrickPos.x+(getSizeX()/2))) & (BolaPos.y<=(mBrickPos.y+(getSizeY()/2))) & (BolaPos.y>=(mBrickPos.y-(getSizeY()/2)))))
				//Lado Izq. Comprobado
		{
				
			if(getUp()==true)
					{
					EntityManager::Instance()->Add(new PowerUpEntity(mBrickPos.x,mBrickPos.y));
					}
			EntityManager::Instance()->Remove(this);
			Brick_Count--;
			
				if(( Prev.x<BolaPos.x) & ( Prev.y > BolaPos.y))
				{
				//comprobado
					
				((BolaEntity *)Bolas[idx])->setmBolaDir(-1.0f,-1.0f);
				
		
				return;
				}

				if(( Prev.x<BolaPos.x) & ( Prev.y < BolaPos.y))
				{
				//Comprobado
					
				((BolaEntity *)Bolas[idx])->setmBolaDir(-1.0f,1.0f);
				
		
				return;
				}
			
		}

		if(((BolaPos.x<=(mBrickPos.x+(getSizeX()/2)+Radio+10)) & (BolaPos.x>(mBrickPos.x+(getSizeX()/2))) & (BolaPos.y<=(mBrickPos.y+(getSizeY()/2))) & (BolaPos.y>=(mBrickPos.y-(getSizeY()/2)))))
				//Lado derecho. Comprobado.
		{
				
				if(getUp()==true)
					{
					EntityManager::Instance()->Add(new PowerUpEntity(mBrickPos.x,mBrickPos.y));
					}
				EntityManager::Instance()->Remove(this);
				Brick_Count--;
				
				if(( Prev.x>BolaPos.x) & ( Prev.y < BolaPos.y))
				{
				//Comprobado
					
				((BolaEntity *)Bolas[idx])->setmBolaDir(1.0f,1.0f);
				
				return;
				}

				if(( Prev.x>BolaPos.x) & ( Prev.y > BolaPos.y))
				{
				//Comprobado
					
				((BolaEntity *)Bolas[idx])->setmBolaDir(1.0f,-1.0f);
				
				return;
				}
			
		}

		if(((BolaPos.x>=(mBrickPos.x-(getSizeX()/2))) & (BolaPos.x<=(mBrickPos.x+(getSizeX()/2))) & (BolaPos.y>=(mBrickPos.y-(getSizeY()/2)-Radio-10)) & (BolaPos.y<(mBrickPos.y))))
				//Lado de abajo. Comprobado
		{
			
		if(getUp()==true)
					{
					EntityManager::Instance()->Add(new PowerUpEntity(mBrickPos.x,mBrickPos.y));
					}
				EntityManager::Instance()->Remove(this);
				Brick_Count--;
				
			if(( Prev.x<BolaPos.x) & ( Prev.y < BolaPos.y))
			{
				//Comprobado
				
				((BolaEntity *)Bolas[idx])->setmBolaDir(1.0f,-1.0f);
				
				
				return;
			}

			if(( Prev.x>BolaPos.x) & ( Prev.y < BolaPos.y))
			{
			//Comprobado
				
			((BolaEntity *)Bolas[idx])->setmBolaDir(-1.0f,-1.0f);
		
			
			return;
			}
			
			
		}

		if(((BolaPos.x>=(mBrickPos.x-(getSizeX()/2))) & (BolaPos.x<=(mBrickPos.x+(getSizeX()/2))) & (BolaPos.y<=(mBrickPos.y+(getSizeY()/2)+Radio+10)) & (BolaPos.y>(mBrickPos.y))))
				//Lado de arriba
				//Comprobado
		{
			if(getUp()==true)
					{
					EntityManager::Instance()->Add(new PowerUpEntity(mBrickPos.x,mBrickPos.y));
					}
				EntityManager::Instance()->Remove(this);
				Brick_Count--;
				
			if(( Prev.x<BolaPos.x) & ( Prev.y > BolaPos.y))
			{
				//Comprobado
				
				((BolaEntity *)Bolas[idx])->setmBolaDir(1.0f,1.0f);
				
			
				return;
			}

			if(( Prev.x>BolaPos.x) & ( Prev.y > BolaPos.y))
			{
			//Comprobado
				
			((BolaEntity *)Bolas[idx])->setmBolaDir(-1.0f,1.0f);
		
			
			return;
			}
			
			
		}


	}

} 
		



void BrickEntity::Render()
{
	d3dcore->G2d_Draw( &BrickSprite, 1 );
}

