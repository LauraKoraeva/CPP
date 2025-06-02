#pragma once
#include <chrono>
#include <string>

class Session
{
private: 
    std::string taskDescription;
    std::chrono::system_clock::time_point sessionStartTime;
    std::chrono::system_clock::time_point sessionEndTime;  
    int durationMinutes;

public:
    Session() { }
    Session(std::string inTaskDescription, int inDurationMinutes);
    Session(std::string inTaskDescription, int inDurationMinutes, std::chrono::system_clock::time_point inStartTime, std::chrono::system_clock::time_point inFinishTime);

    Session& operator=(const Session& other); 


    std::chrono::system_clock::time_point getSessionStartTime() const;
    void setSessionStartTime(std::chrono::time_point<std::chrono::system_clock> inStartTime);

    std::chrono::system_clock::time_point getSessionEndTime() const;
    void setSessionEndTime(std::chrono::time_point<std::chrono::system_clock> inEndTime);

    std::string getTaskDescription() const;
    void setTaskDescription(std::string inTaskDescription);

    int getDurationMinutes() const;
    void setDurationMinutes(int inDurationMinutes);
};