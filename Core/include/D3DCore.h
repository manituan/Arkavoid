#ifndef _D3DCORE_H_
#define _D3DCORE_H_ 1

// Let VC++ know we are compiling for WinXP and up.  This let's us use
// API functions specific to WinXP (e.g., WM_INPUT API).
#ifndef _WIN32_WINNT
#define _WIN32_WINNT   0x0600 // Vista
#endif

#include <Windows.h>
 
// Enable extra D3D debugging in debug builds if using the debug DirectX runtime.  
// This makes D3D objects work well in the debugger watch window, but slows down 
// performance slightly.
#if defined(DEBUG) || defined(_DEBUG)
	#ifndef D3D_DEBUG_INFO
	#define D3D_DEBUG_INFO
	#endif
#endif

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <d3dx10.h>
#include <dxerr.h>
#include <cassert>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

//Math Helper Stuff
template<typename T>
D3DX10INLINE T Min(const T& a, const T& b)
{
	return a < b ? a : b;
}

template<typename T>
D3DX10INLINE T Max(const T& a, const T& b)
{
	return a > b ? a : b;
}
 
template<typename T>
D3DX10INLINE T Lerp(const T& a, const T& b, float t)
{
	return a + (b-a)*t;
}

template<typename T>
D3DX10INLINE T Clamp(const T& x, const T& low, const T& high)
{
	return x < low ? low : (x > high ? high : x); 
}

// Constants
const float INFINITY = FLT_MAX;
const float PI       = 3.14159265358979323f;
const float MATH_EPS = 0.0001f;

const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const D3DXCOLOR YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR CYAN(0.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);

const D3DXCOLOR BEACH_SAND(1.0f, 0.96f, 0.62f, 1.0f);
const D3DXCOLOR LIGHT_YELLOW_GREEN(0.48f, 0.77f, 0.46f, 1.0f);
const D3DXCOLOR DARK_YELLOW_GREEN(0.1f, 0.48f, 0.19f, 1.0f);
const D3DXCOLOR DARKBROWN(0.45f, 0.39f, 0.34f, 1.0f);

// Converts ARGB 32-bit color format to ABGR 32-bit color format.
D3DX10INLINE UINT ARGB2ABGR(UINT argb)
{
	BYTE A = (argb >> 24) & 0xff;
	BYTE R = (argb >> 16) & 0xff;
	BYTE G = (argb >>  8) & 0xff;
	BYTE B = (argb >>  0) & 0xff;

	return (A << 24) | (B << 16) | (G << 8) | (R << 0);
}

//System Stuff
enum SystemStateDWORD
{
	SSTATE_Instace = 1,		//Get the App Instace (HINSTANCE = DWORD)
	SSTATE_HWND,			//Get the windows Handler (HWND = DWORD)
};

enum SystemStateUnsigned
{
	SSTATE_SCREENWIDTH = SSTATE_HWND + 1,		//Set/Get windows width ( unsigned )
	SSTATE_SCREENHEIGHT,	//Set/Get windows height (unsigned )	
	SSTATE_FXVOLUMEN,
	SSTATE_STREAMVOLUMEN,
};

enum SystemStateString
{
	SSTATE_TITLE = SSTATE_STREAMVOLUMEN + 1,			//Set/Get windows caption (wchar*)
	SSTATE_ICON,			//Set windows icon (wchar*)
	SSTATE_INIFILE,			//Set/Get Ini File
	SSTATE_LOGFILE,			//Set/Get Log File
};

enum SystemStateBool
{
	SSTATE_SPRITE = SSTATE_LOGFILE + 1,			//Set/Get Sprite Creation flag (bool)
	SSTATE_WINDOWED,		//Set/Get Windowed or full screen flag (bool)
	SSTATE_HIDEMOUSE,		//Set/Get Hide Mouse flag (bool)
	SSTATE_USESOUND,
};

//Input Stuff
#define DIK_LMOUSE	0x100
#define DIK_RMOUSE	0x101
#define DIK_MMOUSE	0x102


//GameState Stuff
class GameState
{
public:
	virtual void initialize() = 0;			// initialize the state
	virtual void shutdown() = 0;				// shutdown the state
	virtual bool update(float delta) = 0;		// handle frame update
	virtual void render() = 0;				// handle frame rendering
};

//Texture Stuff
#define HTexture	ID3D10Texture2D
#define HResource	ID3D10ShaderResourceView
#define HBLENDSTATE	ID3D10BlendState

//Sound Stuff
typedef DWORD HEFFECT;
typedef DWORD HCHANNEL;
typedef DWORD HSTREAM;

enum EFFECTTYPE
{
	EFFECT_SINE,
	EFFECT_SQUARE,
	EFFECT_WHITENOISE,
	EFFECT_PINKNOISE,
};

//Core Class
class D3DCore
{
public:
	//System Methods
	virtual bool		System_Init() = 0;
	virtual void		System_SetState( SystemStateBool state, bool value ) = 0;
	virtual void		System_SetState( SystemStateUnsigned state, unsigned value ) = 0;
	virtual void		System_SetState( SystemStateString state, LPCWSTR value ) = 0;
	virtual bool		System_GetState( SystemStateBool state) = 0;
	virtual unsigned	System_GetState( SystemStateUnsigned state) = 0;
	virtual LPCWSTR		System_GetState( SystemStateString state) = 0;
	virtual DWORD		System_GetState( SystemStateDWORD state) = 0;
	virtual int			System_Run() = 0;
	virtual void		System_Shutdown() = 0;
	virtual void		System_Log( LPCWSTR text, ... ) = 0;

	//IniFile
	virtual	void		Ini_SetInt(const WCHAR *section, const WCHAR *name, int value) = 0;
	virtual	int			Ini_GetInt(const WCHAR *section, const WCHAR *name, int def_val) = 0;
	virtual	void		Ini_SetFloat(const WCHAR *section, const WCHAR *name, float value) = 0;
	virtual	float		Ini_GetFloat(const WCHAR *section, const WCHAR *name, float def_val) = 0;
	virtual	void		Ini_SetString(const WCHAR *section, const WCHAR *name, const WCHAR *value) = 0;
	virtual	WCHAR*		Ini_GetString(const WCHAR *section, const WCHAR *name, const WCHAR *def_val) = 0;

	//File Methods
	virtual void*		File_Load(LPCWSTR filename, DWORD *size=0) = 0;
	virtual void		File_Free(void *res) = 0;
	virtual WCHAR*		File_MakePath(LPCWSTR filename=0) = 0;
	virtual WCHAR*		File_EnumFiles(LPCWSTR wildcard=0) = 0;
	virtual WCHAR*		File_EnumFolders(LPCWSTR wildcard=0) = 0;

	//Time Methods
	virtual float		Time_GameTime() = 0;
	virtual float		Time_DeltaTime() = 0;
	virtual DWORD		Time_SystemTime() = 0;
	virtual void		Timer_UpdateStats() = 0;

	//Random Methods
	virtual void		Random_Seed( unsigned int seed ) = 0;
	virtual int			Random_Int( int min, int max ) = 0;
	virtual float		Random_Float( float min, float max ) = 0;
	virtual D3DXVECTOR3 Random_UnitVec3() = 0;

	//GameState Methods
	virtual void		GState_Push(GameState* state) = 0;
	virtual GameState*	GState_Pop() = 0;
	virtual void		GState_PopAll() = 0;

	//Input Methods
	virtual bool		Input_keyDown(char key) = 0;
	virtual bool		Input_keyUp(char key) = 0;
	virtual float		Input_mouseDX() = 0;
	virtual float		Input_mouseDY() = 0;
	virtual float		Input_mouseDZ() = 0;

	//Textures Methods
	virtual HTexture*	Texture_Load( LPCWSTR FileName ) = 0;
	virtual void		Texture_Free( HTexture* Texture ) = 0;
	virtual unsigned	Texture_Width( HTexture* texture ) = 0;
	virtual unsigned	Texture_Height( HTexture* texture ) = 0;
	virtual HResource*	Resource_Load( LPCWSTR FileName ) = 0;
	virtual HResource*	Resource_Create( HTexture* Texture ) = 0;
	virtual void		Resource_Free( HResource* recource ) = 0;

	//Effect Methods
	virtual HEFFECT		Effect_Create(EFFECTTYPE Type, double frecuency) = 0;
	virtual HEFFECT		Effect_Load(const WCHAR *filename, DWORD size=0) = 0;
	virtual void		Effect_Free(HEFFECT eff) = 0;
	virtual HCHANNEL 	Effect_Play(HEFFECT eff) = 0;
	virtual HCHANNEL	Effect_PlayEx(HEFFECT eff, int volume=100, int pan=0, float pitch=1.0f, bool loop=false) = 0;

	//Stream Methods
	virtual HSTREAM		Stream_Load(const WCHAR *filename, DWORD size=0) = 0;
	virtual void		Stream_Free(HSTREAM stream) = 0;
	virtual HCHANNEL	Stream_Play(HSTREAM stream, bool loop, int volume = 100) = 0;

	//Channel Methods
	virtual void		Channel_SetPanning(HCHANNEL chn, int pan) = 0;
	virtual void	 	Channel_SetVolume(HCHANNEL chn, int volume) = 0;
	virtual void	 	Channel_SetPitch(HCHANNEL chn, float pitch) = 0;
	virtual void	 	Channel_Pause(HCHANNEL chn) = 0;
	virtual void	 	Channel_Resume(HCHANNEL chn) = 0;
	virtual void	 	Channel_Stop(HCHANNEL chn) = 0;
	virtual void	 	Channel_PauseAll() = 0;
	virtual void	 	Channel_ResumeAll() = 0;
	virtual void	 	Channel_StopAll() = 0;
	virtual bool		Channel_IsPlaying(HCHANNEL chn) = 0;
	virtual float		Channel_GetLength(HCHANNEL chn) = 0;
	virtual float		Channel_GetPos(HCHANNEL chn) = 0;
	virtual void		Channel_SetPos(HCHANNEL chn, float fSeconds) = 0;
	virtual void		Channel_SlideTo(HCHANNEL channel, float time, int volume, int pan = -101, float pitch = -1) = 0;
	virtual bool		Channel_IsSliding(HCHANNEL channel) = 0;

	//Graphics Methods
	virtual void		Gfx_Begin() = 0;
	virtual void		Gfx_End() = 0;
	virtual void		Gfx_Clear( DWORD Color ) = 0;
	virtual HBLENDSTATE*	Gfx_CreateBlendMode( D3D10_BLEND_DESC* pBDesc ) = 0;
	virtual void		Gfx_FreeBlendMode( HBLENDSTATE* hBS) = 0;
	virtual void		Gfx_SetBlendMode( HBLENDSTATE* hBS,  const float* Factor ) = 0;

	//Sprite Methods
	virtual void G2d_Begin() = 0;
	virtual void G2d_End() = 0;
	virtual void G2d_Flush() = 0;
	virtual void G2d_SetView( float x, float y ) = 0;
	virtual void G2d_Draw( D3DX10_SPRITE* buffer, unsigned size = 1) = 0;
};

D3DCore* CreateD3DCore();
void	ReleaseD3DCore();

#endif
