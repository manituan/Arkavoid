#include "BrickState.h"
#include "StateMachine.h"
#include "Main.h"
#include <math.h>
#include "EntityManager.h"
#include "BolaEntity.h" 




NormalMode* NormalMode::Instance()
{
	static NormalMode instance;
	return &instance;
}

void NormalMode::Enter(BrickEntity* ne)
{

}

void NormalMode::Execute(BrickEntity* ne)
{
	float delta = d3dcore->Time_DeltaTime();


	
	
}

void NormalMode::Exit(BrickEntity* ne)
{

}
