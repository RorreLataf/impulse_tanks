#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <cmath>
//#include <cstddef>
//#include <stdexcept>
#include <vector>
#include <list>

#include "Robot.h"

class Gameplay
{


public:

    // Список вражеских роботов
    std::list<RoboTank *> RobotsEnemy;

    Gameplay();


    void AddRobot();
    void Update();
};

#endif // GAMEPLAY_H
