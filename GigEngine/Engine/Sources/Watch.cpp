#include "Watch.h"
#include "Window.h"

float Time::FPS::GetFPS()
{
	return fps;
}

float Time::FPS::GetAverageFPS()
{
	return averageFps;
}

FixedQueue<float, 10>& Time::FPS::GetFPSQueue()
{
	return fpsQueue;
}

float* Time::FPS::GetFPSArray()
{
	return fpsArray;
}

void Time::FPS::UpdateFPS()
{
	if (currentTime - lastFPSUpdate < FPSUpdateDelay) return;

	fps = static_cast<float>(1 / deltaTime);
	lastFPSUpdate = currentTime;

	const float fp = fps;
	fpsQueue.push(fp);

	UpdateAverageFPS();
}

void Time::FPS::ToggleVSync(const bool input)
{
	//int result;

	//if (input)
	//	result = 1;

	//else
	//	result = 0;

	Window::ToggleVSync(input);
}

void Time::FPS::UpdateAverageFPS()
{
	FixedQueue<float, 10> queueCopy = fpsQueue;

	const auto QueueSize = static_cast<float>(queueCopy.size());

	for (int i = 0; i < QueueSize; i++)
	{
		averageFps += queueCopy.front();
		fpsArray[i] = queueCopy.front();
		queueCopy.pop();
	}

	averageFps /= QueueSize;
}

void Time::FPS::SetFPSUpdateDelay(const float newDelay)
{
	FPSUpdateDelay = newDelay;
}

float Time::FPS::GetFPSUpdateDelay()
{
	return FPSUpdateDelay;
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

double Time::GetCurrentTime()
{
	return currentTime;
}

void Time::SetTimeScale(const double& newTimeScale)
{
	timeScale = newTimeScale;
}