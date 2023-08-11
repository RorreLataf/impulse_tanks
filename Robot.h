#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>

#include "GlobalParams.h"
#include "Unit.h"
#include "Chassis.h"
#include "Gun.h"


class RoboTank : public Unit
{
    // Тип шасси
    TypeChassis chassisType;
    Chassis *chassis;

    // Тип пушки
    TypeGun gunType;
    Gun *gun;


    float alphaGun; // угол поворота пушки

    float mass;


public:

    RoboTank(TypeChassis typeChassis, TypeGun typeGun) : chassisType(typeChassis), gunType(typeGun) { // Конструктор ничего не возвращает

        if (typeChassis == TypeChassis::Small) {
            chassis = ChassisSmall;
        }
        else if (typeChassis == TypeChassis::Medium) {
            chassis = ChassisMedium;
        }
        else if (typeChassis == TypeChassis::Big) {
            chassis = ChassisBig;
        }
        else {
            std::cout << "ERROR: RoboTank::RoboTank(): unknown TypeChassis." << std::endl;
        }

        // ====================================================================

        if (typeGun == TypeGun::Small) {
            gun = GunSmall;
        }
        else if (typeGun == TypeGun::Medium) {
            gun = GunMedium;
        }
        else if (typeGun == TypeGun::Big) {
            gun = GunBig;
        }
        else {
            std::cout << "ERROR: RoboTank::RoboTank(): unknown TypeGun." << std::endl;
        }

        // ====================================================================

        //x = 0;

        alphaGun = 0;

        // ====================================================================

        mass = chassis->mass + gun->mass;
        //this->mass = chassis->mass + gun->mass;

        // ====================================================================

    }

//    std::string Voice() const override {
//        return "Meow!";
//    }
};


#endif // ROBOT_H
