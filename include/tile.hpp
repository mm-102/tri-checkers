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
    ALLEGRO_COLOR *colors;

public:
    enum class Mode
    {
        NORMAL,
        SELECTED,
        HINT,
        CAPTURE
    } mode;

    Tile **neighbours;
    PieceType piece_type;
    PieceColor piece_color;
    Tile(int x, int y, Textures *textures, PieceType start_piece, PieceColor start_color, bool spawn);
    ~Tile();
    void draw();
    std::pair<int, int> get_center_pos();
    Tile* move(PieceMoveDir move_dir, PieceColor p_col);
    const int *get_coords() { return pos; };
};