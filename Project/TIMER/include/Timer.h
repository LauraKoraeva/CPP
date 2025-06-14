#pragma once
#include <iostream>
#include <chrono>
#include <atomic>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include "Session.h"

class Timer
{
    std::atomic<bool> isRunning;
    std::atomic<bool> isPaused;

	int durationSeconds;
	int elapsedSeconds;

    std::thread timerThread;
	std::mutex timerMutex;
	std::condition_variable timerCV;

	std::function<void(Session& s)> callback;
	Session session;
	
public:
	Timer();

	void run();

	void start(Session& inSession, std::function<void(Session& s)> inCallback = { });

	void pause();

	void resume();

	void stop();

    bool is_Running();
    bool is_Paused();
};
