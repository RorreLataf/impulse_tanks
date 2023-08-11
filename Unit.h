#ifndef UNIT_H
#define UNIT_H

#include <string>

// Абстрактный класс
class Unit
{
//private:
    // Координаты определяющие положение
    float x;
    float y;
    float alpha; // угол поворота

    int health;

    // Связанное с юнитом тело в физическом движке
    // body


public:
    Unit();
    // виртуальные функции
//    virtual std::string Voice() const = 0; // Чисто-виртуальная функция, поэтому класс получается абстрактным. Объекты этого класса нельзся создать.

//    virtual ~Unit() {

//    }
};

#endif // UNIT_H
