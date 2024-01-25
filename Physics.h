#ifndef PHYSICS_H
#define PHYSICS_H

#include "box2d.h"


class Physics
{
public:

    // Тело шасси
    b2Body* bodyPlayer;

    // Тело башни
    b2Body* bodyGun;

    // Тело снаряда
    b2Body* bodyBullet;

    // Тело шасси
    b2BodyDef bd;

    // Тело башни
    b2BodyDef bdGun;

    // Тело снаряда
    b2BodyDef bdBullet;


    float kScale = 10;

    b2World* m_world;
    b2Vec2 gravity;

    Physics();

//    void initPhysics(b2World *m_world,
//                     RoboTank &playerRoboTank,
//                     b2Body *bodyPlayer,
//                     b2Body *bodyGun,
//                     b2Body* bodyBullet,
//                     std::vector<std::vector<p2t::Triangle*>> &trianglesHoles,
//                     std::vector<p2t::Point*> &polyline);

    void initPhysics();

    b2Vec2 getLateralVelocity(b2Body* m_body);
    b2Vec2 getForwardVelocity(b2Body* m_body);
    void updateFriction(b2Body* m_body);

    void LaunchBullet();

};

extern Physics *pGamePhysics;

#endif // PHYSICS_H
