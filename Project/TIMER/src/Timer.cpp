#include "Timer.h"

Timer::Timer() : durationSeconds(0), elapsedSeconds(0), isRunning(false), isPaused(false) 
{ 
	Session();
}


void Timer::run()
{
	session.setSessionStartTime(std::chrono::system_clock::now());
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
		// session.setSessionEndTime(std::chrono::system_clock::now());   // ПЕРЕНЕСТИ В STOP
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
	callback = inCallback;



	std::cout << "\n=======================\n";
    std::cout << "Focus session STARTED\n";
	std::cout << "=======================\n\n";




	timerThread = std::thread([this]() { run(); }); 
	timerThread.detach(); 
}


void Timer::pause()
{
	if (isRunning && !isPaused)
	{
		std::unique_lock<std::mutex> lock(mutex);



		std::cout << "\n=======================\n";
		std::cout << "Focus session PAUSED\n";
		std::cout << "=======================\n\n";



		isPaused = true;
		cv.notify_one();
	}
}


void Timer::resume()
{
	if (isPaused)
	{
		std::unique_lock<std::mutex> lock(mutex);


		std::cout << "\n=======================\n";
		std::cout << "Focus session RESUMED\n";
		std::cout << "=======================\n\n";



		isPaused = false;
		cv.notify_one();
	}
}


void Timer::stop()
{
	if (isRunning || isPaused)
	{
		session.setSessionEndTime(std::chrono::system_clock::now()); // ДОБАВИТЬ
		isRunning = false;
		session.setDurationMinutes(elapsedSeconds / 60);

		


		std::cout << "\n=======================\n";
		std::cout << "Focus session STOPPED\n";
		std::cout << "=======================\n\n";
		
		
		
		
		callback(session);
	}
}


bool Timer::is_Running() { return isRunning; };
bool Timer::is_Paused() { return isPaused; };