#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "raylib.h"

#include "Physics.h"
#include "Map.h"

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { UNKNOWN = -1, LOGO = 0, TITLE, OPTIONS, GAMEPLAY, ENDING } GameScreen;

class GameState
{
    // TODO
    // Переменные box2d
    // Полигоны и т.д.




public:

    const int screenWidth = 1024;
    const int screenHeight = 768;
    // TODO Сделать настраиваемое разрешение

    // Required variables to manage screen transitions (fade-in, fade-out)
    float transAlpha = 0.0f;
    bool onTransition = false;
    bool transFadeOut = false;
    int transFromScreen = -1;
    GameScreen transToScreen = UNKNOWN;
    GameScreen currentScreen = LOGO;

    GameState();

    //----------------------------------------------------------------------------------
    // Module specific Functions Definition
    //----------------------------------------------------------------------------------
    // Change to next screen, no transition
    void ChangeToScreen(GameScreen screen);

    // Request transition to next screen
    void TransitionToScreen(GameScreen screen);
    // Update transition effect (fade-in, fade-out)
    void UpdateTransition(void);
    // Draw transition effect (full-screen rectangle)
    void DrawTransition(void);
};

extern GameState *pGameState;

#endif // GAMESTATE_H
