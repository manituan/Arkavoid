#include "BolaState.h"
#include "StateMachine.h"
#include "Main.h"
#include <math.h>
#include "BolaEntity.h"


NormalMov* NormalMov::Instance()
{
	static NormalMov instance;
	return &instance;
}

void NormalMov::Enter(BolaEntity* ne)
{



}

void NormalMov::Execute(BolaEntity* ne)
{
	float delta = d3dcore->Time_DeltaTime();

	//No están hechos. Hay que ver si es 1 o -1
	//Lo hago aquí o en BolaEntity?
	//Habría algún bonus que cambie las colisiones con las paredes, volviendote loco. Con otro State, por ejemplo.
	//Comparo la x de la anterior PrevPos con la nueva, para saber qué dirección lleva.
	



	
	
}

void NormalMov::Exit(BolaEntity* ne)
{

}