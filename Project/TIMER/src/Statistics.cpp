#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <sstream>                
#include <stdexcept>              
#include <string>                 
#include <vector>                 
#include <optional>               
#include "Statistics.h"
#include "Session.h"


std::string Statistics::formatTimePoint(const std::chrono::system_clock::time_point& timePoint, const char* format = "%d-%m-%Y %H:%M:%S") const  
{
	auto time = std::chrono::system_clock::to_time_t(timePoint);
	std::tm buffer;

#ifdef _WIN32
    errno_t err = localtime_s(&buffer, &time);  
    if (err != 0)                              
    {
        throw std::runtime_error("localtime_s failed with error code: " + std::to_string(err)); 
    }
#else
    if (localtime_r(&time, &buffer) == nullptr)  
    {
        throw std::runtime_error("localtime_r failed"); 
    }
#endif

	std::stringstream ss;
	ss << std::put_time(&buffer, format); 
	return ss.str();
}



void Statistics::recordSession(Session inSession, const std::string& fileName) 
{
    std::unique_lock<std::mutex> lock(sessionsMutex);                 
    sessions.push_back(inSession);
    
    std::ofstream outFile;
    outFile.open(fileName, std::ios::app);

    if (outFile.is_open())
    {
        std::string startTime = formatTimePoint(inSession.getSessionStartTime());
        std::string endTime = formatTimePoint(inSession.getSessionEndTime());
        outFile << inSession.getTaskDescription() << "," << inSession.getDurationMinutes() << "," << startTime << "," << endTime << std::endl;
        outFile.close();
        std::cout << "Session is recorded and saved to the file " << std::quoted(fileName) << '\n';
    }
    else
    {
        std::cerr << "Couldn't open the file " << std::quoted(fileName) << '\n';
    }


}



void Statistics::saveStatisticsToFile(const std::string& fileName) 
{
    std::ofstream outFile(fileName);

    if (outFile.is_open())
    {
        std::unique_lock<std::mutex> lock(sessionsMutex);                 
        for (const auto& session : sessions)
        {
            std::string startTime = formatTimePoint(session.getSessionStartTime());
            std::string finishTime = formatTimePoint(session.getSessionEndTime());
            outFile << session.getTaskDescription() << "," << session.getDurationMinutes() << "," << startTime << "," << finishTime << std::endl;
        }
        outFile.close();
        std::cout << "Statistics are saved to the file " << std::quoted(fileName) << '\n';
    }
    else
    {
        std::cerr << "Couldn't open the file " << std::quoted(fileName) << '\n';
    }
}



std::chrono::system_clock::time_point Statistics::stringToTimePoint(const std::string& timeString, const char* format = "%d-%m-%Y %H:%M:%S") const 
{
  std::tm t{};
  std::istringstream ss(timeString);
  ss >> std::get_time(&t, format);

  if (ss.fail()) 
  {
    throw std::runtime_error("Error converting string to time_point: Invalid format");
  }

  std::time_t time = mktime(&t);  

  if (time == -1) 
  {
    throw std::runtime_error("Error converting string to time_point: mktime failed");
  }

  return std::chrono::system_clock::from_time_t(time);
}



std::optional<std::tuple<std::string, int, std::chrono::system_clock::time_point, std::chrono::system_clock::time_point>> 
Statistics::parseSessionLine(const std::string& line, const char* format = "%d-%m-%Y %H:%M:%S") 
{
    std::stringstream ss(line);

    std::string description;
    int duration = 0; 
    std::string startTimeString;
    std::string endTimeString;

    if (!std::getline(ss, description, ',')) return std::nullopt; 
    if (!(ss >> duration)) return std::nullopt;                      
    ss.ignore(1); 
    if (!std::getline(ss, startTimeString, ',')) return std::nullopt;
    if (!std::getline(ss, endTimeString, ',')) return std::nullopt;

    try 
    {
        std::chrono::system_clock::time_point start = stringToTimePoint(startTimeString, format);
        std::chrono::system_clock::time_point end = stringToTimePoint(endTimeString, format);

        if (duration <= 0) 
        { 
            std::cerr << "Invalid duration in line: " << line << std::endl;
            return std::nullopt; 
        }

        return std::make_tuple(description, duration, start, end);

    } 
    catch (const std::runtime_error& e) 
    {
        std::cerr << "Error parsing time in line: " << line << " - " << e.what() << std::endl;
        return std::nullopt;
    }
}



void Statistics::loadStatisticsFromFile(const std::string& fileName) 
{
    std::ifstream inFile(fileName);                                          

    if (!inFile.is_open())                                                  
    {
        std::cerr << "Couldn't open file: " << fileName << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) 
    {
        auto sessionData = parseSessionLine(line); 

        if (sessionData) 
        {
            const auto& [description, duration, start, end] = *sessionData;  
            std::unique_lock<std::mutex> lock(sessionsMutex);                        
            sessions.push_back({description, duration, start, end});                    
        }
    }
}



void Statistics::printStatistics(const std::string& fileName) 
{
    std::unique_lock<std::mutex> lock(sessionsMutex);                       
    if (sessions.empty())
    {
        std::cout << "Statistics are empty.\n";
        return;
    }

    std::cout << "=====Statistics=====\n";
    std::cout << std::left
        << std::setw(25) << "Task"
        << std::setw(25) << "Duration"
        << std::setw(25) << "Start Time"
        << std::setw(25) << "End Time";
    std::cout << std::endl;

    for (const auto& session : sessions)
    {
        std::string startTimeStr = formatTimePoint(session.getSessionStartTime());
        std::string endTimeStr = formatTimePoint(session.getSessionEndTime());

        std::cout << std::setw(25) << session.getTaskDescription()
            << std::setw(25) << session.getDurationMinutes()
            << std::setw(25) << startTimeStr
            << std::setw(25) << endTimeStr << std::endl;
    }
}












