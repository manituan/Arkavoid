#ifndef BOLASTATE
#define BOLASTATE

#include "State.h"
#include "BolaEntity.h"
#include "BarraEntity.h"


class NormalMov:public State<BolaEntity>
{
public:
	static NormalMov* Instance();

	void Enter(BolaEntity*);
	void Execute(BolaEntity*);
	void Exit(BolaEntity*);

private:
	NormalMov(){}
	NormalMov(const NormalMov&){}
	NormalMov& operator=(const NormalMov&){}
};


#endif