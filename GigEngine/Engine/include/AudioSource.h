#pragma once
#include "Component.h"

class ISoundEngine;

class AudioSource : public Component
{
public:
	AudioSource(GameObject* gameObject);
	~AudioSource();

	virtual void Start() override;

	void Play();
	void Pause();
	void Stop();

private:
	bool playOnStart = true;
	bool loop = false;
	bool isPlaying = false;
	float volume = 1;

	static ISoundEngine* engine;
};