
#include "BarraState.h"
#include "StateMachine.h"
#include "Main.h"
#include "EntityManager.h"
#include "BolaEntity.h" 


//Cosas que faltan
//

ControledByHuman* ControledByHuman::Instance()
{
	static ControledByHuman instance;
	return &instance;
}

void ControledByHuman::Enter(BarraEntity* ne)
{

}

void ControledByHuman::Execute(BarraEntity* ne)
{
	float delta = d3dcore->Time_DeltaTime();
	ne->setmDir(0.0f);
	
	if ((ne->getPosition().x)<(-200.0f+ne->getSize()/2)) ne->setmBarraPos(-200.0f+(ne->getSize()/2));
	if ((ne->getPosition().x)>(200.0f-ne->getSize()/2))  ne->setmBarraPos(200.0f-(ne->getSize()/2));

	//Ver si hay una bola, sino crearla pulsando espacio.
	//No la ha creado todavía cuando creo este vector, da error en tiempo de ejecución.
	if(EntityManager::Instance()->GetType(Bola_TYPE).size()<1)
	{if (d3dcore->Input_keyDown(DIK_SPACE)) EntityManager::Instance()->Add( new BolaEntity );}

	
	if( d3dcore->Input_keyDown(DIK_A) )	 ne->setmDir(-1.0f);
	if( d3dcore->Input_keyDown(DIK_D) )	 ne->setmDir(1.0f);
	
	
}

void ControledByHuman::Exit(BarraEntity* ne)
{

}





ChangeDir* ChangeDir::Instance()
{
	static ChangeDir instance;
	return &instance;
}

void ChangeDir::Enter(BarraEntity* ne)
{

}

void ChangeDir::Execute(BarraEntity* ne)
{
	float delta = d3dcore->Time_DeltaTime();
	ne->setmDir(0.0f);
	
	if ((ne->getPosition().x)<(-200.0f+ne->getSize()/2)) ne->setmBarraPos(-200.0f+(ne->getSize()/2));
	if ((ne->getPosition().x)>(200.0f-ne->getSize()/2))  ne->setmBarraPos(200.0f-(ne->getSize()/2));

	//Ver si hay una bola, sino crearla pulsando espacio.
	//No la ha creado todavía cuando creo este vector, da error en tiempo de ejecución.
	if(EntityManager::Instance()->GetType(Bola_TYPE).size()<1)
	{if (d3dcore->Input_keyDown(DIK_SPACE)) EntityManager::Instance()->Add( new BolaEntity );}

	
	if( d3dcore->Input_keyDown(DIK_A) )	 ne->setmDir(1.0f);
	if( d3dcore->Input_keyDown(DIK_D) )	 ne->setmDir(-1.0f);
	
	//mFSM se le pasa como argumento un objeto de tipo BarraEntity
	
	
	
}

void ChangeDir::Exit(BarraEntity* ne)
{

}