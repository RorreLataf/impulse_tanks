#include <iostream>
#include <fstream>
#include <cmath>

#include "poly2tri.h"

#include "box2d.h"

// --------------------------
#include "Init.h"
#include "GlobalParams.h"

#include "GameState.h"
#include "LogoScreen.h"
#include "TitleScreen.h"
#include "GameplayScreen.h"
#include "Robot.h"
#include "Physics.h"

// --------------------------

//#define PLATFORM_WEB
// Вместо этого в CMakeLists.txt добавлено add_compile_definitions(PLATFORM_WEB)

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

void UpdateDrawFrame(void);     // Update and Draw one frame

int main()
{
    //--------------------------------------------------------------------------------------


    //--------------------------------------------------------------------------------------

    Init();

    GameState GameSt;
    pGameState = &GameSt;

    RoboTank playerRoboTank(TypeChassis::Medium, TypeGun::Medium);
    pPlayerRoboTank = &playerRoboTank;

    Physics GamePhysics;
    pGamePhysics = &GamePhysics;

    Map GameMap;
    pGameMap = &GameMap;


    //playerRoboTank.getPosition();


    //--------------------------------------------------------------------------------------
    GameMap.readMap();
    //--------------------------------------------------------------------------------------
    // Треугольники для отрисовки полигонов-дырок
    GameMap.triangulateMap();
    //--------------------------------------------------------------------------------------


    // Установка начального положения танка
    //----
    // TODO Вектор должен браться из playerRoboTank
    Vector2 startPosition = {(float)(*(pGameMap->steiner[0])).x, (float)(*(pGameMap->steiner[0])).y};
    playerRoboTank.setPosition(startPosition);
    //----

    // TODO Сделать класс - состояние игры
    //--------------------------------------------------------------------------------------


    //--------------------------------------------------------------------------------------
    // Initialization
    //--------------------------------------------------------------------------------------
    // TODO Сделать настраиваемое разрешение


    // RoboTank в виде квадратика

    //Rectangle player = {(float)(*steiner[0]).x - 8, (float)(*steiner[0]).y - 8, 16, 16};

    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(pGameState->screenWidth, pGameState->screenHeight, "project rts");

    // Setup and init first screen

    //GameState Game;
    //Game.currentScreen = LOGO;
    //pGameState->currentScreen = GAMEPLAY;

    LogoScreen ScreenLogo;
    pLogoScreen = &ScreenLogo;

    TitleScreen ScreenTitle;
    pTitleScreen = &ScreenTitle;

    GameplayScreen ScreenGameplay;
    pGameplayScreen = &ScreenGameplay;

    pGameState->currentScreen = LOGO;
    ScreenLogo.Init();

    //ScreenGameplay.InitLogoScreen();
    //ScreenGameplay.Init();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unload current screen data before closing
    switch (pGameState->currentScreen)
    {
        case LOGO: ScreenLogo.Unload(); break;
        //case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: ScreenGameplay.Unload(); break;
        //case ENDING: UnloadEndingScreen(); break;
        default: break;
    }

    // Unload global data loaded
    //UnloadFont(font);
    //UnloadMusicStream(music);
    //UnloadSound(fxCoin);

    //CloseAudioDevice();     // Close audio context

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


void UpdateDrawFrame(){

    // Update
    //----------------------------------------------------------------------------------
    //UpdateMusicStream(music);       // NOTE: Music keeps playing between screens

    if (!pGameState->onTransition)
    {
        switch(pGameState->currentScreen)
        {
        case LOGO:
        {
            pLogoScreen->Update();

            if (pLogoScreen->Finish())
                pGameState->TransitionToScreen(TITLE);

        } break;
        case TITLE:
        {
            pTitleScreen->Update();

            if (pTitleScreen->Finish() == 1) pGameState->TransitionToScreen(GAMEPLAY);
            else if (pTitleScreen->Finish() == 2) pGameState->TransitionToScreen(OPTIONS);

        } break;
//        case OPTIONS:
//        {
//            UpdateOptionsScreen();

//            if (FinishOptionsScreen()) TransitionToScreen(TITLE);

//        } break;
        case GAMEPLAY:
        {
            pGameplayScreen->Update();

            if (pGameplayScreen->Finish() == 1) pGameState->TransitionToScreen(LOGO);
            //else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);

        } break;
//        case ENDING:
//        {
//            UpdateEndingScreen();

//            if (FinishEndingScreen() == 1) TransitionToScreen(TITLE);

//        } break;
        default: break;
        }
    }
    else pGameState->UpdateTransition();    // Update transition (fade-in, fade-out)
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    switch(pGameState->currentScreen)
    {
    case LOGO: pLogoScreen->Draw(); break;
    case TITLE: pTitleScreen->Draw(); break;
//    case OPTIONS: DrawOptionsScreen(); break;
    case GAMEPLAY: pGameplayScreen->Draw(); break;
//    case ENDING: DrawEndingScreen(); break;
    default: break;
    }

    // Draw full screen rectangle in front of everything
    if (pGameState->onTransition) pGameState->DrawTransition();

    //DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------

}

//public List<Point> sortVerticies(List<Point> points) {
//    // get centroid
//    Point center = findCentroid(points);
//    Collections.sort(points, (a, b) -> {
//        double a1 = (Math.toDegrees(Math.atan2(a.x - center.x, a.y - center.y)) + 360) % 360;
//        double a2 = (Math.toDegrees(Math.atan2(b.x - center.x, b.y - center.y)) + 360) % 360;
//        return (int) (a1 - a2);
//    });
//    return points;
//}

// TODO таймеры в raylib

// Box2d
// Чтобы получилось препятствие полигон, надо склеить вместе треугольники и назначить это одним телом.
//

// TODO
// Управление танком игрока с клавиатуры

// TODO
// Бонусы ускорения и т.д.

//



