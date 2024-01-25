#include "Map.h"

#include <iostream>
#include <fstream>

#include "json.hpp"
using json = nlohmann::json;

Map::Map()
{

}

//void Map::readMap(std::vector<std::vector<Vector2>> &Polygons) {
void Map::readMap() {

    // Чтение из файла

    std::ifstream input("res/map.json");
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

//int Map::triangulateMap(std::vector<p2t::Triangle*> &triangles,
//                   std::vector<std::vector<Vector2>> &Polygons,
//                   std::vector<std::vector<p2t::Triangle*>> &trianglesHoles,
//                   std::vector<p2t::Point*> &polyline,
//                   std::vector<p2t::Point*> &steiner) {

int Map::triangulateMap() {
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

