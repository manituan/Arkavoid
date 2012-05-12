#include "D3DCore_impl.h"

bool D3DCore_Impl::initAudiere()
{
	if(m_pADevice) return true;

	m_bSilent=false;
	m_pADevice = audiere::OpenDevice();
	if ( !m_pADevice.get() )
	{
		m_bSilent=true;
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

HEFFECT D3DCore_Impl::Effect_Create(EFFECTTYPE Type, double frecuency)
{
	if(m_bSilent) return 0;
	if(!m_pADevice) return 0;

	SampleSource* SSource = NULL;

	switch(Type)
	{
	case EFFECT_SINE:
		SSource = CreateTone(frecuency);
		break;
	case EFFECT_SQUARE:
		SSource = CreateSquareWave(frecuency);
		break;
	case EFFECT_WHITENOISE:
		SSource = CreateWhiteNoise();
		break;
	case EFFECT_PINKNOISE:
		SSource = CreatePinkNoise();
		break;
	};

	return (HEFFECT)SSource;
}

HEFFECT D3DCore_Impl::Effect_Load(const WCHAR *filename, DWORD size)
{
	if(m_bSilent) return 0;

	DWORD _size;
	FilePtr pf;
	SampleSource* SSource = NULL;
	void *data;

	if(m_pADevice)
	{
		if(size) { data=(void *)filename; _size=size; }
		else
		{
			data=File_Load(filename, &_size);
			if(!data) return NULL;
		}
	
		pf = CreateMemoryFile(data, _size);
		if(pf.get() != NULL) {
			SSource = OpenSampleSource( pf.get() );			
			if(!SSource) {
				System_Log(L"[AUDIERE - ERROR] %s - Can't create sound effect", filename);				
			}
			SSource->ref();
		}

		if(!size) File_Free(data);
		return (HEFFECT)SSource;
	}
	else return 0;
}

HCHANNEL D3DCore_Impl::Effect_Play(HEFFECT eff)
{
	if(m_pADevice && eff != 0)
	{
		SampleSource* SSource = (SampleSource*)eff;
		OutputStream* OStream = OpenSound( m_pADevice, SSource, false );
		OStream->setVolume( m_VolFX );
		OStream->play();
		OStream->ref();
//		EffectChannelList.insert((HCHANNEL)OStream);

		Channel* chn = new Channel;
		chn->Handler = (HCHANNEL)OStream;
		chn->Type = CT_EFFECT;
		chn->next = channels;
		channels = chn;

		return (HCHANNEL)OStream;
	}
	else return 0;
}

HCHANNEL D3DCore_Impl::Effect_PlayEx(HEFFECT eff, int volume, int pan, float pitch, bool loop)
{
	if(m_pADevice && eff != 0)
	{
		SampleSource* SSource = (SampleSource*)eff;
		OutputStream* OStream = OpenSound( m_pADevice, SSource, false );
		OStream->setVolume( m_VolFX * (volume/100.0f) );
		OStream->setPan( pan/100.0f );
		OStream->setPitchShift( pitch );
		OStream->setRepeat( loop );
		OStream->play();
		OStream->ref();
//		EffectChannelList.insert((HCHANNEL)OStream);

		Channel* chn = new Channel;
		chn->Handler = (HCHANNEL)OStream;
		chn->Type = CT_EFFECT;
		chn->next = channels;
		channels = chn;

		return (HCHANNEL)OStream;
	}
	else return 0;
}


void D3DCore_Impl::Effect_Free(HEFFECT eff)
{
	if(m_pADevice && eff != 0)
	{
		SampleSource* SSource = (SampleSource*)eff;
		SSource->unref();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

HSTREAM D3DCore_Impl::Stream_Load(const WCHAR *filename, DWORD size)
{
	if(m_bSilent) return 0;

	void *data;
	DWORD _size;
	CStreamList *stmItem;

	if(m_pADevice)
	{
		if(size) { data=(void *)filename; _size=size; }
		else
		{
			data=File_Load(filename, &_size);
			if(!data) return 0;
		}
		
		FilePtr pf = CreateMemoryFile(data, _size);
		SampleSourcePtr source = OpenSampleSource(pf);
		if (!source) {
			System_Log(L"[AUDIERE - ERROR] %s: Can't load sample source", filename);
			return 0;
		}

		LoopPointSourcePtr loop_source = CreateLoopPointSource(source);
		if (loop_source) {
			source = loop_source.get();
		}

		OutputStream* stream = OpenSound( m_pADevice, source, true);
		if (!stream) 
		{
			System_Log(L"[AUDIERE - ERROR] %s: Can't load stream", filename);
			if(!size) File_Free(data);
			return 0;
		}
		stream->ref();
		if(!size)
		{
			stmItem=new CStreamList;
			stmItem->hstream=(HSTREAM)stream;
			stmItem->data=data;
			stmItem->next=streams;
			streams=stmItem;
		}
		return (HSTREAM)stream;
	}
	else return 0;
}

void D3DCore_Impl::Stream_Free(HSTREAM stream)
{
	CStreamList *stmItem=streams, *stmPrev=0;

	if(m_pADevice && stream != 0)
	{
		while(stmItem)
		{
			if(stmItem->hstream==stream)
			{
				if(stmPrev) stmPrev->next=stmItem->next;
				else streams=stmItem->next;
				File_Free(stmItem->data);
				delete stmItem;
				break;
			}
			stmPrev=stmItem;
			stmItem=stmItem->next;
		}
		((OutputStream*)stream)->unref();
	}
}

HCHANNEL D3DCore_Impl::Stream_Play(HSTREAM stream, bool loop, int volume)
{
	if(m_pADevice && stream != 0)
	{
		OutputStream* OStream = (OutputStream*)stream;
		OStream->setRepeat( loop );
		OStream->setVolume( m_VolStream * (volume/100.0f) );
		OStream->reset();
		OStream->play();
//		StreamChannelList.insert(stream);
		return stream;
	}
	else return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void D3DCore_Impl::Channel_SetPanning(HCHANNEL chn, int pan)
{
	if(!m_pADevice) return;

	OutputStream* OStream = (OutputStream*)chn;
	for( Channel* I = channels; I != NULL; I = I->next )
	{
		if( I->Handler == chn )
		{			
			OStream->setPan( pan/100.0f );
			return;
		}
	}

	//Recorrer los stream
	for( CStreamList* I = streams; I != NULL; I = I->next )
	{
		if( I->hstream == chn )
		{			
			OStream->setPan( pan/100.0f );
			return;
		}
	}
}

void D3DCore_Impl::Channel_SetVolume(HCHANNEL chn, int volume)
{
	if(!m_pADevice) return;

	OutputStream* OStream = (OutputStream*)chn;
	for( Channel* I = channels; I != NULL; I = I->next )
	{
		if( I->Handler == chn )
		{			
			switch( I->Type)
			{
			case CT_EFFECT:
				OStream->setVolume( m_VolFX * (volume/100.0f) );
				break;
			case CT_MUSIC:
				OStream->setVolume( m_VolMusic * (volume/100.0f) );
				break;
			};
			return;
		}
	}

	//Recorrer los stream
	for( CStreamList* I = streams; I != NULL; I = I->next )
	{
		if( I->hstream == chn )
		{			
			OStream->setVolume( m_VolStream * (volume/100.0f) );
			return;
		}
	}
}

void D3DCore_Impl::Channel_SetPitch(HCHANNEL chn, float pitch)
{
	if(!m_pADevice) return;

	OutputStream* OStream = (OutputStream*)chn;
	for( Channel* I = channels; I != NULL; I = I->next )
	{
		if( I->Handler == chn )
		{			
			OStream->setPitchShift( pitch );
			return;
		}
	}

	//Recorrer los stream
	for( CStreamList* I = streams; I != NULL; I = I->next )
	{
		if( I->hstream == chn )
		{			
			OStream->setPitchShift( pitch );
			return;
		}
	}
}

void D3DCore_Impl::Channel_Pause(HCHANNEL chn)
{
	if(!m_pADevice) return;

	OutputStream* OStream = (OutputStream*)chn;
	for( Channel* I = channels; I != NULL; I = I->next )
	{
		if( I->Handler == chn )
		{			
			OStream->stop();
			return;
		}
	}

	//Recorrer los stream
	for( CStreamList* I = streams; I != NULL; I = I->next )
	{
		if( I->hstream == chn )
		{			
			OStream->stop();
			return;
		}
	}
}

void D3DCore_Impl::Channel_Resume(HCHANNEL chn)
{
	if(!m_pADevice) return;

	OutputStream* OStream = (OutputStream*)chn;
	for( Channel* I = channels; I != NULL; I = I->next )
	{
		if( I->Handler == chn )
		{			
			OStream->play();
			return;
		}
	}

	//Recorrer los stream
	for( CStreamList* I = streams; I != NULL; I = I->next )
	{
		if( I->hstream == chn )
		{			
			OStream->play();
			return;
		}
	}
}

void D3DCore_Impl::Channel_Stop(HCHANNEL chn)
{
	if(!m_pADevice) return;

	OutputStream* OStream = (OutputStream*)chn;
	Channel *I = channels, *Prev = 0;
	while( I != NULL )
	{
		if( I->Handler == chn )
		{			
			OStream->stop();
			OStream->unref();
			if(Prev)	Prev->next = I->next;
			else		channels = I->next;
			delete I;
			return;
		}
		Prev = I;
		I = I->next;
	}

	//Recorrer los stream
	for( CStreamList* I = streams; I != NULL; I = I->next )
	{
		if( I->hstream == chn )
		{			
			OStream->stop();
			return;
		}
	}
}

void D3DCore_Impl::Channel_PauseAll()
{
	if(!m_pADevice) return;

	for( Channel* I = channels; I != NULL; I = I->next )
	{
		((OutputStream*)I->Handler)->stop();
	}

	//Recorrer los stream
	for( CStreamList* I = streams; I != NULL; I = I->next )
	{
		((OutputStream*)I->hstream)->stop();
	}
}

void D3DCore_Impl::Channel_ResumeAll()
{
	if(!m_pADevice) return;

	for( Channel* I = channels; I != NULL; I = I->next )
	{
		((OutputStream*)I->Handler)->play();
	}

	//Recorrer los stream
	for( CStreamList* I = streams; I != NULL; I = I->next )
	{
		((OutputStream*)I->hstream)->play();
	}
}

void D3DCore_Impl::Channel_StopAll()
{
	if(!m_pADevice) return;

	Channel *I = channels, *Prev = 0;
	while( channels != NULL )
	{
		((OutputStream*)channels->Handler)->stop();
		((OutputStream*)channels->Handler)->unref();
		I = channels->next;
		delete channels;
		channels = I;
	}

	//Recorrer los stream
	for( CStreamList* I = streams; I != NULL; I = I->next )
	{
		((OutputStream*)I->hstream)->stop();
	}
}

bool D3DCore_Impl::Channel_IsPlaying(HCHANNEL chn)
{
	if(!m_pADevice) return false;

	OutputStream* OStream = (OutputStream*)chn;
	for( Channel* I = channels; I != NULL; I = I->next )
	{
		if( I->Handler == chn )
		{			
			return OStream->isPlaying();
		}
	}

	//Recorrer los stream
	for( CStreamList* I = streams; I != NULL; I = I->next )
	{
		if( I->hstream == chn )
		{			
			return OStream->isPlaying();
		}
	}

	return false;
}

float D3DCore_Impl::Channel_GetLength(HCHANNEL chn)
{
	if(!m_pADevice) return -1.0f;

	OutputStream* OStream = (OutputStream*)chn;
	for( Channel* I = channels; I != NULL; I = I->next )
	{
		if( I->Handler == chn )
		{			
			return float(OStream->getLength());
		}
	}

	//Recorrer los stream
	for( CStreamList* I = streams; I != NULL; I = I->next )
	{
		if( I->hstream == chn )
		{			
			return float(OStream->getLength());
		}
	}

	return -1.0f;
}

float D3DCore_Impl::Channel_GetPos(HCHANNEL chn)
{
	if(!m_pADevice) return -1.0f;

	OutputStream* OStream = (OutputStream*)chn;
	for( Channel* I = channels; I != NULL; I = I->next )
	{
		if( I->Handler == chn )
		{			
			return float(OStream->getPosition());
		}
	}

	//Recorrer los stream
	for( CStreamList* I = streams; I != NULL; I = I->next )
	{
		if( I->hstream == chn )
		{			
			return float(OStream->getPosition());
		}
	}

	return -1.0f;
}

void D3DCore_Impl::Channel_SetPos(HCHANNEL chn, float fSeconds)
{
	if(!m_pADevice) return;

	OutputStream* OStream = (OutputStream*)chn;
	for( Channel* I = channels; I != NULL; I = I->next )
	{
		if( I->Handler == chn )
		{			
			OStream->setPosition( int(fSeconds) );
			return ;
		}
	}

	//Recorrer los stream
	for( CStreamList* I = streams; I != NULL; I = I->next )
	{
		if( I->hstream == chn )
		{			
			OStream->setPosition( int(fSeconds) );
			return;
		}
	}
}

void D3DCore_Impl::Channel_SlideTo(HCHANNEL chn, float time, int volume, int pan, float pitch)
{
	if(!m_pADevice) return;

	bool existChannel = false;
	for( Channel* I = channels; I != NULL; I = I->next )
		if( I->Handler == chn )
		{
			existChannel = true;
			break;
		}


	if(!existChannel)
	{
		for( CStreamList* I = streams; I != NULL; I = I->next )
			if( I->hstream == chn )
			{
				existChannel = true;
				break;
			}
	}

	if(existChannel)
	{
		Slide* nSlide = new Slide;
		nSlide->chn = (OutputStream*)chn;
		nSlide->time = time;

		OutputStream* OStream = (OutputStream*)chn;

		nSlide->dVolume = 0.0f;
		if( volume != -1 )
		{
			float OldVolume = OStream->getVolume();
			float NewVolume = volume*1.0f;
			if( volume == -2 ) NewVolume = 0.0f;
			nSlide->dVolume = (NewVolume - OldVolume)/time;
		}

		nSlide->dPan = 0.0f;
		if(pan != -101)
		{
			float OldPan = OStream->getPan();
			float NewPan = pan/100.0f;
			nSlide->dPan = (NewPan - OldPan)/time;
		}
			
		nSlide->dPitch = 0.0f;
		if(pan != -101)
		{
			float OldPitch = OStream->getPitchShift();
			float NewPitch = pitch;
			nSlide->dPan = (NewPitch - OldPitch)/time;
		}

		nSlide->next = slides;
		slides = nSlide;		
	}
}

bool D3DCore_Impl::Channel_IsSliding(HCHANNEL channel)
{
	Slide* I = slides;
	while( I != NULL )
	{
		if( I->chn == (OutputStream*)channel )
			return true;		
		I = I->next;
	}

	return false;
}

void D3DCore_Impl::Channel_Slide_Update( float dt )
{
	Slide *I =  slides, *Prev =  0;
	while( I != NULL )
	{
		OutputStream* OStream = (*I).chn;
		(*I).time -= dt;

		float NewVolume = OStream->getVolume() + ((*I).dVolume * dt);
		float NewPan = OStream->getPan() + ((*I).dPan * dt);
		float NewPitch = OStream->getPitchShift() + ((*I).dPitch * dt);

		OStream->setVolume( NewVolume );
		OStream->setPan( NewPan );
		OStream->setPitchShift( NewPitch );

		if( (*I).time <= 0.0f )
		{
			if(Prev)
			{
				Prev->next=I->next;
				delete I;
				I = Prev->next;
			}
			else
			{
				slides=I->next;
				delete I;
				I = slides;
			}			
		}
		else
		{
			Prev = I;
			I = I->next;
		}
	}
}

void D3DCore_Impl::SetFxVolumen()
{
	for( Channel* I = channels; I != NULL; I = I->next )
	{
		if( I->Type == CT_EFFECT )
		{
			((OutputStream*)I->Handler)->setVolume( m_VolFX );
		}
	}
}

void D3DCore_Impl::SetMusicVolumen()
{
	for( Channel* I = channels; I != NULL; I = I->next )
	{
		if( I->Type == CT_MUSIC )
		{
			((OutputStream*)I->Handler)->setVolume( m_VolFX );
		}
	}
}

void D3DCore_Impl::SetStreamVolumen()
{
	for( CStreamList* I = streams; I != NULL; I = I->next )
	{
		((OutputStream*)I->hstream)->setVolume( m_VolStream );
	}
}
