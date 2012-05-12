#ifndef BOLAENTITY
#define BOLAENTITY

#include "StateMachine.h"
#include "D3DCore.h"
#include "BaseGameEntity.h"

#define Bola_TYPE 2

class BolaEntity:public BaseGameEntity
{
public:
	static int Bola_ID;

	BolaEntity();
	~BolaEntity();

	void Update( float delta );
	void Render();
	void Colision();

	StateMachine<BolaEntity>* FSM(){return &mFSM;}

	float getSpeed();
	void  setSpeed(float speed);
	float getmBolaRadio()
	{
	 return mBolaRadio;
	}

	void setmBolaDir (float vx, float vy)
		
	{
		mBolaDir.x= vx;
		mBolaDir.y= vy;
	};
	void setBolaPrevPos (D3DXVECTOR3 prev)
	{  
		mBolaPrevPos=prev;
	};

	void setBolaPos (D3DXVECTOR3 pos)
	{
	
			mBolaPos=pos;

	};
	



	D3DXVECTOR3& getmBolaDir() {return mBolaDir;}
	D3DXVECTOR3& getBolaPos() {return mBolaPos;}
	D3DXVECTOR3& getBolaPrevPos() {return mBolaPrevPos;}

protected:
	StateMachine<BolaEntity> mFSM;

	HResource* mBola;
	D3DX10_SPRITE BolaSprite;
	D3DXVECTOR3 mBolaPos;
	float       mBolaSpeed;
	D3DXVECTOR3 mBolaPrevPos;
	D3DXVECTOR3 mBolaDir;
	float       mBolaRadio;

};


#endif
