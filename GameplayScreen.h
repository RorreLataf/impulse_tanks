#ifndef GAMEPLAYSCREEN_H
#define GAMEPLAYSCREEN_H

#include "Screen.h"
#include "raylib.h"

class GameplayScreen : public Screen
{

    // Установка начального положения танка
    //----
    // TODO Вектор должен браться из playerRoboTank
    Vector2 startPosition;
    //----

    float player_angle = 0.0f;

    Camera2D camera = { 0 };

    Vector2 ballPosition = { -100.0f, -100.0f };
    Color ballColor = DARKBLUE;

    //--------------------------------------------------------------------------------------
    // Загрузка текстур
    Texture2D textureRoboTank;
    Texture2D textureRoboTank2;
    Texture2D textureGrass;
    Texture2D textureStone;

    int frameWidth;
    int frameHeight;

    // Source rectangle (part of the texture to use for drawing)
    Rectangle sourceRec;
    Rectangle sourceRec2;
    Rectangle rectSourceTextureGrass;

    // Destination rectangle (screen rectangle where drawing part of texture)
    Rectangle destRec;

    // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
    Vector2 origin;

    float angle = 0.0f;


    int timer = 0;
    int isTimerStart = 0;

public:
    GameplayScreen();

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

extern GameplayScreen *pGameplayScreen;

#endif // GAMEPLAYSCREEN_H
