#pragma once
#include <tile.hpp>
#include <vector>
#include <allegro5/allegro_image.h>

class Board{
    std::vector<std::vector<Tile*>> tiles;
    ALLEGRO_BITMAP* tile_texture;
    ALLEGRO_COLOR tile_color;
    ALLEGRO_COLOR background_color;
    float trangle_points[6];
    float board_size_factor;
    public:
    Board(ALLEGRO_BITMAP* tile_texture);
    void draw();
};