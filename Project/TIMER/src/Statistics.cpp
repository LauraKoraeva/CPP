#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include "Statistics.h"
#include "Session.h"


std::string Statistics::formatTimePoint(const std::chrono::system_clock::time_point& timePoint, const std::string& format = "%d-%m-%Y %H:%M:%S") const
{
	auto time = std::chrono::system_clock::to_time_t(timePoint);
	std::tm buffer;
#ifdef _WIN32
	localtime_s(&buffer, &time); 
#else
	localtime_r(&time, &buffer); 
#endif

	std::stringstream ss;
	ss << std::put_time(&buffer, format.c_str());
	return ss.str();
}





void Statistics::recordSession(Session inSession, const std::string& fileName) // /////////////////////
{
    sessions.push_back(inSession);


    std::ofstream outFile;
    outFile.open(fileName, std::ios::app);

    if (outFile.is_open())
    {
        std::string startTime = formatTimePoint(inSession.sessionStartTime);
        std::string finishTime = formatTimePoint(inSession.sessionFinishTime);
        outFile << inSession.taskDescription << "," << inSession.durationMinutes << "," << startTime << "," << finishTime << std::endl;
        outFile.close();
        std::cout << "Session is recorded and saved to the file " << std::quoted(fileName) << '\n';
    }
    else
    {
        std::cerr << "Couldn't open the file " << std::quoted(fileName) << '\n';
    }


}








void Statistics::saveStatisticsToFile(const std::string& fileName) const
{
    std::ofstream outFile(fileName);

    if (outFile.is_open())
    {
        for (const auto& session : sessions)
        {
            std::string startTime = formatTimePoint(session.sessionStartTime);
            std::string finishTime = formatTimePoint(session.sessionFinishTime);
            outFile << session.taskDescription << "," << session.durationMinutes << "," << startTime << "," << finishTime << std::endl;
        }
        outFile.close();
        std::cout << "Statistics are saved to the file " << std::quoted(fileName) << '\n';
    }
    else
    {
        std::cerr << "Couldn't open the file " << std::quoted(fileName) << '\n';
    }
}



time_t Statistics::stringToTimeT(const std::string& timeString, const std::string& format = "%d-%m-%Y %H:%M:%S") const
{
	std::tm t{};
	std::istringstream ss(timeString);
	ss >> std::get_time(&t, format.c_str());

	if (ss.fail())
	{
		std::cerr << "Error converting string to time_t\n";
		return 1;
	}

	return mktime(&t);
}


void Statistics::loadStatisticsFromFile(const std::string& fileName)
{
    std::ifstream inFile(fileName);

    if (inFile.is_open())
    {
        std::string line;

        while(std::getline(inFile, line))
        {
            std::stringstream ss(line);

            std::string description;
            int duration;
            std::string startTimeString;
            std::string endTimeString;

            std::getline(ss, description, ',');
            ss >> duration;
            ss.ignore(1);
            std::getline(ss, startTimeString, ',');
            std::getline(ss, endTimeString, ',');



            
            std::time_t start_time_t = stringToTimeT(startTimeString);
            std::time_t end_time_t = stringToTimeT(endTimeString);


            std::chrono::system_clock::time_point start = std::chrono::system_clock::from_time_t(start_time_t);
            std::chrono::system_clock::time_point end = std::chrono::system_clock::from_time_t(end_time_t);

            sessions.push_back({description, duration, start, end});

            
        }
    }
    else
    {
        std::cout << "Couldn't open the file.\n";
    }
}





void Statistics::printStatistics(const std::string& fileName) //const
{
    // loadStatisticsFromFile(fileName);
    if (sessions.empty())
    {
        std::cout << "Statistics are empty.\n";
        return;
    }

    std::cout << "=====Statistics=====\n";
    std::cout << std::left << 
        std::setw(25) << "Task" <<
        std::setw(25) << "Duration" <<
        std::setw(25) << "Start Time" << 
        std::setw(25) << "End Time";
    std::cout << std::endl;

    for (const auto& session : sessions)
    {
        std::time_t start_time_t = std::chrono::system_clock::to_time_t(session.sessionStartTime);
        std::time_t end_time_t = std::chrono::system_clock::to_time_t(session.sessionFinishTime);

        std::tm* startTimeInfo = std::localtime(&start_time_t);
        char startBuffer[80];
        std::strftime(startBuffer, sizeof(startBuffer), "%d-%m-%Y %H:%M", startTimeInfo);

        std::tm* endTimeInfo = std::localtime(&end_time_t);
        char endBuffer[80];
        std::strftime(endBuffer, sizeof(endBuffer), "%d-%m-%Y %H:%M", endTimeInfo);
        std::cout << std::setw(25) << session.taskDescription <<
            std::setw(25) << session.durationMinutes <<
            std::setw(25) << startBuffer <<
            std::setw(25) << endBuffer << std::endl;
            
    }
}















