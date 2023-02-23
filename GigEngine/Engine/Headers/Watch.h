#pragma once
#include "FixedQueue.h"
#include <GLFW/glfw3.h>

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

		static inline float fps = 0;
		static inline float averageFps = 0;
		static inline double FPSUpdateDelay = 1;
		static inline double lastFPSUpdate = 0;

	public:
		static void UpdateFPS();
		static void UpdateAverageFPS();

		static void SetFPSUpdateDelay(const double& newDelay);
		[[nodiscard]] static float GetFPS();
		static float GetAverageFPS();
	};

	static void UpdateDeltaTime();

	[[nodiscard]] static double GetDeltaTime();
	[[nodiscard]] static double GetTimeScale();
	[[nodiscard]] static double GetUnscaledDeltaTime();

	static void SetTimeScale(const double& newTimeScale);
};
