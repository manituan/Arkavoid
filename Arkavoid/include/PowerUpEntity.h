#ifndef POWERUPENTITY
#define POWERUPENTITY

#include "StateMachine.h"
#include "D3DCore.h"
#include "BaseGameEntity.h"
#include "Main.h"
#include "EntityManager.h"
#include "BarraEntity.h"

#define PowerUp_TYPE 10

class PowerUpEntity:public BaseGameEntity
{
public:
	static int PowerUp_ID;

	PowerUpEntity(float x, float y);
	~PowerUpEntity();

	void Update( float delta );
	void Render();
	void Colision();

	StateMachine<PowerUpEntity>* FSM(){return &mFSM;}

	int getmPowerUp_tipo()
	{
		return mPowerUp_tipo;
	}

	float getSpeed();
	void  setSpeed(float speed);
	float getmPowerUpRadio()
	{
	 return mPowerUpRadio;
	}


	void setPowerUpPrevPos (D3DXVECTOR3 prev)
	{  
		mPowerUpPrevPos=prev;
	};

	void setPowerUpPos (D3DXVECTOR3 pos)
	{
	
			mPowerUpPos=pos;

	};
	




	D3DXVECTOR3& getPowerUpPos() {return mPowerUpPos;}
	D3DXVECTOR3& getPowerUpPrevPos() {return mPowerUpPrevPos;}

protected:
	StateMachine<PowerUpEntity> mFSM;

	HResource* mPowerUp;
	D3DX10_SPRITE PowerUpSprite;
	D3DXVECTOR3 mPowerUpPos;
	float       mPowerUpSpeed;
	D3DXVECTOR3 mPowerUpPrevPos;
	int			mPowerUp_tipo;
	float       mPowerUpRadio;

};


#endif