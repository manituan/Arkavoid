#ifndef _BarraENTITY_H_
#define _BarraENTITY_H_

#include "StateMachine.h"
#include "D3DCore.h"
#include "BaseGameEntity.h"

#define Barra_TYPE 1

class BarraEntity:public BaseGameEntity
{
public:
	static int Barra_ID;

	BarraEntity();
	~BarraEntity();

	void Update( float delta );
	void Render();

	StateMachine<BarraEntity>* FSM(){return &mFSM;}

	void Colision();

	float getSpeed();
	void  setSpeed(float speed);

	float getmDir()
	{return mDir;}
	void setmDir(float dir)
	{
		mDir=dir;	
	};

	void setmBarraPos (float x)
	{
		mBarraPos.x=x;
	
	}

	void setSize (float size)
	{
		mSize=size;
	}

	float getSize()
	{
		return mSize;
	}

	D3DXVECTOR3& getPosition() {return mBarraPos;}

	
	StateMachine<BarraEntity> mFSM;

protected:
	
	

	HResource* mBarra;
	D3DX10_SPRITE BarraSprite;
	D3DXVECTOR3 mBarraPos;
	D3DXVECTOR3 mCameraPos;
	float       mDir;
	float       mBarraSpeed;
	float       mSize;
	
	
	
};

#endif
