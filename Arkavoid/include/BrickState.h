#ifndef BrickSTATES_H_
#define BrickSTATES_H_1

#include "State.h"
#include "BrickEntity.h"



class NormalMode:public State<BrickEntity>
{
public:
	static NormalMode* Instance();

	void Enter(BrickEntity*);
	void Execute(BrickEntity*);
	void Exit(BrickEntity*);

private:
	NormalMode(){}
	NormalMode(const NormalMode&){}
	NormalMode& operator=(const NormalMode&){}
};





#endif