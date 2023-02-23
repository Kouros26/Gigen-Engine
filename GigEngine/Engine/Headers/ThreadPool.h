#pragma once
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>

class ThreadPool
{
public:

	ThreadPool();
	~ThreadPool();
	static void AddToQueue(const std::function<void()>& task);
	static bool IsBusy();
private:

	void ThreadLoop();

	inline static bool shouldStop = false;
	inline static std::mutex queueMutex;
	inline static std::condition_variable mutexCondition;
	inline static std::vector<std::thread> threads;
	inline static std::queue<std::function<void()>> tasksQueue;
};