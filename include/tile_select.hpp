#pragma once
#include <camera.hpp>
#include <queue>
#include <allegro5/allegro5.h>
#include <tile.hpp>

class TileSelect
{
    typedef struct TailNode
    {
        float draw_x;
        float draw_y;
        ALLEGRO_COLOR color;
        double spawn_time;
    } TailNode;

    Camera *camera;
    std::queue<TailNode> tail;
    ALLEGRO_COLOR main_color;
    int main_x, main_y;
    float draw_x, draw_y;
    bool draw_main;

public:
    TileSelect(Camera *camera);
    void draw();
    void add_node(Tile *tile);
};