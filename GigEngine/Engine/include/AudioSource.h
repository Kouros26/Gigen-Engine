#pragma once
#include "Component.h"

class Camera;

namespace irrklang
{
	class ISoundEngine;
	class ISound;
}

class AudioSource : public Component
{
public:
	AudioSource(GameObject* gameObject);
	~AudioSource() override;

	virtual void Start() override;
	virtual void Update(float pDeltaTime) override;
	virtual Component* Clone(GameObject* newGameObject) override;

	static void UpdateAudioEngine(Camera* cam);
	static void Clear();

	void Play();
	void Pause();
	void UnPause();
	void Stop();

	void SetAudio(const std::string& path);
	std::string GetAudio();

	bool GetPlayOnStart();
	void SetPlayOnStart(bool b);

	bool GetIsLooping();
	void SetIsLooping(bool b);

	bool GetIsPlaying();

	float GetVolume();
	void SetVolume(float f);

	float GetMinDistance();
	void SetMinDistance(float f);

	bool GetIs2D();
	void SetIs2D(bool b);

private:
	std::string audioPath;

	bool is2D = false;
	bool playOnStart = true;
	bool loop = true;
	bool isPlaying = false;
	float volume = 1;
	float minDistance = 5.0f;

	irrklang::ISound* sound = nullptr;
	inline static irrklang::ISoundEngine* engine;
};