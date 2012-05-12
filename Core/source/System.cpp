#include "D3DCore_impl.h"

//System Methods
bool D3DCore_Impl::System_Init()
{
	if( !initMainWindow() ) return false;
	if( !initDirect3D() ) return false;
	if( !initDirectInput() ) return false;
	if( !initAudiere() ) return false;

	D3DX10_FONT_DESC fontDesc;
	fontDesc.Height          = 24;
    fontDesc.Width           = 0;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
    wcscpy(fontDesc.FaceName, L"Times New Roman");

	D3DX10CreateFontIndirect(md3dDevice, &fontDesc, &mFont);

	if( mSpriteOn )
	{
		HR(D3DX10CreateSprite(md3dDevice, 0, &mSprite));
		
		D3DXMATRIX matProjection;
		D3DXMatrixOrthoLH(&matProjection,								
								(float)mClientWidth,
								(float)mClientHeight,
								0.1f,
								10);
		HR( mSprite->SetProjectionTransform(&matProjection) );
	}
	 return true;
}

void D3DCore_Impl::System_SetState( SystemStateBool state, bool value )
{
	switch( state)
	{
	case SSTATE_SPRITE:			//Set/Get Sprite Creation flag (bool)
		mSpriteOn = value;
		return;
	case SSTATE_WINDOWED:		//Set/Get Windowed or full screen flag (bool)
		mWindowed = value;
		return;
	case SSTATE_HIDEMOUSE:		//Set/Get Hide Mouse flag (bool)
		mHideMouse = value;
		return;
	case SSTATE_USESOUND:
		m_bSilent = value;
		return;
	};
}

void D3DCore_Impl::System_SetState( SystemStateUnsigned state, unsigned value )
{
	switch(state)
	{
	case SSTATE_SCREENWIDTH:		//Set/Get windows width ( unsigned )
		mClientWidth = value;
		return;
	case SSTATE_SCREENHEIGHT:	//Set/Get windows height (unsigned )
		mClientHeight = value;
		return;
	case SSTATE_FXVOLUMEN:
		m_VolFX=(value*1.0f);
		SetFxVolumen();
		return;
	case SSTATE_STREAMVOLUMEN:
		m_VolStream=(value*1.0f);
		SetStreamVolumen();
		return;
	};
}

void D3DCore_Impl::System_SetState( SystemStateString state, LPCWSTR value )
{
	switch(state)
	{
	case SSTATE_TITLE:		//Set/Get windows caption (wchar*)
		mMainWndCaption = value;
		return;
	case SSTATE_ICON:			//Set windows icon (wchar*)
		mMainWndIcon = value;
		return;
	case SSTATE_INIFILE:
		szIniFile = value;
		return;
	case SSTATE_LOGFILE:
		szLogFile = value;
		return;
	};
}

bool D3DCore_Impl::System_GetState( SystemStateBool state)
{
	switch( state)
	{
	case SSTATE_SPRITE:			//Set/Get Sprite Creation flag (bool)
		return mSpriteOn;
	case SSTATE_WINDOWED:		//Set/Get Windowed or full screen flag (bool)
		return mWindowed;
	case SSTATE_HIDEMOUSE:		//Set/Get Hide Mouse flag (bool)
		return mHideMouse;
	case SSTATE_USESOUND:
		return m_bSilent;
	};
	return false;
}

unsigned D3DCore_Impl::System_GetState( SystemStateUnsigned state)
{
	switch(state)
	{
	case SSTATE_SCREENWIDTH:		//Set/Get windows width ( unsigned )
		return mClientWidth;
	case SSTATE_SCREENHEIGHT:	//Set/Get windows height (unsigned )
		return mClientHeight;
	case SSTATE_FXVOLUMEN:
		return (unsigned)m_VolFX;
	case SSTATE_STREAMVOLUMEN:
		return (unsigned)m_VolStream;
	};
	return 0;
}

LPCWSTR	D3DCore_Impl::System_GetState( SystemStateString state)
{
	switch(state)
	{
	case SSTATE_TITLE:			//Set/Get windows caption (wchar*)
		return mMainWndCaption.c_str();
	case SSTATE_ICON:			//Set windows icon (wchar*)
		return mMainWndIcon.c_str();
	case SSTATE_INIFILE:
		return szIniFile.c_str();
	case SSTATE_LOGFILE:
		return szLogFile.c_str();
	};
	return NULL;
}

DWORD D3DCore_Impl::System_GetState( SystemStateDWORD state)
{
	switch(state)
	{
	case SSTATE_Instace:		//Get the App Instace (HINSTANCE = DWORD)
		return (DWORD)mhAppInst;
	case SSTATE_HWND:			//Get the windows Handler (HWND = DWORD)
		return (DWORD)mhMainWnd;
	};
	return DWORD(0);
}

int	D3DCore_Impl::System_Run()
{
	MSG msg = {0};
 
	mTimer.reset();

	while(msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}
		// Otherwise, do animation/game stuff.
		else
        {	
			mTimer.tick();

			poll();
			Channel_Slide_Update(mTimer.getDeltaTime());

			if( !mAppPaused )
				if( !update(mTimer.getDeltaTime()) )
					PostQuitMessage(0);
			else
				Sleep(50);

			render();
        }
    }
	return (int)msg.wParam;
}

void D3DCore_Impl::System_Shutdown()
{
	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mSwapChain);
	ReleaseCOM(mDepthStencilBuffer);
	ReleaseCOM(md3dDevice);
	ReleaseCOM(mFont);
	ReleaseCOM(mSprite);

	ReleaseCOM(mDInput);
	mKeyboard->Unacquire();
	mMouse->Unacquire();
	ReleaseCOM(mKeyboard);
	ReleaseCOM(mMouse);

	//Game State
	GState_PopAll();
	delPopStates();

	//Texture & Resources
	std::list<HTexture*>::iterator i = mTextureList.begin();
	while( i != mTextureList.end() )
	{
		ReleaseCOM( (*i) );
		i++;
	}
	mTextureList.clear();

	std::list<HResource*>::iterator j = mResourceList.begin();
	while( j != mResourceList.end() )
	{
		ReleaseCOM( (*j) );
		j++;
	}
	mResourceList.clear();

	std::list<HBLENDSTATE*>::iterator k = mBlendStateList.begin();
	while( k != mBlendStateList.end() )
	{
		ReleaseCOM( (*k) );
		k++;
	}
	mBlendStateList.clear();

	//Audiere
	CStreamList *stmItem=streams, *stmNext;
	
	if(m_pADevice)
	{
		m_pADevice = 0;

		while(stmItem)
		{
			stmNext=stmItem->next;
			File_Free(stmItem->data);
			delete stmItem;
			stmItem=stmNext;
		}
		streams=0;
	}


}

void D3DCore_Impl::System_Log( LPCWSTR text, ... )
{
	FILE *hf = NULL;
	va_list ap;
	
	if(!szLogFile[0]) return;

	hf = _wfopen(szLogFile.c_str(), L"a");
	if(!hf) return;

	va_start(ap, text);
	vfwprintf(hf, text, ap);
	va_end(ap);

	fwprintf(hf, L"\n");

	fclose(hf);
}

////////////////////////////////////////////////////////////////////

bool D3DCore_Impl::update(float delta)
{
	delPopStates();
	if (_states.size() > 0)
		return _states[_states.size()-1]->update(delta);
	else
		return false;
}

void D3DCore_Impl::render()
{
	if (_states.size() > 0)
		_states[_states.size()-1]->render();
}

void D3DCore_Impl::onResize()
{
	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.

	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilBuffer);


	// Resize the swap chain and recreate the render target view.

	HR(mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D10Texture2D* backBuffer;
	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	ReleaseCOM(backBuffer);


	// Create the depth/stencil buffer and view.

	D3D10_TEXTURE2D_DESC depthStencilDesc;
	
	depthStencilDesc.Width     = mClientWidth;
	depthStencilDesc.Height    = mClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = 1; // multisampling must match
	depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
	depthStencilDesc.Usage          = D3D10_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D10_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	HR(md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer));
	HR(md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));


	// Bind the render target view and depth/stencil view to the pipeline.

	md3dDevice->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	

	// Set the viewport transform.

	D3D10_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width    = mClientWidth;
	vp.Height   = mClientHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	md3dDevice->RSSetViewports(1, &vp);

	if( ! mSprite ) return;

	D3DXMATRIX matProjection;
	D3DXMatrixOrthoLH(&matProjection,								
								(float)mClientWidth,
								(float)mClientHeight,
								0.1f,
								10);
	HR( mSprite->SetProjectionTransform(&matProjection) );
}