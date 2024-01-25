#include "Init.h"
#include "GlobalParams.h"
#include "Robot.h"

void Init()
{
    ChassisSmall = new Chassis(TypeChassis::Small);
    ChassisMedium = new Chassis(TypeChassis::Medium);
    ChassisBig = new Chassis(TypeChassis::Big);

    GunSmall = new Gun(TypeGun::Small);
    GunMedium = new Gun(TypeGun::Medium);
    GunBig = new Gun(TypeGun::Big);




    //playerRoboTank = new RoboTank(TypeChassis::Medium, TypeGun::Medium);

}
