#include "Gameplay.h"

Gameplay::Gameplay()
{

}

// CreateRobot
void Gameplay::AddRobot()
{

    RoboTank *pNewRobot;

    pNewRobot = new RoboTank(TypeChassis::Medium, TypeGun::Medium);

    // Добавляем новый танк в список
    RobotsEnemy.push_back(pNewRobot);

    // Создаем физику танка

}

void Gameplay::Update()
{

}
