#pragma once
#include <camera.hpp>
#include <deque>
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

    const double tail_lifeTime = 0.2;
    Camera *camera;
    std::deque<TailNode> tail;
    ALLEGRO_COLOR main_color;
    ALLEGRO_COLOR tail_color;
    ALLEGRO_BITMAP *texture;
    int texWOff, texHOff;
    Tile *current;
    float draw_x, draw_y;

public:
    TileSelect(Camera *camera, ALLEGRO_BITMAP *texture);
    void draw();
    void add_node(Tile *tile);
};