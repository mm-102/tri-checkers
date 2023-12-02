#pragma once
#include <tile.hpp>
#include <vector>
#include <allegro5/allegro_image.h>

class Board{
    std::vector<std::vector<Tile*>> tiles;
    ALLEGRO_BITMAP* tile_texture;
    public:
    Board(ALLEGRO_BITMAP* tile_texture);
    void draw();
};