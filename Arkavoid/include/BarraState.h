

#ifndef BarraSTATES_H_
#define BarraSTATES_H_1

#include "State.h"
#include "BarraEntity.h"



class ControledByHuman:public State<BarraEntity>
{
public:
	static ControledByHuman* Instance();

	

	void Enter(BarraEntity*);
	void Execute(BarraEntity*);
	void Exit(BarraEntity*);

private:
	ControledByHuman(){}
	ControledByHuman(const ControledByHuman&){}
	ControledByHuman& operator=(const ControledByHuman&){}
	
};


class ChangeDir:public State<BarraEntity>
{
public:
	static ChangeDir* Instance();
	void Enter(BarraEntity*);
	void Execute(BarraEntity*);
	void Exit(BarraEntity*);


private:
	ChangeDir(){}
	ChangeDir(const ChangeDir&){}
	ChangeDir& operator=(const ControledByHuman&){}	

};


#endif