#include "Session.h"

Session::Session(std::string inTaskDescription, int inDurationMinutes) : 
    taskDescription(inTaskDescription), durationMinutes(inDurationMinutes) { }

Session::Session(std::string inTaskDescription, int inDurationMinutes, std::chrono::system_clock::time_point inStartTime, std::chrono::system_clock::time_point inFinishTime) : 
    taskDescription(inTaskDescription), durationMinutes(inDurationMinutes),
    sessionStartTime(inStartTime), sessionEndTime(inFinishTime) { }


Session& Session::operator=(const Session& other) 
{    
    if (this == &other) 
    {
        return *this;
    }

    taskDescription = other.taskDescription;
    durationMinutes = other.durationMinutes;
    sessionStartTime = other.sessionStartTime;
    sessionEndTime = other.sessionEndTime;

    return *this;
}


std::chrono::system_clock::time_point Session::getSessionStartTime() const
{
    return sessionStartTime;
}

void Session::setSessionStartTime(std::chrono::time_point<std::chrono::system_clock> inStartTime)
{
    sessionStartTime = inStartTime;
}

std::chrono::system_clock::time_point Session::getSessionEndTime() const
{
    return sessionEndTime;
}

void Session::setSessionEndTime(std::chrono::time_point<std::chrono::system_clock> inEndTime)
{
    sessionEndTime = inEndTime;
}

std::string Session::getTaskDescription() const
{
    return taskDescription;
}

void Session::setTaskDescription(std::string inTaskDescription)
{
    taskDescription = inTaskDescription;
}

int Session::getDurationMinutes() const
{
    return durationMinutes;
}

void Session::setDurationMinutes(int inDurationMinutes)
{
    durationMinutes = inDurationMinutes;
}





