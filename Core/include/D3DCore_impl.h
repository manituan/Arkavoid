#ifndef _D3DCORE_IMPL_H_
#define _D3DCORE_IMPL_H_ 1

#include "D3DCore.h"
#include "GameTimer.h"
#include <string>
#include <vector>
#include <list>

#include "audiere.h"
using namespace audiere;

#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }

//*****************************************************************************
// Simple d3d error checker for book demos.
//*****************************************************************************

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
	#endif

#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif 

///////////////////////////////////////////////////////////////////////////////////////////

struct CStreamList
{
	HSTREAM				hstream;
	void*				data;
	CStreamList*		next;
};

struct Slide
{
	OutputStream* chn;
	float dVolume;
	float dPan;
	float dPitch;
	float time;
	Slide* next;
};

enum ChannelType
{
	CT_EFFECT,
	CT_MUSIC,
	CT_STREAM,
};

struct Channel
{
	HCHANNEL Handler;
	ChannelType Type;
	Channel* next;
};


class D3DCore_Impl:public D3DCore
{
public:
	D3DCore_Impl();
	~D3DCore_Impl();

	//System Methods
	bool		System_Init();
	void		System_SetState( SystemStateBool state, bool value );
	void		System_SetState( SystemStateUnsigned state, unsigned value );
	void		System_SetState( SystemStateString state, LPCWSTR value );
	bool		System_GetState( SystemStateBool state);
	unsigned	System_GetState( SystemStateUnsigned state);
	LPCWSTR		System_GetState( SystemStateString state);
	DWORD		System_GetState( SystemStateDWORD state);
	int			System_Run();
	void		System_Shutdown();
	void		System_Log( LPCWSTR text, ... );

	//Ini File
	void		Ini_SetInt(const WCHAR *section, const WCHAR *name, int value);
	int			Ini_GetInt(const WCHAR *section, const WCHAR *name, int def_val);
	void		Ini_SetFloat(const WCHAR *section, const WCHAR *name, float value);
	float		Ini_GetFloat(const WCHAR *section, const WCHAR *name, float def_val);
	void		Ini_SetString(const WCHAR *section, const WCHAR *name, const WCHAR *value);
	WCHAR*		Ini_GetString(const WCHAR *section, const WCHAR *name, const WCHAR *def_val);

	//File Methods
	void*		File_Load(LPCWSTR filename, DWORD *size=0);
	void		File_Free(void *res);
	WCHAR*		File_MakePath(LPCWSTR filename=0);
	WCHAR*		File_EnumFiles(LPCWSTR wildcard=0);
	WCHAR*		File_EnumFolders(LPCWSTR wildcard=0);

	//Time Methods
	float		Time_GameTime();
	float		Time_DeltaTime();
	DWORD		Time_SystemTime();
	void		Timer_UpdateStats();

	//Random Methods
	void		Random_Seed( unsigned int seed );
	int			Random_Int( int min, int max );
	float		Random_Float( float min, float max );
	D3DXVECTOR3 Random_UnitVec3();

	//GameState Methods
	void		GState_Push(GameState* state);
	GameState*	GState_Pop();
	void		GState_PopAll();

	//Input Methods
	bool		Input_keyDown(char key);
	bool		Input_keyUp(char key);
	float		Input_mouseDX();
	float		Input_mouseDY();
	float		Input_mouseDZ();

	//Textures Methods
	HTexture*	Texture_Load( LPCWSTR FileName );
	void		Texture_Free( HTexture* Texture );
	unsigned	Texture_Width( HTexture* texture );
	unsigned	Texture_Height( HTexture* texture );
	HResource*	Resource_Load( LPCWSTR FileName );
	HResource*	Resource_Create( HTexture* Texture );
	void		Resource_Free( HResource* recource );

	//Effect Methods
	HEFFECT		Effect_Create(EFFECTTYPE Type, double frecuency);
	HEFFECT		Effect_Load(const WCHAR *filename, DWORD size=0);
	void		Effect_Free(HEFFECT eff);
	HCHANNEL 	Effect_Play(HEFFECT eff);
	HCHANNEL	Effect_PlayEx(HEFFECT eff, int volume=100, int pan=0, float pitch=1.0f, bool loop=false);

	//Stream Methods
	HSTREAM		Stream_Load(const WCHAR *filename, DWORD size=0);
	void		Stream_Free(HSTREAM stream);
	HCHANNEL	Stream_Play(HSTREAM stream, bool loop, int volume = 100);

	//Channel Methods
	void		Channel_SetPanning(HCHANNEL chn, int pan);
	void	 	Channel_SetVolume(HCHANNEL chn, int volume);
	void	 	Channel_SetPitch(HCHANNEL chn, float pitch);
	void	 	Channel_Pause(HCHANNEL chn);
	void	 	Channel_Resume(HCHANNEL chn);
	void	 	Channel_Stop(HCHANNEL chn);
	void	 	Channel_PauseAll();
	void	 	Channel_ResumeAll();
	void	 	Channel_StopAll();
	bool		Channel_IsPlaying(HCHANNEL chn);
	float		Channel_GetLength(HCHANNEL chn);
	float		Channel_GetPos(HCHANNEL chn);
	void		Channel_SetPos(HCHANNEL chn, float fSeconds);
	void		Channel_SlideTo(HCHANNEL channel, float time, int volume, int pan = -101, float pitch = -1);
	bool		Channel_IsSliding(HCHANNEL channel);

	//Graphics Methods
	void			Gfx_Begin();
	void			Gfx_End();
	void			Gfx_Clear( DWORD Color );
	HBLENDSTATE*	Gfx_CreateBlendMode( D3D10_BLEND_DESC* pBDesc ) ;
	void			Gfx_FreeBlendMode( HBLENDSTATE* hBS);
	void			Gfx_SetBlendMode( HBLENDSTATE* hBS,  const float* Factor );

	//Sprite Methods
	void G2d_Begin();
	void G2d_End();
	void G2d_Flush();
	void G2d_SetView( float x, float y );
	void G2d_Draw( D3DX10_SPRITE* buffer, unsigned size = 1);

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool initMainWindow();
	bool initDirect3D();
	bool initDirectInput();
	bool initAudiere();

	void poll();
	
	void onResize();// reset projection/etc		

	bool update(float delta);
	void render();

protected:
	//System State Variable
	HINSTANCE	mhAppInst;
	HWND		mhMainWnd;
		
	int			mClientWidth;
	int			mClientHeight;

	std::wstring mMainWndCaption;
	std::wstring mMainWndIcon;

	bool		mSpriteOn;
	bool		mWindowed;
	bool		mHideMouse;

	bool		mAppPaused;
	bool		mMinimized;
	bool		mMaximized;
	bool		mResizing;

	//Timer
	GameTimer mTimer;
	std::wstring mFrameStats;

	//D3D10
	D3D10_DRIVER_TYPE md3dDriverType;
	ID3D10Device*    md3dDevice;
	IDXGISwapChain*  mSwapChain;
	ID3D10Texture2D* mDepthStencilBuffer;
	ID3D10RenderTargetView* mRenderTargetView;
	ID3D10DepthStencilView* mDepthStencilView;
	ID3DX10Font* mFont;

	//D3DXSPRITE
	ID3DX10Sprite* mSprite;

	//Texture & Resource
	std::list<HTexture*> mTextureList;
	std::list<HResource*> mResourceList;
	std::list<HBLENDSTATE*> mBlendStateList;

	//DInput
	IDirectInput8*       mDInput;
	IDirectInputDevice8* mKeyboard;
	char                 mKeyboardState[256];
	IDirectInputDevice8* mMouse;
	DIMOUSESTATE2        mMouseState;

	//GameState
	std::vector<GameState*> _states;
	
	void delPopStates();
	std::vector<GameState*> _popped_states;

	//Audio		
	bool	m_bSilent;

	CStreamList*	streams;
	float			m_VolFX;
	float			m_VolMusic;
	float			m_VolStream;
	float			m_Sample;

	AudioDevicePtr		m_pADevice;
	Channel*			channels;
	Slide*				slides;

	void Channel_Slide_Update( float dt );
	void SetFxVolumen();
	void SetMusicVolumen();
	void SetStreamVolumen();

	//Files
	WCHAR				szAppPath[_MAX_PATH];
	WCHAR				szTmpFilename[_MAX_PATH];
	HANDLE				hSearch;
	WIN32_FIND_DATA		SearchData;

	//LogFile
	std::wstring		szLogFile;
	std::wstring		szIniFile;
	WCHAR				szIniString[256];
};

#endif

