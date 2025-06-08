#pragma once
#include <chrono>
#include <string>
#include <vector>
#include <fstream>
#include "Session.h"

class Statistics
{
private:

    std::vector<Session> sessions;

public:
    void recordSession(Session inSession, const std::string& fileName); 
    
    std::chrono::system_clock::time_point stringToTimePoint(const std::string& timeString, const char* format) const;         
    
    std::optional<std::tuple<std::string, int, std::chrono::system_clock::time_point, std::chrono::system_clock::time_point>>
    parseSessionLine(const std::string& line, const char* format);                                           
    
    void loadStatisticsFromFile(const std::string& fileName);                                                                  

    std::string formatTimePoint(const std::chrono::system_clock::time_point& timePoint, const char* format) const;              

    void saveStatisticsToFile(const std::string& fileName) const;
    
    void printStatistics(const std::string& fileName); //const;

    
};