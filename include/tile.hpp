#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <utility>

class Tile{
    int pos[2];
    std::pair<int, int> center_pos;
    int draw_pos[2];
    ALLEGRO_BITMAP* texture;
    ALLEGRO_COLOR color;
    public:
    Tile(int x, int y, ALLEGRO_BITMAP* texture, ALLEGRO_COLOR color);
    void draw();
    std::pair<int, int> get_center_pos();
};