#ifndef MAP_H
#define MAP_H

#include "raylib.h"
#include "poly2tri.h"


class Map
{

public:

    //--------------------------------------------------------------------------------------
    std::vector<std::vector<Vector2>> Polygons;
    //--------------------------------------------------------------------------------------
    // Треугольники для отрисовки полигонов-дырок
    std::vector<std::vector<p2t::Triangle*>> trianglesHoles;
    std::vector<p2t::Point*> polyline;
    //std::vector<std::vector<p2t::Point*>> holes;
    std::vector<p2t::Point*> steiner;
    std::vector<p2t::Triangle*> triangles;

    //--------------------------------------------------------------------------------------

    Map();

    void readMap();

    int triangulateMap();

};

extern Map *pGameMap;

#endif // MAP_H
