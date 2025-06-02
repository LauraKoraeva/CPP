#include "Timer.h"

Timer::Timer() : durationSeconds(0), elapsedSeconds(0), isRunning(false), isPaused(false) 
{ 
	Session();
}


void Timer::run()
{
	while (isRunning && elapsedSeconds < durationSeconds)
	{
		if (isPaused)
		{
			std::unique_lock<std::mutex> lock(mutex);
			cv.wait(lock, [&] { return isPaused == false; });
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
		++elapsedSeconds;
	}
	if (elapsedSeconds >= durationSeconds)
	{
		finishTime = std::chrono::system_clock::now();
		session.setSessionEndTime(finishTime);
		stop();
	}
}



void Timer::start(Session& inSession, std::function<void(Session& s)> inCallback)
{
	if (isRunning)
	{
		std::cout << "Timer is already running.\n";
		return;		
	}
	durationSeconds = inSession.getDurationMinutes() * 60;
	session = inSession;
	isRunning = true;
	isPaused = false;
	callback = inCallback;
	startTime = std::chrono::system_clock::now();
	session.setSessionStartTime(startTime);
    std::cout << "Running\n";
	timerThread = std::thread([this]() { run(); }); 
	timerThread.detach(); // ///////////////////////
}



void Timer::pause()
{
	if (isRunning && !isPaused)
	{
		std::unique_lock<std::mutex> lock(mutex);
		std::cout << "Paused\n";
		isPaused = true;
		cv.notify_one();
	}
}



void Timer::resume()
{
	if (isPaused)
	{
		std::unique_lock<std::mutex> lock(mutex);
		std::cout << "Resumed\n";
		isPaused = false;
		cv.notify_one();
	}
}



void Timer::stop()
{
	if (isRunning || isPaused)
	{
		isRunning = false;
		
	
		session.setDurationMinutes(elapsedSeconds / 60);
		std::cout << "Stopped\n";
		//timerThread.join();
		callback(session);
	}
}


bool Timer::is_Running() { return isRunning; };
bool Timer::is_Paused() { return isPaused; };