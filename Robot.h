#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>

#include "GlobalParams.h"
#include "Unit.h"
#include "Chassis.h"
#include "Gun.h"

#include "raylib.h"


class RoboTank : public Unit
{
    // Тип шасси
    TypeChassis chassisType;
    Chassis *chassis;

    // Тип пушки
    TypeGun gunType;
    Gun *gun;

    Vector2 position; // координаты положения шасси

    float alphaChassis; // угол поворота шасси

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

        position = {0, 0};
        alphaChassis = 0;
        alphaGun = 0;

        // ====================================================================

        mass = chassis->mass + gun->mass;
        //this->mass = chassis->mass + gun->mass;

        // ====================================================================

    }

    Vector2 getPosition() {
        return this->position;
    }

    void setPosition(Vector2 pos) {
        this->position = pos;
    }

    float getAngleChassis() {
        return this->alphaChassis;
    }

    void setAngleChassis(float angle) {
        this->alphaChassis = angle;
    }

    float getAngleGun() {
        return this->alphaGun;
    }

    void setAngleGun(float angle) {
        this->alphaGun = angle;
    }

    float getWidthChassis() {
        return this->chassis->width;
    }

    float getLengthChassis() {
        return this->chassis->length;
    }


//    std::string Voice() const override {
//        return "Meow!";
//    }
};


#endif // ROBOT_H
