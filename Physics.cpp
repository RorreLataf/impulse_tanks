#include "Physics.h"
//#include "GlobalParams.h"
#include "Robot.h"
#include "Map.h"

Physics::Physics()
{

}

//void Physics::initPhysics(b2World* m_world,
//                 //Vector2 player,
//                 RoboTank& playerRoboTank,
//                 b2Body* bodyPlayer,
//                 b2Body* bodyGun,
//                 b2Body* bodyBullet,
//                 std::vector<std::vector<p2t::Triangle*>> &trianglesHoles,
//                 std::vector<p2t::Point*> &polyline) {

void Physics::initPhysics() {

    //--------------------------------------------------------------------------------------
    // Работа с box2d
    //--------------------------------------------------------------------------------------

    gravity.Set(0.0f, 0.0f);
    m_world = new b2World(gravity);

    bd.type = b2_dynamicBody;
    bd.position.Set((*pPlayerRoboTank).getPosition().x/kScale, (*pPlayerRoboTank).getPosition().y/kScale);
    bd.angle = -0.5f * b2_pi;
    bodyPlayer = m_world->CreateBody(&bd);

    bdGun.type = b2_dynamicBody;
    bdGun.position.Set((*pPlayerRoboTank).getPosition().x/kScale, (*pPlayerRoboTank).getPosition().y/kScale);
    bodyGun = m_world->CreateBody(&bdGun);

    bdBullet.type = b2_dynamicBody;
    //bdBullet.position.Set(playerRoboTank.getPosition().x/kScale, playerRoboTank.getPosition().y/kScale);
    bdBullet.position.Set(10/kScale, 10/kScale);
    bdBullet.bullet = true;
    bodyBullet = m_world->CreateBody(&bdBullet);

    //TODO Eсли перенести b2BodyDef в initPhysics то будет segfault, почему???

    //b2Vec2 gravity;
//    gravity.Set(0.0f, -10.0f);
    //gravity.Set(0.0f, 0.0f);

    //b2World* m_world;
    //m_world = new b2World(gravity);

    // Создание границ игрового поля 1024*1024
    //b2Body** bodies = (b2Body**)b2Alloc(2 * sizeof(b2Body*));
    b2Body *bodyBox;
    {
        b2BodyDef bd;
        bd.type = b2BodyType(0);
        //bodies[0] = m_world->CreateBody(&bd);
        bodyBox = m_world->CreateBody(&bd);
        {
//            b2Vec2 v1(0.0f, 0.0f);
//            b2Vec2 v2(1024.0f/kScale, 0.0f);
//            b2Vec2 v3(1024.0f/kScale, 1024.0f/kScale);
//            b2Vec2 v4(0.0f, 1024.0f/kScale);

            b2Vec2 v1((*pGameMap).polyline[0]->x/kScale, (*pGameMap).polyline[0]->y/kScale);
            b2Vec2 v2((*pGameMap).polyline[1]->x/kScale, (*pGameMap).polyline[1]->y/kScale);
            b2Vec2 v3((*pGameMap).polyline[2]->x/kScale, (*pGameMap).polyline[2]->y/kScale);
            b2Vec2 v4((*pGameMap).polyline[3]->x/kScale, (*pGameMap).polyline[3]->y/kScale);

            b2EdgeShape shape;
            b2FixtureDef fd;
            fd.shape = &shape;
            fd.restitutionThreshold = 0.01f;
            bodyBox->CreateFixture(&fd);

            shape.SetTwoSided(v1, v2);
            //bodies[0]->CreateFixture(&shape, 0.0f);
            bodyBox->CreateFixture(&shape, 0.0f);

            shape.SetTwoSided(v2, v3);
            //bodies[0]->CreateFixture(&shape, 0.0f);
            bodyBox->CreateFixture(&shape, 0.0f);

            shape.SetTwoSided(v3, v4);
            bodyBox->CreateFixture(&shape, 0.0f);

            shape.SetTwoSided(v4, v1);
            bodyBox->CreateFixture(&shape, 0.0f);
        }
    }

    //-----------------------------------------------------

    // Создание динамического тела шасси танка

//    // Тело шасси
//    b2BodyDef bd;
//    bd.type = b2_dynamicBody;
//    bd.position.Set(playerRoboTank.getPosition().x/kScale, playerRoboTank.getPosition().y/kScale);
//    bd.angle = -0.5f * b2_pi;
//    bodyPlayer = m_world->CreateBody(&bd);

//    // Тело башни
//    b2BodyDef bdGun;
//    bdGun.type = b2_dynamicBody;
//    bdGun.position.Set(playerRoboTank.getPosition().x/kScale, playerRoboTank.getPosition().y/kScale);
//    bodyGun = m_world->CreateBody(&bdGun);

    // Тело снаряда
//    b2BodyDef bdBullet;
//    bdBullet.type = b2_dynamicBody;
//    //bdBullet.position.Set(playerRoboTank.getPosition().x/kScale, playerRoboTank.getPosition().y/kScale);
//    bdBullet.position.Set(10/kScale, 10/kScale);
//    bdBullet.bullet = true;
//    bodyBullet = m_world->CreateBody(&bdBullet);

    //-----------------------------------------------------


    // Задание геометрии и размеров
    float widthChassis = (*pPlayerRoboTank).getWidthChassis() / (float)kScale;
    float lengthChassis = (*pPlayerRoboTank).getLengthChassis() / (float)kScale;
    // У прямоугольника надо задавать половину ширины и длины

    b2PolygonShape shape;
    shape.SetAsBox(lengthChassis/(2.0f), widthChassis/(2.0f)); // TODO Заменить деление умножением
    //shape.SetAsBox(lengthChassis, widthChassis); // TODO Заменить деление умножением
    //b2CircleShape shape;
    //shape.m_radius = 8.0f/kScale;


    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.restitution = 0.1f;
    fd.restitutionThreshold = 0.1f; // Порог при котором еще происходит отскакивание
    fd.filter.groupIndex = -1;

    //b2BodyDef bd;
    //bd.type = b2_dynamicBody;

//    bd.position.Set(-40.0f,5.0f);
    //bd.position.Set(player.x/kScale, player.y/kScale);

//    bd.bullet = true; // Что это значит?

    //b2Body* bodyPlayer = m_world->CreateBody(&bd);
    bodyPlayer->CreateFixture(&fd);
    //bodyPlayer->SetLinearVelocity(b2Vec2(-12.0f, -2.0f));

    //bodyPlayer->

    //-----------------------------------------------------
    // Динамическое тело башни танка

    // Задание геометрии и размеров

    // У прямоугольника надо задавать половину ширины и длины

    //b2PolygonShape shapeGun;
    //shapeGun.SetAsBox(lengthChassis/(2.0f), widthChassis/(2.0f)); // TODO Заменить деление умножением
    //shapeGun.SetAsBox(lengthChassis, widthChassis); // TODO Заменить деление умножением
    b2CircleShape shapeGun;
    shapeGun.m_radius = ((*pPlayerRoboTank).getWidthChassis()/2.0f - 1) / (float)kScale;

    b2FixtureDef fdGun;
    fdGun.shape = &shapeGun;
    fdGun.density = 1.0f;
    fdGun.restitution = 0.1f;
    fdGun.restitutionThreshold = 0.1f; // Порог при котором еще происходит отскакивание
    fdGun.filter.groupIndex = -1;
    bodyGun->CreateFixture(&fdGun);

    //-----------------------------------------------------
    // Для снаряда
    //b2PolygonShape shapeBullet;
    b2CircleShape shapeBullet;
    //shapeBullet.SetAsBox(0.25f, 0.25f);
    shapeBullet.m_radius = 0.2f;

    b2FixtureDef fdBullet;
    fdBullet.shape = &shapeBullet;
    fdBullet.density = 1.0f;
    fdBullet.restitution = 0.1f;
    fdBullet.restitutionThreshold = 0.1f; // Порог при котором еще происходит отскакивание
    fdBullet.filter.groupIndex = -1;
    bodyBullet->CreateFixture(&fdBullet);


    //-----------------------------------------------------
    // Соединение шасси и башни
    b2RevoluteJoint *flJoint;

    //car class constructor
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = bodyPlayer;
    jointDef.enableLimit = false;
    //jointDef.lowerAngle = -PI;//with both these at zero...
    //jointDef.upperAngle = PI;//...the joint will not move
    jointDef.localAnchorB.SetZero();//joint anchor in tire is always center

    //TDTire* tire = new TDTire(world);
    jointDef.bodyB = bodyGun;
    //jointDef.localAnchorA.Set( -3, 8.5f );
    jointDef.localAnchorA.SetZero();
    //flJoint = (b2RevoluteJoint*)m_world->CreateJoint( &jointDef );

    jointDef.collideConnected = false;

    m_world->CreateJoint( &jointDef );

    //TODO Сделать вместо приложения момента к башне мотор для соединения,
    //это позволит сделать движение башни зависимым от поворота шасси
    //например чтобы быстрее повернуть башню можно еще поворачивать и корпус


    //-----------------------------------------------------

    // Можно создавать shape в виде окружности, полигона

    // Создание тела из нескольких фигур

    //--------------------------------------------------------------------------------------
    // Создание составных тел - препятствий из треугольников после триангуляции полигонов - препятствий
    // Взять набор треугольников принадлежащих препятствию

    // Перебор дырок
    for (int i = 0; i < (*pGameMap).trianglesHoles.size(); i++) {
        // -----------------------------------------
        b2Body* obstacle;

        b2BodyDef bd;
        bd.type = b2_staticBody;

        p2t::Triangle& t = *((*pGameMap).trianglesHoles[i][0]);
        p2t::Point& pos = *t.GetPoint(0);

        //bd.position.Set(5.0f, 1.0f);
        bd.position.Set(pos.x/kScale, pos.y/kScale);
        obstacle = m_world->CreateBody(&bd);
        // -----------------------------------------

        std::vector<p2t::Triangle*> tmpTrianglesHole;

        // Перебор треугольников внутри i-дырки
        for (int j = 0; j < (*pGameMap).trianglesHoles[i].size(); j++) {

            // ------------------------------------------------

            p2t::Triangle& t = *((*pGameMap).trianglesHoles[i][j]);
            p2t::Point& a = *t.GetPoint(0);
            p2t::Point& b = *t.GetPoint(1);
            p2t::Point& c = *t.GetPoint(2);

            // ------------------------------------------------
            // TODO

            b2Vec2 vertices[3];

            b2PolygonShape shapeTriangle;
            vertices[0].Set(a.x/kScale - pos.x/kScale, a.y/kScale - pos.y/kScale);
            vertices[1].Set(b.x/kScale - pos.x/kScale, b.y/kScale - pos.y/kScale);
            vertices[2].Set(c.x/kScale - pos.x/kScale, c.y/kScale - pos.y/kScale);
            shapeTriangle.Set(vertices, 3);

            b2FixtureDef fd;
            fd.shape = &shapeTriangle;
            fd.density = 1.0f;
            fd.restitution = 0.1f;
            fd.restitutionThreshold = 0.1f; // Порог при котором еще происходит отскакивание

            //obstacle->CreateFixture(&shapeTriangle, 2.0f);
            obstacle->CreateFixture(&fd);

            // ------------------------------------------------

//            std::vector<Vector2> vectorPointsIn;
//            vectorPointsIn.push_back((Vector2){(float)a.x,(float)a.y});
//            vectorPointsIn.push_back((Vector2){(float)b.x,(float)b.y});
//            vectorPointsIn.push_back((Vector2){(float)c.x,(float)c.y});

//            sortVerticies(vectorPointsIn);

//            DrawTriangle(vectorPointsIn[0], vectorPointsIn[1], vectorPointsIn[2], RED);
//            DrawTriangleLines(vectorPointsIn[0], vectorPointsIn[1], vectorPointsIn[2], BLACK);
        }
    }
    // Выбрать у препятствия первую точку
    // Относительно неё получить координаты других точек
    //--------------------------------------------------------------------------------------
    // s_test->Step(s_settings);
    //--------------------------------------------------------------------------------------



}

// Получение боковой скорости перпендикулярно корпусу (проекция вектора скорости на ось поперек корпуса)
b2Vec2 Physics::getLateralVelocity(b2Body* m_body) {
    b2Vec2 currentRightNormal = m_body->GetWorldVector( b2Vec2(0,1) );
    return b2Dot( currentRightNormal, m_body->GetLinearVelocity() ) * currentRightNormal;
}

// Получение скорости параллельно корпусу (проекция вектора скорости на ось вдоль корпуса)
b2Vec2 Physics::getForwardVelocity(b2Body* m_body) {
    b2Vec2 currentForwardNormal = m_body->GetWorldVector( b2Vec2(1,0) );
    return b2Dot( currentForwardNormal, m_body->GetLinearVelocity() ) * currentForwardNormal;
}

void Physics::updateFriction(b2Body* m_body) {
    //lateral linear velocity
//    float maxLateralImpulse = 2.5f;
//    b2Vec2 impulse = m_body->GetMass() * -getLateralVelocity(m_body);
//    if ( impulse.Length() > maxLateralImpulse )
//        impulse *= maxLateralImpulse / impulse.Length();

//    // Применить к телу импульс направленный против боковой скорости
//    m_body->ApplyLinearImpulse( impulse, m_body->GetWorldCenter(), true);

    //lateral linear velocity

    float Kz = 15;
    float Kb = 15;

    b2Vec2 currentLateralNormal = getLateralVelocity(m_body);
    float currentLateralSpeed = currentLateralNormal.Normalize();
    float dragLateralForceMagnitude = -Kz * currentLateralSpeed;
    m_body->ApplyForce( dragLateralForceMagnitude * currentLateralNormal, m_body->GetWorldCenter(), true);

    //angular velocity
    m_body->ApplyAngularImpulse( 0.16f * m_body->GetInertia() * -m_body->GetAngularVelocity(), true);

    //forward linear velocity
    b2Vec2 currentForwardNormal = getForwardVelocity(m_body);
    float currentForwardSpeed = currentForwardNormal.Normalize();
    float dragForceMagnitude = -Kb * currentForwardSpeed;
    m_body->ApplyForce( dragForceMagnitude * currentForwardNormal, m_body->GetWorldCenter(), true);

    // Когда мы отпускаем газ, то сразу нажимается тормоз и трение скольжение вдоль и поперек примерно равны
    // Пока мы держим газ нам пофиг на трение вдоль, потому что она компенсируется двигателем.

    // Для усиления занаса снижать коэф. Kz.
}

//projectile
//void CreateBullet() {
//}

//void LaunchBullet(b2Body* bodyBullet, RoboTank& playerRoboTank) {
void Physics::LaunchBullet() {
    bodyBullet->SetTransform(b2Vec2(pPlayerRoboTank->getPosition().x/kScale, pPlayerRoboTank->getPosition().y/kScale), 0.0f);

    // Снаряд должен вылетать под углом равным углу поворота башни
    float magnitudeBulletVelocity = 50;

    float angleGun = pPlayerRoboTank->getAngleGun();

    float xBulletVelocity = magnitudeBulletVelocity * cos(angleGun);
    float yBulletVelocity = magnitudeBulletVelocity * sin(angleGun);

    bodyBullet->SetLinearVelocity(b2Vec2(xBulletVelocity, yBulletVelocity));
    bodyBullet->SetAngularVelocity(0.0f);
}

//void Physics::AddRobot() {
