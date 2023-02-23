#include "Watch.h"

float Time::FPS::GetFPS()
{
	return fps;
}

float Time::FPS::GetAverageFPS()
{
	return averageFps;
}

void Time::FPS::UpdateFPS()
{
	if (currentTime - lastFPSUpdate < FPSUpdateDelay) return;

	fps = static_cast<float>(1000 / deltaTime);
	lastFPSUpdate = currentTime;

	const float fp = fps;
	fpsQueue.push(fp);

	UpdateAverageFPS();
}

void Time::FPS::UpdateAverageFPS()
{
	FixedQueue<float, 10> queueCopy = fpsQueue;

	const auto QueueSize = static_cast<float>(queueCopy.size());

	for (unsigned short i = 0; i < QueueSize; i++)
	{
		averageFps += queueCopy.front();
		queueCopy.pop();
	}

	averageFps /= QueueSize;
}

void Time::FPS::SetFPSUpdateDelay(const double& newDelay)
{
	FPSUpdateDelay = newDelay;
}

void Time::UpdateDeltaTime()
{
	currentTime = glfwGetTime();
	unscaledDeltaTime = currentTime - lastTime;
	deltaTime = timeScale * unscaledDeltaTime;
	lastTime = currentTime;

	FPS::UpdateFPS();
}

double Time::GetDeltaTime()
{
	return deltaTime;
}

double Time::GetTimeScale()
{
	return timeScale;
}

double Time::GetUnscaledDeltaTime()
{
	return unscaledDeltaTime;
}

void Time::SetTimeScale(const double& newTimeScale)
{
	timeScale = newTimeScale;
}