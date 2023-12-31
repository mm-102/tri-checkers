#pragma once
#include <textures.hpp>
#include <tile.hpp>
#include <camera.hpp>
#include <vector>
#include <unordered_map>
#include <allegro5/allegro_image.h>
#include <tile_select.hpp>

class Board
{
    Camera *camera;
    std::vector<std::vector<Tile *>> tiles;
    ALLEGRO_COLOR tile_color;
    ALLEGRO_COLOR background_color;
    float trangle_points[6];
    float board_size_factor;
    int texW, texH;
    TileSelect *tileSelect;
    Tile *selected_tile;
    bool capture_streak;
    PieceColor active_player;

    std::vector<std::vector<Tile*>> pieces;
    std::unordered_map<Tile*,Tile*> active_piece_moves;
    std::vector<Tile*> movable_pieces;


    void gen_pawn_move(Tile *tile, PieceMoveDir moveDir, bool capture_only);
    void gen_queen_move(Tile *tile, PieceMoveDir moveDir, bool capture_only);

public:

    Board(Textures *textures, Camera *camera);
    ~Board();
    void draw();
    void handle_event(ALLEGRO_EVENT event);
    Tile *get_tile_from_pos(float x, float y);
    Tile *get_tile_from_mouse_pos(float x, float y);
    Tile *get_tile_from_coords(int x, int y);
    void gen_avaliable_moves(Tile *tile, bool capture_only = false);
    void hint_avaliable_moves(bool capture_only = false);
    void reset_avaliable_moves();
};
