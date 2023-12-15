#pragma once
#include <textures.hpp>
#include <tile.hpp>
#include <camera.hpp>
#include <vector>
#include <allegro5/allegro_image.h>
#include <tile_select.hpp>

class Board
{
    Camera *camera;
    std::vector<std::vector<Tile *>> tiles;
    std::vector<Tile *> avaliable_moves;
    std::vector<Tile*> capture;
    ALLEGRO_COLOR tile_color;
    ALLEGRO_COLOR background_color;
    float trangle_points[6];
    float board_size_factor;
    int texW, texH;
    TileSelect *tileSelect;
    Tile *selected_tile;
    PieceColor active_player;
    void gen_pawn_move(Tile *tile, PieceMoveDir moveDir);

public:

    Board(Textures *textures, Camera *camera);
    void draw();
    void handle_event(ALLEGRO_EVENT event);
    Tile *get_tile_from_pos(float x, float y);
    Tile *get_tile_from_mouse_pos(float x, float y);
    Tile *get_tile_from_coords(int x, int y);
    void gen_avaliable_moves(Tile *tile);
    void reset_avaliable_moves();
};
