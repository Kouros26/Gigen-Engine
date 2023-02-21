#include "Watch.h"
#include <iostream>

unsigned short Time::FPS::GetFPS()
{
	return fps;
}

void Time::FPS::UpdateFPS()
{
	if (currentTime - lastFPSUpdate < FPSUpdateDelay) return;

	fps = static_cast<unsigned short>(1000 / deltaTime);
	lastFPSUpdate = currentTime;

	const unsigned short fp = fps;
	fpsQueue.push(fp);

	UpdateAverageFPS();

	std::cout << fps << std::endl; //TEMP
}

void Time::FPS::UpdateAverageFPS()
{
	FixedQueue<unsigned short, 10> queueCopy = fpsQueue;

	const auto QueueSize = static_cast<unsigned short>(queueCopy.size());

	for (unsigned short i = 0; i < QueueSize; i++)
	{
		averageFps += queueCopy.front();
		queueCopy.pop();
	}

	averageFps /= QueueSize;

	std::cout << "Average FPS from last 10 seconds : " << averageFps << std::endl;
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