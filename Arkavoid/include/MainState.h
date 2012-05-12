
#ifndef _MAINSTATE_H_
#define _MAINSTATE_H_


#include "soundclass.h"

class MainState:public GameState
{
public:
	MainState();
	void initialize();				// initialize the state
	void shutdown();				// shutdown the state
	bool update(float delta);		// handle frame update
	void render();					// handle frame rendering

	static int Lifes_Count;

protected:
	HResource* mFondo;
	HResource* mBarra;
	HResource* mBorde;
	HResource* mLife1;
	HResource* mLife2;
	HResource* mLife3;

	D3DX10_SPRITE BkgdSprite[1];
	D3DX10_SPRITE BordeSprite[1];
	D3DX10_SPRITE Life1Sprite[1];
	D3DX10_SPRITE Life2Sprite[1];
	D3DX10_SPRITE Life3Sprite[1];

	HBLENDSTATE *AlphaOn;

	SoundClass* m_Sound;
	

};

 #endif 