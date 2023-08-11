#include <iostream>
#include <fstream>
#include <cmath>

#include "raylib.h"
#include "json.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "rlgl.h"           // Required for: Vertex definition
#include "raymath.h"

#include "poly2tri.h"

#include "box2d.h"

// --------------------------
#include "Init.h"
#include "GlobalParams.h"
#include "Robot.h"
// --------------------------

using json = nlohmann::json;

Texture texture;

void draw_polyline(std::vector<Vector2> &tmpPolygon, int thickness, Color color);

void draw_polygon(std::vector<Vector2> &tmpPolygon, int thickness, Color color);

int sortVerticies(std::vector<Vector2> &vectorPointsIn) {
    std::reverse(vectorPointsIn.begin(), vectorPointsIn.end());
    return 0;
}

void readMap(std::vector<std::vector<Vector2>> &Polygons);

int triangulateMap(std::vector<p2t::Triangle*> &triangles,
                   std::vector<std::vector<Vector2>> &Polygons,
                   std::vector<std::vector<p2t::Triangle*>> &trianglesHoles,
                   std::vector<p2t::Point*> &polyline,
                   std::vector<p2t::Point*> &steiner);

void initPhysics(b2World* m_world,
                 Vector2 player,
                 b2Body* bodyPlayer,
                 std::vector<std::vector<p2t::Triangle*>> &trianglesHoles,
                 std::vector<p2t::Point*> &polyline);


float kScale = 10;

int main()
{
    //--------------------------------------------------------------------------------------
    Init();
    RoboTank playerRoboTank(TypeChassis::Medium, TypeGun::Medium);

    //--------------------------------------------------------------------------------------
    std::vector<std::vector<Vector2>> Polygons;
    readMap(Polygons);
    //--------------------------------------------------------------------------------------
    // Треугольники для отрисовки полигонов-дырок
    std::vector<std::vector<p2t::Triangle*>> trianglesHoles;
    std::vector<p2t::Point*> polyline;
    //std::vector<std::vector<p2t::Point*>> holes;
    std::vector<p2t::Point*> steiner;
    std::vector<p2t::Triangle*> triangles;

    triangulateMap(triangles, Polygons, trianglesHoles, polyline, steiner);
    //--------------------------------------------------------------------------------------
    b2World* m_world;
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    m_world = new b2World(gravity);
    Vector2 player = {(float)(*steiner[0]).x, (float)(*steiner[0]).y};
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    //bd.position.Set(-40.0f,5.0f);
    bd.position.Set(player.x/kScale, player.y/kScale);
    b2Body* bodyPlayer = m_world->CreateBody(&bd);

    initPhysics(m_world, player, bodyPlayer, trianglesHoles, polyline);
    //--------------------------------------------------------------------------------------


    //--------------------------------------------------------------------------------------
    // Initialization
    //--------------------------------------------------------------------------------------




    //const int screenWidth = 800;
    //const int screenHeight = 450;
    const int screenWidth = 1024;
    const int screenHeight = 768;

    // RoboTank в виде квадратика

    //Rectangle player = {(float)(*steiner[0]).x - 8, (float)(*steiner[0]).y - 8, 16, 16};

    float player_angle = 0.0f;

    InitWindow(screenWidth, screenHeight, "project rts");

    // ???
    Camera2D camera = { 0 };
    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // ???
    Vector2 ballPosition = { -100.0f, -100.0f };
    Color ballColor = DARKBLUE;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    float angle = 0.0f;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update variables / Implement example logic at this point
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------
        // Box2d
        //----------------------------------------------------------------------------------
        m_world->Step(1.0f/60.0f, 6, 2);

        player.x = bodyPlayer->GetPosition().x * kScale;
        player.y = bodyPlayer->GetPosition().y * kScale;

        player_angle = bodyPlayer->GetAngle(); //Radians

        std::cout << "Position = " << player.x << " " << player.y << " " << player_angle <<  std::endl;

        float F_x = 40*cos(player_angle);
        float F_y = 40*sin(player_angle);

        //----------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------

        // Пример управления с помощью мышки и клавиатуры
        {
            //Режим навигации по экрану
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                camera.target.x -= GetMouseDelta().x;
                camera.target.y -= GetMouseDelta().y;
            }

            // Центрирования вида по координатам соответствующим координатам точки на которую указывает курсор
            // Неправильно работает при camera.zoom != 1.0f;
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                camera.target.x += GetMousePosition().x - screenWidth/2.0f;
                camera.target.y += GetMousePosition().y - screenHeight/2.0f;
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

            // Должна быть сила сопротивления двигателю
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

            // Направление движение при танковом управлении определяется направлением переда танка

            // Поворот танка


//            if (IsKeyDown(KEY_RIGHT)) bodyPlayer->ApplyForceToCenter(b2Vec2(40 - F_sopr,0), 1);
//            else if (IsKeyDown(KEY_LEFT)) bodyPlayer->ApplyForceToCenter(b2Vec2(-40 + F_sopr,0), 1);

//            if (IsKeyDown(KEY_UP)) bodyPlayer->ApplyForceToCenter(b2Vec2(0, -40 + F_sopr), 1);
//            else if (IsKeyDown(KEY_DOWN)) bodyPlayer->ApplyForceToCenter(b2Vec2(0, 40 - F_sopr), 1);

            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) bodyPlayer->ApplyTorque(5.0f, 1);
            else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) bodyPlayer->ApplyTorque(-5.0f, 1);
            //else bodyPlayer->ApplyTorque(0.0f, 1);
            //else bodyPlayer->ApplyAngularImpulse(0.0f, 1);
            else bodyPlayer->SetAngularVelocity(0.0f);





//            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) bodyPlayer->ApplyForceToCenter(b2Vec2(0, -40 + F_sopr), 1);
//            else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) bodyPlayer->ApplyForceToCenter(b2Vec2(0, 40 - F_sopr), 1);

            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) bodyPlayer->ApplyForceToCenter(b2Vec2(F_x, F_y), 1);
            else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) bodyPlayer->ApplyForceToCenter(b2Vec2(-F_x, -F_y), 1);
            //else bodyPlayer->ApplyForceToCenter(b2Vec2(0.0f, 0.0f), 1);
            //else bodyPlayer->ApplyLinearImpulseToCenter(b2Vec2(0.0f, 0.0f), 1);
            else bodyPlayer->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

//            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) bodyPlayer->ApplyForceToCenter(b2Vec2(F_x - F_sopr_x, F_y - F_sopr_y), 1);
//            else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) bodyPlayer->ApplyForceToCenter(b2Vec2(-F_x + F_sopr_x, -F_y + F_sopr_y), 1);
//            else bodyPlayer->ApplyForceToCenter(b2Vec2(-F_sopr_x, -F_sopr_y), 1);



            // Camera zoom controls
            camera.zoom += ((float)GetMouseWheelMove() * 0.1f);

            if (camera.zoom > 10.0f) camera.zoom = 10.0f;
            else if (camera.zoom < 0.01f) camera.zoom = 0.01f;

            // Camera reset (zoom and rotation)
            if (IsKeyPressed(KEY_R))
            {
                camera.zoom = 1.0f;
                camera.rotation = 0.0f;
            }

        }


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            // TODO: Draw everything that requires to be drawn at this point:

            //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);  // Example


            // Пример управления с помощью мышки и клавиатуры
            {
                DrawCircleV(ballPosition, 8, ballColor); // Почему размер круга при уменьшении зума не меняется?

                BeginMode2D(camera);
                {

                    DrawLine(0, -screenHeight*10, 0, screenHeight*10, GREEN);
                    DrawLine(-screenWidth*10, 0, screenWidth*10, 0, GREEN);

//                    DrawLine((int)camera.target.x, -screenHeight*10, (int)camera.target.x, screenHeight*10, GREEN);
//                    DrawLine(-screenWidth*10, (int)camera.target.y, screenWidth*10, (int)camera.target.y, GREEN);


                    // Отрисовка линий треугольников после триангуляции
                    for (int i = 0; i < triangles.size(); i++) {
                      p2t::Triangle& t = *triangles[i];
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

                      DrawLineEx(tmpPointA, tmpPointB, 1, BLUE);
                      DrawLineEx(tmpPointB, tmpPointC, 1, BLUE);
                      DrawLineEx(tmpPointC, tmpPointA, 1, BLUE);

                      //glEnd();
                    }

                    for(int i = 0; i < Polygons.size(); i++) {
                        draw_polygon(Polygons[i], 1, RED);
                    }

                    // Отрисовка триугольников полигонов
                    for (int i = 0; i < trianglesHoles.size(); i++) {

                        //std::vector<p2t::Point*> hole;

                        std::vector<p2t::Triangle*> tmpTrianglesHole;

                        for (int j = 0; j < trianglesHoles[i].size(); j++) {

                            p2t::Triangle& t = *trianglesHoles[i][j];
                            p2t::Point& a = *t.GetPoint(0);
                            p2t::Point& b = *t.GetPoint(1);
                            p2t::Point& c = *t.GetPoint(2);

                            //DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);

                            std::vector<Vector2> vectorPointsIn;
                            vectorPointsIn.push_back((Vector2){(float)a.x,(float)a.y});
                            vectorPointsIn.push_back((Vector2){(float)b.x,(float)b.y});
                            vectorPointsIn.push_back((Vector2){(float)c.x,(float)c.y});

                            sortVerticies(vectorPointsIn);

                            DrawTriangle(vectorPointsIn[0], vectorPointsIn[1], vectorPointsIn[2], RED);
                            DrawTriangleLines(vectorPointsIn[0], vectorPointsIn[1], vectorPointsIn[2], BLACK);

                            // DrawTriangle((Vector2){(float)a.x,(float)a.y}, (Vector2){(float)b.x,(float)b.y}, (Vector2){(float)c.x,(float)c.y}, RED);
                        }
                    }

                    //DrawRectangle(-200, 200, 200, 200, DARKGRAY);

                    // Отрисовка юнитов-роботов
                    //DrawRectangleRec(player, RED);
                    DrawCircleV(player, 8, RED);

                    // Поворот прямоугольника
                    // https://www.reddit.com/r/raylib/comments/mgjgbl/how_do_you_rotate_a_shape_around_its_origin_in/
                    {
                        rlPushMatrix();

                        rlTranslatef(player.x, player.y, 0);

                        rlRotatef(-player_angle * 57.29578f, 0, 0, -1);
                        angle += 0.25f;

                        //DrawRectangleRec((Rectangle){0, 0, 16, 16}, BLUE);

                        DrawRectangleLines(-8, -8, 16, 16, BLUE);

                        rlPopMatrix();
                    }
                    {
                        DrawRectanglePro((Rectangle){player.x, player.y, 16, 16}, (Vector2){8, 8}, player_angle * 57.29578f, RED);
                        angle += 0.25f;
                    }
                    {
                        DrawRectanglePro((Rectangle){player.x, player.y, 8 + 4, 16}, (Vector2){4 + 4, 8}, player_angle * 57.29578f, BLUE);
                        angle += 0.25f;
                    }
                    //                    {
                    //                        DrawRectanglePro((Rectangle){player.x + 12.0f, player.y, 4, 4}, (Vector2){-10.0f, 2}, player_angle * 57.29578f, BLUE);
                    //                        angle += 0.25f;
                    //                    }

                }
                EndMode2D();

                DrawText("Hello!", 10, 10, 20, DARKGRAY);

                camera.zoom = GuiSliderBar((Rectangle){ 600, 40, 120, 20 }, "Zoom", NULL, camera.zoom, 0.01f, 10.0f);
            }

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // TODO: Unload all loaded resources at this point

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void readMap(std::vector<std::vector<Vector2>> &Polygons) {

    // Чтение из файла

    std::ifstream input("map.json");
    //json data = json::parse(f);

    // Чтение из файла построчно
    std::string line;
    while (std::getline(input, line)) {
        json dataLine = json::parse(line);
        std::cout << dataLine << std::endl;

        std::vector<Vector2> tmpPolygon;

        for(int i = 0; i < dataLine.size(); i++) {
            std::cout << dataLine[i] << std::endl;
            //dataLine[i];

            Vector2 tmpPoint;
            tmpPoint.x = dataLine[i][0];
            tmpPoint.y = dataLine[i][1];

            // Добавляем точку к полигону
            tmpPolygon.push_back(tmpPoint);
            std::cout << "x = " << tmpPolygon.at(tmpPolygon.size() - 1).x << " y = " << tmpPolygon.at(tmpPolygon.size() - 1).y << std::endl;
        }

        // Добавляем полигон к вектору полигонов
        Polygons.push_back(tmpPolygon);

    }

    // TODO закрыть файл!

//    // Access the values existing in JSON data
//    string name = data.value("name", "not found");
//    string grade = data.value("grade", "not found");
//    // Access a value that does not exist in the JSON data
//    string email = data.value("email", "not found");
//    // Print the values
//    cout << "Name: " << name << endl;
//    cout << "Grade: " << grade << endl;

//    cout << "Email: " << email << endl;

}

int triangulateMap(std::vector<p2t::Triangle*> &triangles,
                   std::vector<std::vector<Vector2>> &Polygons,
                   std::vector<std::vector<p2t::Triangle*>> &trianglesHoles,
                   std::vector<p2t::Point*> &polyline,
                   std::vector<p2t::Point*> &steiner) {


    /*
     * STEP 1: Create CDT and add primary polyline
     * NOTE: polyline must be a simple polygon. The polyline's points
     * constitute constrained edges. No repeat points!!!
     */

    /// Constrained triangles
    //std::vector<p2t::Triangle*> triangles;
    /// Triangle map
    std::list<p2t::Triangle*> map;
    //------------------------------------

    //std::vector<p2t::Point*> polyline;
    std::vector<std::vector<p2t::Point*>> holes;
    //std::vector<p2t::Point*> steiner;

    if (Polygons.empty()) {
        std::cout << "In file no polygons!" << std::endl;
        return 1;
    }

    // Если бы не проверка на пустоту Polygons, то был бы сегфолт...

    // Считываем границы игрового поля из нулевого полигона
    for (int i = 0; i < Polygons[0].size(); i++) {
        //DrawLineEx(tmpPolygon[i], tmpPolygon[i + 1], thickness, color);
        // TODO Создаём в динамической памяти новые точки
        //std::unique_ptr<> name = std::make_unique(type);
        p2t::Point* tmpPoint = new p2t::Point(Polygons[0][i].x, Polygons[0][i].y);
        polyline.push_back(tmpPoint);
    }



    for (int i = 1; i < Polygons.size(); i++) {
    //for (int i = 1; i < 2; i++) {

        std::vector<p2t::Point*> hole;

        for (int j = 0; j < Polygons[i].size(); j++) {
            // Если полигон-точка то добавить в точки штейнера
            if (Polygons[i].size() == 1) {
                steiner.push_back(new p2t::Point(Polygons[i][0].x, Polygons[i][0].y));
            }
            else {
                hole.push_back(new p2t::Point(Polygons[i][j].x, Polygons[i][j].y));
            }

        }
        if (!hole.empty()) holes.push_back(hole);
    }

    p2t::CDT* cdt = new p2t::CDT(polyline);

    /*
     * STEP 2: Add holes or Steiner points
     */
    for (const auto& hole : holes) {
      assert(!hole.empty());
      cdt->AddHole(hole);
    }

    // TODO Добавить точки начала и конца
    for (const auto& s : steiner) {
      cdt->AddPoint(s);
    }

    /*
     * STEP 3: Triangulate!
     */
    cdt->Triangulate();

    triangles = cdt->GetTriangles();
    map = cdt->GetMap();


    /// Constrained triangles
    // Треугольники для отрисовки полигонов-дырок
    //std::vector<std::vector<p2t::Triangle*>> trianglesHoles;

    std::vector<p2t::Triangle*> tmpTrianglesHole;

    for (const auto& hole : holes) {
      assert(!hole.empty());

      p2t::CDT* cdtHole = new p2t::CDT(hole);

      cdtHole->Triangulate();

      tmpTrianglesHole = cdtHole->GetTriangles();

      trianglesHoles.push_back(tmpTrianglesHole);

    }

    return 0;
}

void initPhysics(b2World* m_world,
                 Vector2 player,
                 b2Body* bodyPlayer,
                 std::vector<std::vector<p2t::Triangle*>> &trianglesHoles,
                 std::vector<p2t::Point*> &polyline) {

    //--------------------------------------------------------------------------------------
    // Работа с box2d
    //--------------------------------------------------------------------------------------

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

            b2Vec2 v1(polyline[0]->x/kScale, polyline[0]->y/kScale);
            b2Vec2 v2(polyline[1]->x/kScale, polyline[1]->y/kScale);
            b2Vec2 v3(polyline[2]->x/kScale, polyline[2]->y/kScale);
            b2Vec2 v4(polyline[3]->x/kScale, polyline[3]->y/kScale);

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

    // Создание динамического тела

    b2PolygonShape shape;
    shape.SetAsBox(8.0f/kScale, 8.0f/kScale); // TODO Заменить деление умножением
    //b2CircleShape shape;
    //shape.m_radius = 8.0f/kScale;


    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.restitution = 0.1f;
    fd.restitutionThreshold = 0.1f; // Порог при котором еще происходит отскакивание

    //b2BodyDef bd;
    //bd.type = b2_dynamicBody;

//    bd.position.Set(-40.0f,5.0f);
    //bd.position.Set(player.x/kScale, player.y/kScale);

//    bd.bullet = true; // Что это значит?

    //b2Body* bodyPlayer = m_world->CreateBody(&bd);
    bodyPlayer->CreateFixture(&fd);
    //bodyPlayer->SetLinearVelocity(b2Vec2(-12.0f, -2.0f));


    // Можно создавать shape в виде окружности, полигона

    // Создание тела из нескольких фигур

    //--------------------------------------------------------------------------------------
    // Создание составных тел - препятствий из треугольников после триангуляции полигонов - препятствий
    // Взять набор треугольников принадлежащих препятствию

    // Перебор дырок
    for (int i = 0; i < trianglesHoles.size(); i++) {
        // -----------------------------------------
        b2Body* obstacle;

        b2BodyDef bd;
        bd.type = b2_staticBody;

        p2t::Triangle& t = *trianglesHoles[i][0];
        p2t::Point& pos = *t.GetPoint(0);

        //bd.position.Set(5.0f, 1.0f);
        bd.position.Set(pos.x/kScale, pos.y/kScale);
        obstacle = m_world->CreateBody(&bd);
        // -----------------------------------------

        std::vector<p2t::Triangle*> tmpTrianglesHole;

        // Перебор треугольников внутри i-дырки
        for (int j = 0; j < trianglesHoles[i].size(); j++) {

            // ------------------------------------------------

            p2t::Triangle& t = *trianglesHoles[i][j];
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



