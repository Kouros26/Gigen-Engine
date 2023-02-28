#pragma once
#include "FixedQueue.h"

class Time
{
	static inline double deltaTime = 0;
	static inline double timeScale = 1;
	static inline double unscaledDeltaTime;
	static inline double currentTime = 0;
	static inline double lastTime = 0;

public:

	class FPS
	{
		static inline FixedQueue<float, 10> fpsQueue;
		static inline float fpsArray[10];

		static inline float fps = 0;
		static inline float averageFps = 0;
		static inline float FPSUpdateDelay = 0.5f;
		static inline float lastFPSUpdate = 0.0f;

		static void UpdateAverageFPS();

	public:
		static void UpdateFPS();
		static void ToggleVSync(bool input);

		static void SetFPSUpdateDelay(const float newDelay);
		static float GetFPSUpdateDelay();
		[[nodiscard]] static float GetFPS();
		static float GetAverageFPS();
		static FixedQueue<float, 10>& GetFPSQueue();
		static float* GetFPSArray();
	};

	static void UpdateDeltaTime();

	[[nodiscard]] static double GetDeltaTime();
	[[nodiscard]] static double GetTimeScale();
	[[nodiscard]] static double GetUnscaledDeltaTime();
	[[nodiscard]] static double GetCurrentTime();

	static void SetTimeScale(const double& newTimeScale);
};
