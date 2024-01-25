#include "TitleScreen.h"

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define TITLE_FONT_SIZE 160
#define TITLE_FONT_SPACING 10.0f
#define MENU_FONT_SIZE 40

#define BUTTON_WIDTH 280
#define BUTTON_HEIGHT 70
#define BUTTON_SPACING (BUTTON_HEIGHT + 10)

TitleScreen::TitleScreen()
{

}

// Gameplay Screen Initialization logic
void TitleScreen::Init(void) {
    // TODO: Initialize TITLE screen variables here!
    framesCounter = 0;
    finishScreen = 0;

}

// Gameplay Screen Update logic
void TitleScreen::Update(void) {
    // TODO: Update TITLE screen variables here!

    // Press enter or tap to change to GAMEPLAY screen
//    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
//    {
//        //finishScreen = 1;   // OPTIONS
//        finishScreen = 2;   // GAMEPLAY
//        //PlaySound(fxCoin);
//    }

    switch (Choice) {
    case MenuChoice::Start:
    {
        finishScreen = 1;
        break;
    }
    case MenuChoice::Options:
    {
        //finishScreen = 2;
        finishScreen = 3;
        break;
    }
    case MenuChoice::Quit:
    {
        finishScreen = 3;
        break;
    }
    default:
    {
        break;
    }
    }

}

// Gameplay Screen Draw logic
void TitleScreen::Draw(void) {
    // TODO: Draw TITLE screen here!
    //DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);
    Vector2 pos = { 20, 10 };
    //DrawTextEx(font, "TITLE SCREEN", pos, font.baseSize*3.0f, 4, DARKGREEN);
    //DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);

    int menuLeft = (GetScreenWidth() - BUTTON_WIDTH) / 2;
    int menuTop = GetScreenHeight() / 2 - BUTTON_HEIGHT*2;
    float currX = (float)menuLeft;
    float currY = (float)menuTop;

    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(DEFAULT, TEXT_SIZE, MENU_FONT_SIZE);
    if (GuiButton((Rectangle){ currX, currY, BUTTON_WIDTH, BUTTON_HEIGHT }, "Start Game")) {
        Choice = MenuChoice::Start;
    }
    currY += BUTTON_SPACING;
    if (GuiButton((Rectangle){ currX, currY, BUTTON_WIDTH, BUTTON_HEIGHT }, "Options")) {
        Choice = MenuChoice::Options;
    }
    currY += BUTTON_SPACING;
    if (GuiButton((Rectangle){ currX, currY, BUTTON_WIDTH, BUTTON_HEIGHT }, "Quit")) {
        Choice = MenuChoice::Quit;
    }

}

// Gameplay Screen Unload logic
void TitleScreen::Unload(void) {
    // TODO: Unload TITLE screen variables here!

}

// Gameplay Screen should finish?
int TitleScreen::Finish(void) {
    return finishScreen;
}
