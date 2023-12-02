#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

class Tile{
    int pos[2];
    int draw_pos[2];
    ALLEGRO_BITMAP* texture;
    public:
    Tile(int x, int y, ALLEGRO_BITMAP* texture);
    void draw();
};