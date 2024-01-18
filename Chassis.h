#ifndef CHASSIS_H
#define CHASSIS_H

#include <iostream>

enum class TypeChassis {
    Small,
    Medium,
    Big,
};

// Простейший класс шасси, объектами этого класса будут являться различные шасси различных размеров
struct Chassis
{
    TypeChassis type;

    float length;
    float width;

    float mass;

    Chassis(TypeChassis type) {
        if (type == TypeChassis::Small) {
            // Эти параметры можно считывать из файла
            length = 100.0f;
            width = 100.0f;
            mass = 10.0f;
        }
        else if (type == TypeChassis::Medium) {
//            length = 200.0f;
//            width = 100.0f;
            //length = 32.0f;
            //length = 24.0f;
            length = 20.0f;
            width = 16.0f;
            mass = 20.0f;
        }
        else if (type == TypeChassis::Big) {
            length = 200.0f;
            width = 200.0f;
            mass = 40.0f;
        }
        else {
            std::cout << "ERROR: Chassis::Chassis(): unknown TypeChassis." << std::endl;
        }
    }
};

// class SmallChassis : public Chassis
// {
//     // Какие то уникальные свойства

// public:
//     SmallChassis();
// };

#endif // CHASSIS_H
