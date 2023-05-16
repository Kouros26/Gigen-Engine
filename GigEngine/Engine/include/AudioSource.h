#pragma once
#include "Component.h"

namespace irrklang
{
	class ISoundEngine;
}

class AudioSource : public Component
{
public:
	AudioSource(GameObject* gameObject);
	~AudioSource() override;

	virtual void Start() override;
	virtual Component* Clone(GameObject* newGameObject) override;

	void Play();
	void Pause();
	void Stop();

private:
	bool playOnStart = true;
	bool loop = false;
	bool isPlaying = false;
	float volume = 1;

	inline static irrklang::ISoundEngine* engine;
};