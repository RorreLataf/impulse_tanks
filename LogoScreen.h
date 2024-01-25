#ifndef LOGOSCREEN_H
#define LOGOSCREEN_H

#include "Screen.h"

//TODO Сделать чтобы логотип рисовал двузвенный робот

class LogoScreen : public Screen
{
//    int framesCounter = 0;
//    int finishScreen = 0;

    int logoPositionX = 0;
    int logoPositionY = 0;

    int lettersCount = 0;

    int topSideRecWidth = 0;
    int leftSideRecHeight = 0;

    int bottomSideRecWidth = 0;
    int rightSideRecHeight = 0;

    int state = 0;              // Logo animation states
    float alpha = 1.0f;         // Useful for fading

public:
    LogoScreen();

    // Gameplay Screen Initialization logic
    void Init(void) override;

    // Gameplay Screen Update logic
    void Update(void) override;

    // Gameplay Screen Draw logic
    void Draw(void) override;

    // Gameplay Screen Unload logic
    void Unload(void) override;

    // Gameplay Screen should finish?
    int Finish(void) override;

};

extern LogoScreen *pLogoScreen;

#endif // LOGOSCREEN_H
