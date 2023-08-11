#ifndef GUN_H
#define GUN_H

#include <iostream>
enum class TypeGun {
    Small,
    Medium,
    Big,
};

// Простейший класс пушек, объектами этого класса будут являться различные пушки
struct Gun
{
    TypeGun type;

    float length;
    float width;

    float range; //Range of the shot

    float mass;

    float projectile_velocity;


    Gun(TypeGun type) {
        if (type == TypeGun::Small) {
            // Эти параметры можно считывать из файла
            length = 64.0f;
            width = 64.0f;
            mass = 5.0f;
        }
        else if (type == TypeGun::Medium) {
            length = 80.0f;
            width = 80.0f;
            mass = 7.0f;
        }
        else if (type == TypeGun::Big) {
            length = 96.0f;
            width = 96.0f;
            mass = 10.0f;
        }
        else {
            std::cout << "ERROR: Gun::Gun(): unknown TypeGun." << std::endl;
        }
    }
};

#endif // GUN_H
