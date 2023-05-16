#include "AudioSource.h"
#include <irrklang/irrKlang.h>
using namespace irrklang;

AudioSource::AudioSource(GameObject* gameObject) : Component(gameObject)
{
	if (!engine)
	{
		engine = createIrrKlangDevice();
	}
}

AudioSource::~AudioSource()
{
}

void AudioSource::Start()
{
}

void AudioSource::Play()
{
}

void AudioSource::Pause()
{
}

void AudioSource::Stop()
{
}