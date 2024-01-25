#include "GameplayScreen.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

//#define RAYGUI_IMPLEMENTATION
//#include "raygui.h"

#include "rlgl.h"           // Required for: Vertex definition
#include "raymath.h"

#include "poly2tri.h"

#include "box2d.h"

// --------------------------
#include "Init.h"
//#include "GlobalParams.h"
#include "GameState.h"
#include "Robot.h"
#include "Physics.h"
#include "Map.h"
// --------------------------

constexpr float ZoomDefault = 3.0f;

void draw_polyline(std::vector<Vector2> &tmpPolygon, int thickness, Color color);
void draw_polygon(std::vector<Vector2> &tmpPolygon, int thickness, Color color);
void DrawTextureTriangle(Texture2D texture, Vector2 *points, Vector2 *texcoords, Color tint);

GameplayScreen::GameplayScreen()
{

}

// Gameplay Screen Initialization logic
void GameplayScreen::Init(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;

    // ???
    camera.target = (Vector2) {0, 0};
    camera.offset = (Vector2) {pGameState->screenWidth/2.0f, pGameState->screenHeight/2.0f };
    camera.rotation = 0.0f;
    //camera.zoom = 1.0f;
    //camera.zoom = 2.0f;
    camera.zoom = ZoomDefault;
    // TODO Подумать какой zoom должен быть по умолчанию. Зависит от размера танка игрока

    // ???

    //--------------------------------------------------------------------------------------
    // Загрузка текстур
    textureRoboTank = LoadTexture("res/tank1.png");
    textureRoboTank2 = LoadTexture("res/tank2.png");
    textureGrass = LoadTexture("res/grass.png");
    textureStone = LoadTexture("res/stone.png");

    SetTextureFilter(textureGrass, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(textureStone, TEXTURE_FILTER_BILINEAR);

    int frameWidth = textureRoboTank.width/2;
    int frameHeight = textureRoboTank.height - 32;

    // Source rectangle (part of the texture to use for drawing)
    sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
    sourceRec2 = { 0.0f, 0.0f, (float)128, (float)64 };
    rectSourceTextureGrass = {0.0f, 0.0f, (float)1024, (float)1024};

    // Destination rectangle (screen rectangle where drawing part of texture)
    destRec = { pGameState->screenWidth/2.0f, pGameState->screenHeight/2.0f, frameWidth*2.0f, frameHeight*2.0f };

    // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
    origin = { (float)frameWidth, (float)frameHeight };

    // Запуск физического мира
    pGamePhysics->initPhysics();

}

// Gameplay Screen Update logic
void GameplayScreen::Update(void)
{
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update variables / Implement example logic at this point
    //----------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------
    // Box2d
    //----------------------------------------------------------------------------------

    pGamePhysics->m_world->Step(1.0f/60.0f, 6, 2);


    if (isTimerStart) timer++;

    //-------------------------------------------------
    // Обновление положения танка игрока
    Vector2 newPosition;

    newPosition.x = pGamePhysics->bodyPlayer->GetPosition().x * pGamePhysics->kScale;
    newPosition.y = pGamePhysics->bodyPlayer->GetPosition().y * pGamePhysics->kScale;


    (*pPlayerRoboTank).setPosition(newPosition);

    player_angle = pGamePhysics->bodyPlayer->GetAngle(); //Radians
    (*pPlayerRoboTank).setAngleChassis(player_angle);

    float angleGun = pGamePhysics->bodyGun->GetAngle(); //Radians
    (*pPlayerRoboTank).setAngleGun(angleGun);


    //std::cout << "Position = " << newPosition.x << " " << newPosition.y << " " << player_angle <<  std::endl;

    //-------------------------------------------------

    float F_x = 100*cos(player_angle);
    float F_y = 100*sin(player_angle);

    //----------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------

    // Пример управления с помощью мышки и клавиатуры
    {
        //Режим навигации по экрану
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            camera.target.x -= GetMouseDelta().x;
            camera.target.y -= GetMouseDelta().y;
        }

        // Центрирование вида по координатам соответствующим координатам точки на которую указывает курсор
        // Неправильно работает при camera.zoom != 1.0f;
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            camera.target.x += GetMousePosition().x - pGameState->screenWidth/2.0f;
            camera.target.y += GetMousePosition().y - pGameState->screenWidth/2.0f;
        }

        ballPosition = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ballColor = MAROON;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) ballColor = DARKBLUE;

        // TODO
        // Переделать под управление роботом
        // Player movement

//            if (IsKeyDown(KEY_RIGHT)) player.x += 1;
//            else if (IsKeyDown(KEY_LEFT)) player.x -= 1;

//            if (IsKeyDown(KEY_UP)) player.y -= 1;
//            else if (IsKeyDown(KEY_DOWN)) player.y += 1;



//            if (IsKeyDown(KEY_RIGHT)) bodyPlayer->ApplyForceToCenter(b2Vec2(40 - F_sopr,0), 1);
//            else if (IsKeyDown(KEY_LEFT)) bodyPlayer->ApplyForceToCenter(b2Vec2(-40 + F_sopr,0), 1);

//            if (IsKeyDown(KEY_UP)) bodyPlayer->ApplyForceToCenter(b2Vec2(0, -40 + F_sopr), 1);
//            else if (IsKeyDown(KEY_DOWN)) bodyPlayer->ApplyForceToCenter(b2Vec2(0, 40 - F_sopr), 1);

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) pGamePhysics->bodyPlayer->ApplyTorque(40.0f, 1);
        else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) pGamePhysics->bodyPlayer->ApplyTorque(-40.0f, 1);
        //else bodyPlayer->ApplyTorque(0.0f, 1);saaaw
        //else bodyPlayer->ApplyAngularImpulse(0.0f, 1);
        //else bodyPlayer->SetAngularVelocity(0.0f);





//            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) bodyPlayer->ApplyForceToCenter(b2Vec2(0, -40 + F_sopr), 1);
//            else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) bodyPlayer->ApplyForceToCenter(b2Vec2(0, 40 - F_sopr), 1);

        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) pGamePhysics->bodyPlayer->ApplyForceToCenter(b2Vec2(F_x, F_y), 1);
        else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) pGamePhysics->bodyPlayer->ApplyForceToCenter(b2Vec2(-F_x, -F_y), 1);
        //else bodyPlayer->ApplyForceToCenter(b2Vec2(0.0f, 0.0f), 1);
        //else bodyPlayer->ApplyLinearImpulseToCenter(b2Vec2(0.0f, 0.0f), 1);
        //else bodyPlayer->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

//            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) bodyPlayer->ApplyForceToCenter(b2Vec2(F_x - F_sopr_x, F_y - F_sopr_y), 1);
//            else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) bodyPlayer->ApplyForceToCenter(b2Vec2(-F_x + F_sopr_x, -F_y + F_sopr_y), 1);
//            else bodyPlayer->ApplyForceToCenter(b2Vec2(-F_sopr_x, -F_sopr_y), 1);

        //----------------------------------------------------------------------------------
        // Управление башней

        if (IsKeyDown(KEY_Z) || IsKeyDown(KEY_Z)) pGamePhysics->bodyGun->ApplyTorque(10.0f, 1);
        else if (IsKeyDown(KEY_X) || IsKeyDown(KEY_X)) pGamePhysics->bodyGun->ApplyTorque(-10.0f, 1);

        //----------------------------------------------------------------------------------
        // Выстрел

        if (IsKeyDown(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (!isTimerStart) {
                isTimerStart = 1;
                pGamePhysics->LaunchBullet();
            }
        }

        if (isTimerStart) {
            if (timer > 50) {
                timer = 0;

                pGamePhysics->bodyBullet->SetTransform(b2Vec2((*pPlayerRoboTank).getPosition().x/pGamePhysics->kScale, (*pPlayerRoboTank).getPosition().y/pGamePhysics->kScale), 0.0f);
                pGamePhysics->bodyBullet->SetLinearVelocity(b2Vec2(0, 0));

                isTimerStart = 0;
            }

        }

        // TODO Сделать нормальный таймер на тиках игры а не на кадрах. Посмотреть как это делается для webasm.

        // TODO Сделать отдачу, для этого надо как то взаимодействовать с корпусом танка
        //----------------------------------------------------------------------------------




        //----------------------------------------------------------------------------------
        // Определение положение прицела
        Vector2 posAim = GetMousePosition();

        //std::cout << "posAim " << posAim.x << " " << posAim.y << std::endl;

        Vector2 vecAim = {(posAim.x - pGameState->screenWidth/2.0f), (posAim.y - pGameState->screenHeight/2.0f)};

        //std::cout << "vecAim " << vecAim.x << " " << vecAim.y << std::endl;

        // Определение целевого угла поворота башни
        float magVecAim = sqrt(pow(vecAim.x, 2) + pow(vecAim.y, 2));
        float cosAngleAim = vecAim.x / magVecAim;
        float angleAim = acos(cosAngleAim);

        //float sinAngleAim = vecAim.y / magVecAim;
        if (vecAim.y < 0) angleAim *= -1;

        // TODO проверять если acos возвращает nan при положении прицела в центре экрана.

        //std::cout << "AngleGun " << (*pPlayerRoboTank).getAngleGun() << std::endl;
        //std::cout << "angleAim " << angleAim << std::endl;

        float numberOborots = 0;
        modf((abs((*pPlayerRoboTank).getAngleGun()) / (2*PI)), &numberOborots);

        float AngleGunNormal = abs((*pPlayerRoboTank).getAngleGun()) - numberOborots*2*PI;

        if ((*pPlayerRoboTank).getAngleGun() < 0) AngleGunNormal = 2*PI - AngleGunNormal;


        float AngleAimNormal;
        if (angleAim < 0) AngleAimNormal = 2*PI + angleAim;
        else AngleAimNormal = angleAim;

        //std::cout << "AngleGunNormal " << AngleGunNormal << std::endl;
        //std::cout << "AngleAimNormal " << AngleAimNormal << std::endl;

        //std::cout << "numberOborots " << numberOborots << std::endl;

        float diffAngle = 0;

        diffAngle = AngleAimNormal - AngleGunNormal;
        if (abs(diffAngle) > PI) {

            if (AngleAimNormal > AngleGunNormal) pGamePhysics->bodyGun->ApplyTorque(-10.0f, 1);
            else pGamePhysics->bodyGun->ApplyTorque(10.0f, 1);

        }
        else {
            float epsilon = 0.001;

            if (diffAngle >= epsilon) pGamePhysics->bodyGun->ApplyTorque(10.0f, 1);
            else pGamePhysics->bodyGun->ApplyTorque(-10.0f, 1);

            // TODO Точность прицеливание зависит от epsilon, а от разрешения экрана зависит?
        }

//            if (diffAngle < 0) {
//                if (diffAngle >= 0.1) bodyGun->ApplyTorque(10.0f, 1);
//                else bodyGun->ApplyTorque(-10.0f, 1);
//            }


//            if (angleAim - (*pPlayerRoboTank).getAngleGun() >= 0.1) bodyGun->ApplyTorque(10.0f, 1);
//            else bodyGun->ApplyTorque(-10.0f, 1);



        //TODO Сделать отключаемым режим прицела с помощью мышки



        //----------------------------------------------------------------------------------



        //----------------------------------------------------------------------------------
        // Для башни
        //angular velocity
        pGamePhysics->bodyGun->ApplyAngularImpulse( 0.1f * pGamePhysics->bodyGun->GetInertia() * -pGamePhysics->bodyGun->GetAngularVelocity(), true);

        //----------------------------------------------------------------------------------
        // Должна быть сила сопротивления двигателю (движению по вперед или назад) направленная вдоль корпуса танка
        // Она должна нарастать тем больше чем выше скорость (проекция скорости на ось вдоль корпуса танка)

        // Если танк пихнули в бок, то должна возникнуть сила трения направленная поперек корпуса танка, при этом она не должна зависеть от скорость,
        // а быть постоянной и достаточной чтобы остановить танк

        // Физику движения можно взять из AI CUPS
//            float F_sopr = 0.0f;
//            float V_tank = sqrt(pow(bodyPlayer->GetLinearVelocity().x, 2) + pow(bodyPlayer->GetLinearVelocity().y, 2));
//            float K_sopr_0 = 0.5f;
//            float K_sopr = K_sopr_0 * (1 + (V_tank * V_tank)/10);
//            float M_tank = bodyPlayer->GetMass();
//            float g = 9.8f;
//            F_sopr = K_sopr * M_tank * g;

//            std::cout << "F_sopr = " << F_sopr << std::endl;

//            float F_sopr_x = F_sopr * cos(player_angle);
//            float F_sopr_y = F_sopr * sin(player_angle);

        // Со временем танк под действием сил сопротивления должен остановиться

        pGamePhysics->updateFriction(pGamePhysics->bodyPlayer);

        // Направление движение при танковом управлении определяется направлением переда танка

        // Поворот танка

        // TODO Режим без сопротивления превращает танк в космический корабль, можно ставить такой режим по кнопке или в настройках.
        //----------------------------------------------------------------------------------


        // Camera zoom controls
        camera.zoom += ((float)GetMouseWheelMove() * 0.1f);

        if (camera.zoom > 10.0f) camera.zoom = 10.0f;
        else if (camera.zoom < 0.01f) camera.zoom = 0.01f;

        // Camera reset (zoom and rotation)
        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = ZoomDefault;
            camera.rotation = 0.0f;
        }

    }

    //----------------------------------------------------------------------------------
    // Обновление вида камеры
    //camera.target = (Vector2) {0, 0};

    // Привязка вида камеры к танку игрока
    camera.target = (*pPlayerRoboTank).getPosition();
    // TODO Сделать опциональным

    // TODO сделать режим когда угол камеры изменяется вместе с поворотом танчика
    //camera.rotation = -(*pPlayerRoboTank).getAngleChassis()*(180/PI) - 90.0f;
    //camera.rotation = (*pPlayerRoboTank).getAngleChassis(); // пьяный мастер

    //----
    //А если мышкой экран сдвигают?
    //----

    // TODO
    // Сделать различные режимы видов на горячие клавиши или в панели настроек или из конф. файла.



    //----------------------------------------------------------------------------------
    // TODO Сделать нормальный выход в меню
    if (IsKeyPressed(KEY_G))
    {
        finishScreen = 1;
        //PlaySound(fxCoin);
    }
}

int sortVerticies(std::vector<Vector2> &vectorPointsIn) {
    std::reverse(vectorPointsIn.begin(), vectorPointsIn.end());
    return 0;
}

// Gameplay Screen Draw logic
void GameplayScreen::Draw(void)
{
    // Draw
    //----------------------------------------------------------------------------------

    // TODO: Draw everything that requires to be drawn at this point:

    //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);  // Example


    // Пример управления с помощью мышки и клавиатуры
    {
        BeginMode2D(camera);
        {
            // TODO Понять как работает этот фильтр, как сглаживание получается
            //DrawTexturePro(textureGrass, rectSourceTextureGrass, {0.0f, 0.0f, 1024.0f, 1024.0f}, {0, 0}, (float)0, WHITE);
            DrawTexturePro(textureGrass, rectSourceTextureGrass, {0.0f, 0.0f, 512.0f, 512.0f}, {0, 0}, (float)0, WHITE);
            DrawTexturePro(textureGrass, rectSourceTextureGrass, {512.0f, 0.0f, 512.0f, 512.0f}, {0, 0}, (float)0, WHITE);
            DrawTexturePro(textureGrass, rectSourceTextureGrass, {0.0f, 512.0f, 512.0f, 512.0f}, {0, 0}, (float)0, WHITE);
            DrawTexturePro(textureGrass, rectSourceTextureGrass, {512.0f, 512.0f, 512.0f, 512.0f}, {0, 0}, (float)0, WHITE);

            DrawLine(0, -pGameState->screenHeight*10, 0, pGameState->screenHeight*10, GREEN);
            DrawLine(-pGameState->screenWidth*10, 0, pGameState->screenWidth*10, 0, GREEN);

            //                    DrawLine((int)camera.target.x, -screenHeight*10, (int)camera.target.x, screenHeight*10, GREEN);
            //                    DrawLine(-screenWidth*10, (int)camera.target.y, screenWidth*10, (int)camera.target.y, GREEN);


            // Отрисовка линий треугольников после триангуляции
            for (int i = 0; i < pGameMap->triangles.size(); i++) {
                p2t::Triangle& t = *(pGameMap->triangles[i]);
                p2t::Point& a = *t.GetPoint(0);
                p2t::Point& b = *t.GetPoint(1);
                p2t::Point& c = *t.GetPoint(2);

                // Red
                //glColor3f(1, 0, 0);

                //glBegin(GL_LINE_LOOP);
                //      glVertex2d(a.x, a.y);
                //      glVertex2d(b.x, b.y);
                //      glVertex2d(c.x, c.y);

                Vector2 tmpPointA;

                tmpPointA.x = a.x;
                tmpPointA.y = a.y;

                Vector2 tmpPointB;

                tmpPointB.x = b.x;
                tmpPointB.y = b.y;

                Vector2 tmpPointC;

                tmpPointC.x = c.x;
                tmpPointC.y = c.y;

                // TODO Сделать опциональной отрисовку линий графа после триангуляции
                //DrawLineEx(tmpPointA, tmpPointB, 1, BLUE);
                //DrawLineEx(tmpPointB, tmpPointC, 1, BLUE);
                //DrawLineEx(tmpPointC, tmpPointA, 1, BLUE);

                //glEnd();
            }

//            for(int i = 0; i < pGameMap->Polygons.size(); i++) {
//                draw_polygon(pGameMap->Polygons[i], 1, RED);
//            }

            // Отрисовка треугольников полигонов
            for (int i = 0; i < pGameMap->trianglesHoles.size(); i++) {

                //std::vector<p2t::Point*> hole;

                std::vector<p2t::Triangle*> tmpTrianglesHole;

                for (int j = 0; j < pGameMap->trianglesHoles[i].size(); j++) {

                    p2t::Triangle& t = *(pGameMap->trianglesHoles[i][j]);
                    p2t::Point& a = *t.GetPoint(0);
                    p2t::Point& b = *t.GetPoint(1);
                    p2t::Point& c = *t.GetPoint(2);

                    //DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);

                    std::vector<Vector2> vectorPointsIn;
                    vectorPointsIn.push_back((Vector2){(float)a.x,(float)a.y});
                    vectorPointsIn.push_back((Vector2){(float)b.x,(float)b.y});
                    vectorPointsIn.push_back((Vector2){(float)c.x,(float)c.y});

                    sortVerticies(vectorPointsIn);

                    //DrawTriangle(vectorPointsIn[0], vectorPointsIn[1], vectorPointsIn[2], RED);
                    //DrawTriangleLines(vectorPointsIn[0], vectorPointsIn[1], vectorPointsIn[2], BLACK);

                    Vector2 texcoords[4] = {
                        (Vector2){ 1.0f, 0.0f },
                        (Vector2){ 0.0f, 0.0f },
                        (Vector2){ 1.0f, 1.0f },
                        (Vector2){ 1.0f, 0.0f },
                    };

                    Vector2 points[4] = {vectorPointsIn[0], vectorPointsIn[1], vectorPointsIn[2], vectorPointsIn[0]};

                    DrawTextureTriangle(textureStone, points, texcoords, WHITE);

                    //// DrawTriangle((Vector2){(float)a.x,(float)a.y}, (Vector2){(float)b.x,(float)b.y}, (Vector2){(float)c.x,(float)c.y}, RED);
                }
            }

            //DrawRectangle(-200, 200, 200, 200, DARKGRAY);

            // Отрисовка снаряда
            DrawCircleV({pGamePhysics->bodyBullet->GetPosition().x*pGamePhysics->kScale, pGamePhysics->bodyBullet->GetPosition().y*pGamePhysics->kScale}, 2.0f, BLACK);

            // Отрисовка юнитов-роботов
            //DrawRectangleRec(player, RED);
            DrawCircleV((*pPlayerRoboTank).getPosition(), 8, RED);

            // Поворот прямоугольника
            // https://www.reddit.com/r/raylib/comments/mgjgbl/how_do_you_rotate_a_shape_around_its_origin_in/
            {
                rlPushMatrix();

                rlTranslatef((*pPlayerRoboTank).getPosition().x, (*pPlayerRoboTank).getPosition().y, 0);

                //rlRotatef(-player_angle * 57.29578f, 0, 0, -1);
                //rlRotatef(-(*pPlayerRoboTank).getAngleChassis() * PI, 0, 0, -1);
                rlRotatef(-(*pPlayerRoboTank).getAngleChassis() * (180/PI), 0, 0, -1); // TODO Оптимизировать!!! Перевод из радиан в градусы
                // TODO RADTODEG AND DEGTORAD
                angle += 0.25f;

                //DrawRectangleRec((Rectangle){0, 0, 16, 16}, BLUE);

                float widthChassis = (*pPlayerRoboTank).getWidthChassis();
                float lengthChassis = (*pPlayerRoboTank).getLengthChassis();

                //DrawRectangleLines(-widthChassis/2.0f, -lengthChassis/2.0f, widthChassis, lengthChassis, BLUE);
                DrawRectangleLines(-lengthChassis/2.0f, -widthChassis/2.0f, lengthChassis, widthChassis, BLUE);

                DrawTexturePro(textureRoboTank, sourceRec, {-widthChassis/2.0f, -lengthChassis/2.0f, widthChassis, lengthChassis}, {0 - 2, lengthChassis - 2}, (float)90, WHITE);

                rlPopMatrix();

            }

            // Отрисовка пушки
            float angleGun;

            angleGun = (*pPlayerRoboTank).getAngleGun();

            {
                rlPushMatrix();

                rlTranslatef((*pPlayerRoboTank).getPosition().x, (*pPlayerRoboTank).getPosition().y, 0);

                //rlRotatef(-angleAim * (180/PI), 0, 0, -1); // TODO Оптимизировать!!! Перевод из радиан в градусы
                rlRotatef(-angleGun * (180/PI), 0, 0, -1); // TODO Оптимизировать!!! Перевод из радиан в градусы

                //DrawRectangleRec((Rectangle){0, 0, 16, 16}, BLUE);

                float widthGun = 10.0f;
                float lengthGun = (*pPlayerRoboTank).getLengthChassis();

                ////DrawRectangleLines(-widthChassis/2.0f, -lengthChassis/2.0f, widthChassis, lengthChassis, BLUE);
                //DrawRectangleLines(-lengthChassis/4.0f, -widthChassis/4.0f, lengthChassis/2.0f, widthChassis/2.0f, RED);

                DrawTexturePro(textureRoboTank2, sourceRec2, {-lengthGun/2.0f, -widthGun/2.0f, lengthGun, widthGun}, {-4, 0}, (float)0, WHITE);

                //DrawTexturePro(textureRoboTank, sourceRec, {-widthChassis/2.0f, -lengthChassis/2.0f, widthChassis, lengthChassis}, {0 - 2, lengthChassis - 2}, (float)90, WHITE);

                // TODO Сделать пасхалку чтобы текстура пушки заменялась на текстуру пениса ао нвжвтию спец кнопки.

                rlPopMatrix();

            }


            // Поворот пушки

            //----------------------------------------------------------------------------------

            // Отрисовка прямоугольника и отметки переда
            //                    {
            //                        DrawRectanglePro((Rectangle){(*pPlayerRoboTank).getPosition().x, (*pPlayerRoboTank).getPosition().y, 16, 16}, (Vector2){8, 8}, player_angle * 57.29578f, RED);
            //                        angle += 0.25f;
            //                    }
            //                    {
            //                        DrawRectanglePro((Rectangle){(*pPlayerRoboTank).getPosition().x, (*pPlayerRoboTank).getPosition().y, 8 + 4, 16}, (Vector2){4 + 4, 8}, player_angle * 57.29578f, BLUE);
            //                        angle += 0.25f;
            //                    }

            //                    {
            //                        DrawRectanglePro((Rectangle){player.x + 12.0f, player.y, 4, 4}, (Vector2){-10.0f, 2}, player_angle * 57.29578f, BLUE);
            //                        angle += 0.25f;
            //                    }

            // Отрисовка радиуса обзора
            {
                //                        DrawCircleLines
            }


        }
        EndMode2D();

        DrawCircleV(ballPosition, 8, ballColor); // Почему размер круга при уменьшении зума не меняется?

        //DrawText("Hello!", 10, 10, 20, DARKGRAY);

        //camera.zoom = GuiSliderBar((Rectangle){ 600, 40, 120, 20 }, "Zoom", NULL, &(camera.zoom), 0.01f, 10.0f);
    }

    //DrawTexturePro(textureRoboTank, sourceRec, destRec, origin, (float)0, WHITE);
}

// Gameplay Screen Unload logic
void GameplayScreen::Unload(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int GameplayScreen::Finish(void)
{
    return finishScreen;
}

void draw_polyline(std::vector<Vector2> &tmpPolygon, int thickness, Color color) {
    // ! -1 чтобы не выйти за границу
    for (int i = 0; i < tmpPolygon.size() - 1; i++) {
        DrawLineEx(tmpPolygon[i], tmpPolygon[i + 1], thickness, color);
    }
}

void draw_polygon(std::vector<Vector2> &tmpPolygon, int thickness, Color color) {
    if (tmpPolygon.size() == 1) {
        DrawCircleV(tmpPolygon[0], 8, color);
    }
    draw_polyline(tmpPolygon, thickness, color);
    if (tmpPolygon.size() > 2) {
        DrawLineEx(tmpPolygon[0], tmpPolygon[tmpPolygon.size() - 1], thickness, color);

//        const int numPointsMax = 10;
//        Vector2 points[numPointsMax];
//        Vector2 texcoords[numPointsMax];

//        int numPoints = tmpPolygon.size() + 1;
//        if (numPoints > numPointsMax) numPoints = numPointsMax;

        // Переписать так чтобы было против часовой стрелки
//        for (int i = 0; i < numPoints; i++) {
//            points[numPoints - i] = tmpPolygon[i];
//            texcoords[numPoints - i] = points[i];
////            points[i] = tmpPolygon[i];
////            texcoords[i] = points[i];
//            texcoords[numPoints - i].x = (points[numPoints - i].x)/256.0f + 0.5f;
//            texcoords[numPoints - i].y = (points[numPoints - i].y)/256.0f + 0.5f;
//        }
//        points[0] = points[numPoints];
//        texcoords[0] = texcoords[numPoints];

//        float minX = 0;
//        float minY = 0;
//        float maxX = 0;
//        float maxY = 0;

//        for (int i = 0; i < numPoints; i++) {
//            if (points[i].x < minX) minX = points[i].x;
//            if (points[i].x > maxX) maxX = points[i].x;
//            if (points[i].y < minY) minY = points[i].y;
//            if (points[i].y > maxY) maxY = points[i].y;
//        }

//        Vector2 pointCenter = {(minX + maxX)/2, (minY + maxY)/2};

//        DrawTexturePoly(texture, pointCenter, points, texcoords, numPoints, WHITE);

    }


}

void DrawTextureTriangle(Texture2D texture, Vector2 *points, Vector2 *texcoords, Color tint)
{
    rlSetTexture(texture.id);

    // Texturing is only supported on RL_QUADS
    rlBegin(RL_QUADS);

        rlColor4ub(tint.r, tint.g, tint.b, tint.a);

        for (int i = 0; i < 4; i++)
        {
            rlTexCoord2f(texcoords[i].x, texcoords[i].y);
            rlVertex2f(points[i].x, points[i].y);
        }
    rlEnd();

    rlSetTexture(0);


}
