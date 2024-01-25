#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "Screen.h"

enum class MenuChoice {None, Start, Options, Quit};

class TitleScreen : public Screen
{
    //bool startClicked = false;
    //bool quit = false;

    //MenuChoice Choice {MenuChoice::None};
    MenuChoice Choice = MenuChoice::None;

public:
    TitleScreen();

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

extern TitleScreen *pTitleScreen;

#endif // TITLESCREEN_H
