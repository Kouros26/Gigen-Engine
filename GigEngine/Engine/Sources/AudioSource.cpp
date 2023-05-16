#include "AudioSource.h"
#include <irrKlang.h>
using namespace irrklang;

AudioSource::AudioSource(GameObject* gameObject) : Component(gameObject)
{
	if (!engine)
	{
		engine = createIrrKlangDevice();
	}
	engine->play2D("Audio/maybe.mp3", true);
}

AudioSource::~AudioSource()
{
}

void AudioSource::Start()
{
}

Component* AudioSource::Clone(GameObject* newGameObject)
{
	return nullptr;
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