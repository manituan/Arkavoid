
#ifndef _LifeENTITY_H_
#define _LifeENTITY_H_

#include "StateMachine.h"
#include "D3DCore.h"
#include "BaseGameEntity.h"

#define Life_TYPE 1

class LifeEntity:public BaseGameEntity
{
public:
	static int Life_ID;
	static int Life_Count;
	LifeEntity(float x, float y);
	~LifeEntity();

	void Update( float delta );
	void Render();

	StateMachine<LifeEntity>* FSM(){return &mFSM;}


	D3DXVECTOR3& getPosition() {return mLifePos;}
	

protected:
	StateMachine<LifeEntity> mFSM;
	
	HResource* mLife;
	D3DX10_SPRITE LifeSprite;
	D3DXVECTOR3 mLifePos;	

};

#endif