#include "Chassis.h"
#include "Gun.h"

#include "LogoScreen.h"
#include "TitleScreen.h"
#include "GameplayScreen.h"
#include "GameState.h"
#include "Robot.h"
#include "Physics.h"
#include "Map.h"

Chassis *ChassisSmall, *ChassisMedium, *ChassisBig;
Gun *GunSmall, *GunMedium, *GunBig;

LogoScreen *pLogoScreen;
TitleScreen *pTitleScreen;
GameplayScreen *pGameplayScreen;
GameState *pGameState;
RoboTank *pPlayerRoboTank;
Physics *pGamePhysics;
Map *pGameMap;

