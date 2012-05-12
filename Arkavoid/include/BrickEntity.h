#ifndef _BrickENTITY_H_
#define _BrickENTITY_H_

#include "StateMachine.h"
#include "D3DCore.h"
#include "BaseGameEntity.h"

#define Brick_TYPE 1

class BrickEntity:public BaseGameEntity
{
public:
	static int Brick_ID;
	static int Brick_Count;

	BrickEntity(float x, float y, bool up);
	~BrickEntity();

	void Update( float delta );
	void Render();

	StateMachine<BrickEntity>* FSM(){return &mFSM;}

	void setPosition(float x,float y)
	{
		mBrickPos.x=x;
		mBrickPos.y=y;


	}
	D3DXVECTOR3& getPosition() {return mBrickPos;}

	float getSizeX ()
	{
	return SizeX;
	}

	float getSizeY()
	{
	return SizeY;
	};

	bool getUp()
	{
		return up;
	}

	void setUp(bool upp)
	{
		up=upp;
	}

protected:
	
	StateMachine<BrickEntity> mFSM;

	HResource* mBrick;
	D3DX10_SPRITE BrickSprite;
	D3DXVECTOR3 mBrickPos;
	float SizeX;
	float SizeY;
	bool up;


	
	
};

#endif