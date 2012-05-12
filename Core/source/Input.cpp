#include "D3DCore_impl.h"

//Input Methods
bool D3DCore_Impl::Input_keyDown(char key)
{
	if( key == DIK_LMOUSE ) return (mMouseState.rgbButtons[0] & 0x80) != 0; 
	if( key == DIK_RMOUSE ) return (mMouseState.rgbButtons[1] & 0x80) != 0; 
	if( key == DIK_MMOUSE ) return (mMouseState.rgbButtons[2] & 0x80) != 0; 
	return (mKeyboardState[key] & 0x80) != 0;
}

bool D3DCore_Impl::Input_keyUp(char key)
{
	if( key == DIK_LMOUSE ) return (mMouseState.rgbButtons[0] & 0x80) == 0; 
	if( key == DIK_RMOUSE ) return (mMouseState.rgbButtons[1] & 0x80) == 0; 
	if( key == DIK_MMOUSE ) return (mMouseState.rgbButtons[2] & 0x80) == 0; 
	return (mKeyboardState[key] & 0x80) == 0;
}

float D3DCore_Impl::Input_mouseDX()
{
	return (float)mMouseState.lX;
}

float D3DCore_Impl::Input_mouseDY()
{
	return (float)mMouseState.lY;
}

float D3DCore_Impl::Input_mouseDZ()
{
	return (float)mMouseState.lZ;
}

/////////////////////////////////////////////////////////////////////////

bool D3DCore_Impl::initDirectInput()
{
	ZeroMemory(mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&mMouseState, sizeof(mMouseState));

	HR(DirectInput8Create(mhAppInst, DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&mDInput, 0));

	HR(mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0));
	HR(mKeyboard->SetDataFormat(&c_dfDIKeyboard));
	HR(mKeyboard->SetCooperativeLevel(mhMainWnd, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND));
	HR(mKeyboard->Acquire());

	HR(mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0));
	HR(mMouse->SetDataFormat(&c_dfDIMouse2));
	HR(mMouse->SetCooperativeLevel(mhMainWnd, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND));
	HR(mMouse->Acquire());

	return true;
}


void D3DCore_Impl::poll()
{
	// Poll keyboard.
	HRESULT hr = mKeyboard->GetDeviceState(sizeof(mKeyboardState), (void**)&mKeyboardState); 
	if( FAILED(hr) )
	{
		// Keyboard lost, zero out keyboard data structure.
		ZeroMemory(mKeyboardState, sizeof(mKeyboardState));

		 // Try to acquire for next time we poll.
		hr = mKeyboard->Acquire();
	}

	// Poll mouse.
	hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mMouseState); 
	if( FAILED(hr) )
	{
		// Mouse lost, zero out mouse data structure.
		ZeroMemory(&mMouseState, sizeof(mMouseState));

		// Try to acquire for next time we poll.
		hr = mMouse->Acquire(); 
	}
}