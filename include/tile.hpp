#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <utility>
#include <piece.hpp>
#include <textures.hpp>

class Tile
{
    int pos[2];
    std::pair<int, int> center_pos;
    int draw_pos[2];
    Textures *textures;
    ALLEGRO_COLOR color;

public:
    PieceType piece_type;
    PieceColor piece_color;
    Tile(int x, int y, Textures *textures, ALLEGRO_COLOR color, PieceType start_piece, PieceColor start_color);
    void draw();
    std::pair<int, int> get_center_pos();
    const int *get_coords() { return pos; };
};