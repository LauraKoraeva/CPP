#include "UI.h"
#include "Timer.h"
#include "Statistics.h"


int main() 
{
    Timer timer;
    Statistics statistics;

    statistics.loadStatisticsFromFile("Focus_Statistics.txt");
    

    

    UI ui(timer, statistics);
    ui.run();

    return 0;
}