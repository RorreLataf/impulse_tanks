#include "GameState.h"
#include "LogoScreen.h"
#include "TitleScreen.h"
#include "GameplayScreen.h"

GameState::GameState()
{

}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
// Change to next screen, no transition
void GameState::ChangeToScreen(GameScreen screen)
{
    // Unload current screen
    switch (currentScreen)
    {
        case LOGO: pLogoScreen->Unload(); break;
        case TITLE: pTitleScreen->Unload(); break;
        case GAMEPLAY: pGameplayScreen->Unload(); break;
        //case ENDING: UnloadEndingScreen(); break;
        default: break;
    }

    // Init next screen
    switch (screen)
    {
        case LOGO: pLogoScreen->Init(); break;
        case TITLE: pTitleScreen->Init(); break;
        case GAMEPLAY: pGameplayScreen->Init(); break;
        //case ENDING: InitEndingScreen(); break;
        default: break;
    }

    currentScreen = screen;
}

// Request transition to next screen
void GameState::TransitionToScreen(GameScreen screen)
{
    onTransition = true;
    transFadeOut = false;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transAlpha = 0.0f;
}

// Update transition effect (fade-in, fade-out)
void GameState::UpdateTransition(void)
{
    if (!transFadeOut)
    {
        transAlpha += 0.05f;

        // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
        // For that reason we compare against 1.01f, to avoid last frame loading stop
        if (transAlpha > 1.01f)
        {
            transAlpha = 1.0f;

            // Unload current screen
            switch (transFromScreen)
            {
                case LOGO: pLogoScreen->Unload(); break;
                case TITLE: pTitleScreen->Unload(); break;
                //case OPTIONS: UnloadOptionsScreen(); break;
                case GAMEPLAY: pGameplayScreen->Unload(); break;
                //case ENDING: UnloadEndingScreen(); break;
                default: break;
            }

            // Load next screen
            switch (transToScreen)
            {
                case LOGO: pLogoScreen->Init(); break;
                case TITLE: pTitleScreen->Init(); break;
                case GAMEPLAY: pGameplayScreen->Init(); break;
                //case ENDING: InitEndingScreen(); break;
                default: break;
            }

            currentScreen = transToScreen;

            // Activate fade out effect to next loaded screen
            transFadeOut = true;
        }
    }
    else  // Transition fade out logic
    {
        transAlpha -= 0.02f;

        if (transAlpha < -0.01f)
        {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = UNKNOWN;
        }
    }
}

// Draw transition effect (full-screen rectangle)
void GameState::DrawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}
