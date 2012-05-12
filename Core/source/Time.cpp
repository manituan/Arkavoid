#include "D3DCore_impl.h"
#include <sstream>

//Time Methods
float D3DCore_Impl::Time_GameTime()
{
	return mTimer.getGameTime();
}

float D3DCore_Impl::Time_DeltaTime()
{
	return mTimer.getDeltaTime();
}

DWORD D3DCore_Impl::Time_SystemTime()
{
	return timeGetTime();
}

void D3DCore_Impl::Timer_UpdateStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.

	static int frameCnt = 0;
	static float t_base = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if( (mTimer.getGameTime() - t_base) >= 1.0f )
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;   
		outs.precision(6);
		outs << L"FPS: " << fps << L"\n" 
			 << "Milliseconds: Per Frame: " << mspf;
		mFrameStats = outs.str();
		
		// Reset for next average.
		frameCnt = 0;
		t_base  += 1.0f;
	}
}