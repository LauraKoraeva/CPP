#include <iostream>
#include <string>
#include <iomanip>
#include "UI.h"
#include "Session.h"
#include "Config.h"
#include "Utility.h"

UI::UI(Timer& inTimer, Statistics& inStatistics) : timer(inTimer), statistics(inStatistics) { }


void UI::displayMenu() 
{
    std::cout << "-----------------------\n";
    std::cout << "    Focus Timer MENU   \n";
    std::cout << "-----------------------\n";
    std::cout << "1. Start focus session\n";
    std::cout << "2. Pause focus session\n"; 
    std::cout << "3. Resume focus session\n";   
    std::cout << "4. Stop focus session\n";   
    std::cout << "5. Show statistics\n";  
    std::cout << "6. Exit\n";     
    std::cout << "-----------------------\n";
}


void UI::startFocusSession()
{
    if (timer.is_Running())
    {
        std::cout << "Timer is running. Stop the current session first.\n";
        return;
    }

    Session currentSession;

    std::string description;
    std::cout << "Description: ";
    std::cin.ignore();
    std::getline(std::cin, description);
    currentSession.setTaskDescription(description);

    int duration;
    std::cout << "Focus time: ";
    do
    {
    std::cin >> duration; 
    } while (!correctInput(duration));
    currentSession.setDurationMinutes(duration);
    
    timer.start(currentSession, [this, currentSession](Session& s) 
{
    statistics.recordSession(s, "Focus_Statistics.txt");
}); 
}


void UI::pauseFocusSession()
{
    timer.pause();
}

void UI::resumeFocusSession()
{
    timer.resume();
}


void UI::stopFocusSession()
{
    timer.stop();
}


void UI::showStatistics()
{
    statistics.printStatistics(Config::getStatisticsFileName());
}


void UI::run()
{
    int operation;
    do
    {
        displayMenu();
        do
        {
            std::cout << "Choose the operation: ";
            std::cin >> operation; 
        } while (!correctInput(operation));

        switch (operation) 
        {
        case 1:
            startFocusSession();
            break;
        case 2:
            pauseFocusSession();
            break;
        case 3:
            resumeFocusSession();
            break;
        case 4:
            stopFocusSession();
            break;
        case 5:
            showStatistics();
            break;
        case 6:
            statistics.saveStatisticsToFile("Focus_Statistics.txt");
            std::cout << "Exit." << std::endl;
            break;
        default:
            std::cout << "Incorrect input." << std::endl;
        }
    } while (operation != 6);
}


