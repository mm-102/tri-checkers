#include <board.hpp>
#include <game.hpp>
#include <settings.hpp>
#include <allegro5/allegro_primitives.h>
#include <utility>
#include <cmath>
#include <iostream>
#include <algorithm>

Board::~Board()
{
    delete tileSelect;
}

Board::Board(Textures *textures, Camera *camera, int size)
{
    this->size = size;
    this->camera = camera;
    this->tile_color = al_map_rgb(255, 205, 177);
    this->background_color = al_map_rgb(115, 23, 0);
    this->board_size_factor = 1;
    this->capture_streak = false;
    selected_tile = nullptr;
    active_player = PieceColor::RED;
    force_capture = false;
    tileSelect = new TileSelect(camera, textures->TILE_SELECT);
    for(int c = 0; c < static_cast<int>(PieceColor::NONE); c++){
        std::unordered_set<Tile*> s;
        pieces.push_back(s);
    }
    ALLEGRO_BITMAP* tile_texture = textures->TILE;
    texW = al_get_bitmap_width(tile_texture);
    texH = al_get_bitmap_height(tile_texture);
    const int min_y = -size;
    const int max_y = (size - 1) * 2;
    tiles = std::vector<std::vector<Tile *>>(max_y-min_y+1);
    int min_x = 0;
    int max_x = 0;
    for (int y = max_y; y >= min_y; y--)
    {
        std::vector<Tile *> row;
        for (int x = min_x; x <= max_x; x++){
            PieceType type = PieceType::NONE;
            PieceColor color = PieceColor::NONE;
            bool spawn = false;
            if (max_y - y < size) // bottom
            {
                type = PieceType::PAWN;
                color = PieceColor::RED;
                spawn = true;
            }
            else if (x - min_x < size - (y - min_y)) // top left
            {
                type = PieceType::PAWN;
                color = PieceColor::BLUE;
                spawn = true;
            }
            else if (max_x - x < size - (y - min_y)) // top right
            {
                type = PieceType::PAWN;
                color = PieceColor::GREEN;
                spawn = true;
            }
            Tile* tmp = new Tile(x, y, textures, type, color, spawn);
            if(color != PieceColor::NONE){
                pieces[static_cast<int>(color)].insert(tmp);
            }
            row.push_back(tmp);
        }
        tiles[y-min_y] = row;
        if (abs(y) % 2)
            min_x--;
        else
            max_x++;
    }
    for(int y = 0; y < tiles.size(); y++){
        for(int x = 0; x < tiles[y].size(); x++){
            Tile *t = tiles[y][x];
            t->neighbours[0] = x > 0 ? tiles[y][x-1] : nullptr; // LEFT
            t->neighbours[1] = y > 0 ? tiles[y-1][x] : nullptr; // F_LEFT
            t->neighbours[2] = (y > 0 && x < tiles[y].size()) ? tiles[y-1][x+1] : nullptr; // F_RIGHT
            t->neighbours[3] = (x < tiles[y].size() - 1) ? tiles[y][x+1] : nullptr; // RIGHT
            t->neighbours[4] = (y < tiles.size() - 1 && x < tiles[y+1].size()) ? tiles[y+1][x] : nullptr; // B_RIGHT
            t->neighbours[5] = x > 0 ? tiles[y+1][x-1] : nullptr; // B_LEFT
        }
    }

    const std::pair<int, int> p1 = tiles[0].front()->get_center_pos();
    const std::pair<int, int> p2 = tiles[0].back()->get_center_pos();
    const std::pair<int, int> p3 = tiles.back()[0]->get_center_pos();
    const float b = texW * board_size_factor;
    const float by = b * 0.5;
    const float bx = by * sqrt(3);
    trangle_points[0] = p1.first - bx;
    trangle_points[1] = p1.second - by;
    trangle_points[2] = p2.first + bx;
    trangle_points[3] = p2.second - by;
    trangle_points[4] = p3.first;
    trangle_points[5] = p3.second + b;
    update_movable_pieces();
}

void Board::draw()
{
    al_draw_filled_triangle(
        trangle_points[0],
        trangle_points[1],
        trangle_points[2],
        trangle_points[3],
        trangle_points[4],
        trangle_points[5],
        background_color);
    camera->set_use_shadow(true);
    for (std::vector<Tile *> row : tiles)
    {
        for (Tile *tile : row)
            tile->draw();
    }
    camera->set_use_shadow(false);
    tileSelect->draw();
}

Tile *Board::get_tile_from_mouse_pos(float x, float y)
{
    camera->revert_transform(&x, &y);
    return get_tile_from_pos(x, y);
}

Tile *Board::get_tile_from_pos(float x, float y)
{
    y -= 0.333 * texH;
    int row = floorf(y / texH);
    x += 0.5 * (1 + (abs(row) % 2)) * texW;
    int col = floorf(x / texW) + floorf(size * 0.5) - 1 - (1 - (size % 2)) * (abs(row) % 2);
    int xox = (int)floor(x);
    int yoy = (int)floor(y);
    float ox = (xox % texW + texW) % texW;
    float oy = (yoy % texH + texH) % texH;
    if (ox < texW / 2 && oy > ox * sqrt(3))
        return nullptr;
    else if (ox >= texW / 2 && oy > (texW - ox) * sqrt(3))
        return nullptr;

    return get_tile_from_coords(col, row);
}

Tile *Board::get_tile_from_coords(int x, int y)
{
    int vec_y = y + size;
    int vec_x = x + size - vec_y / 2;
    if (vec_y < 0 || vec_y >= tiles.size())
        return nullptr;
    if (vec_x < 0 || vec_x >= tiles[vec_y].size())
        return nullptr;
    return tiles[vec_y][vec_x];
}

void Board::handle_event(ALLEGRO_EVENT event)
{
    if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        Tile *tile = get_tile_from_mouse_pos(event.mouse.x, event.mouse.y);
        tileSelect->set_color(PieceDrawColor[active_player]);
        tileSelect->add_node(tile);
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
    {
        Tile *tile = get_tile_from_mouse_pos(event.mouse.x, event.mouse.y);

        if(std::find(movable_pieces.begin(),movable_pieces.end(),tile) != movable_pieces.end()){
            reset_avaliable_moves();
            if(selected_tile != nullptr){
                selected_tile->mode = Tile::Mode::NORMAL;
            }
            if(selected_tile != tile){
                selected_tile = tile;
                selected_tile->mode = Tile::Mode::SELECTED;
                gen_avaliable_moves(tile, force_capture);
                hint_avaliable_moves();
            }
            else{
                selected_tile = nullptr;
            }
        }
        else if(std::unordered_map<Tile*, Tile*>::iterator t = active_piece_moves.find(tile); t != active_piece_moves.end()){
            if(tile->move(PieceMoveDir::F_LEFT, active_player) == nullptr){
                tile->piece_type = PieceType::QUEEN;
            }
            else
                tile->piece_type = selected_tile->piece_type;

            tile->piece_color = selected_tile->piece_color;
            pieces[static_cast<int>(active_player)].insert(tile);
            selected_tile->mode = Tile::Mode::NORMAL;
            selected_tile->piece_color = PieceColor::NONE;
            selected_tile->piece_type = PieceType::NONE;
            pieces[static_cast<int>(active_player)].erase(selected_tile);

            reset_avaliable_moves();
            bool next_player = true;
            if(t->second && t->second != (Tile*) 0xfeeefeeefeeefeee){ //TODO fix the win gdb
                pieces[static_cast<int>(t->second->piece_color)].erase(t->second);
                t->second->piece_type = PieceType::NONE;
                t->second->piece_color = PieceColor::NONE;
                gen_avaliable_moves(tile, true);
                if(!active_piece_moves.empty()){
                    capture_streak = true;
                    next_player = false;
                    selected_tile = tile;
                    selected_tile->mode = Tile::Mode::SELECTED;
                    hint_avaliable_moves();
                }
            }
            if(next_player){
                capture_streak = false;
                selected_tile = nullptr;
                for(int p = 0; p < static_cast<int>(PieceColor::NONE); p++){
                    active_player = (PieceColor)((static_cast<int>(active_player) + 1) % static_cast<int>(PieceColor::NONE));
                    double target_rot = M_PI * static_cast<int>(active_player) * 2.0 / 3.0;
                    camera->interpolate_rotate_to(target_rot);
                    tileSelect->reset();
                    update_movable_pieces();
                    if(!movable_pieces.empty()) break;
                }
            }
        }
    }

}
void Board::reset_avaliable_moves(){

    for(std::unordered_map<Tile*,Tile*>::iterator it = active_piece_moves.begin(); it != active_piece_moves.end(); it++){
        if(it->second != nullptr)
            it->second->mode = Tile::Mode::NORMAL;
        it->first->mode = Tile::Mode::NORMAL;
    }
    active_piece_moves.clear();
}
bool Board::gen_pawn_move(Tile *tile, PieceMoveDir moveDir, bool capture_only){
    Tile *moved;
    moved = tile->move(moveDir, active_player);

    if(moved == nullptr) return false;
    if(moved->piece_type == PieceType::NONE){
        if(capture_only) return false;
        active_piece_moves.insert(std::make_pair(moved, nullptr));
        return false;
    }
    if(moved->piece_color == active_player) return false;
    Tile* move_capture = moved->move(moveDir, active_player);
    if(move_capture == nullptr || move_capture->piece_type != PieceType::NONE)
        return false;
    active_piece_moves.insert(std::make_pair(move_capture, moved));
    return true;
}

bool Board::gen_queen_move(Tile *tile, PieceMoveDir moveDir, bool capture_only){
    bool capture_possible = false;
    for(Tile *moved = tile->move(moveDir, active_player); moved != nullptr; moved = moved->move(moveDir, active_player)){
        if(moved->piece_type == PieceType::NONE){
            if(!capture_only)
                active_piece_moves.insert(std::make_pair(moved, nullptr));
            continue;
        }
        if(moved->piece_color == active_player) break;
        Tile* move_capture = moved->move(moveDir, active_player);
        if(move_capture == nullptr || move_capture->piece_type != PieceType::NONE) break;
        active_piece_moves.insert(std::make_pair(move_capture, moved));
        capture_possible = true;
        break;
    }
    return capture_possible;
}

bool Board::gen_avaliable_moves(Tile *tile, bool capture_only){
    bool cp = false;
    reset_avaliable_moves();
    if(tile->piece_type == PieceType::PAWN){
        cp = gen_pawn_move(tile, PieceMoveDir::F_LEFT, capture_only) || cp;
        cp = gen_pawn_move(tile, PieceMoveDir::F_RIGHT, capture_only) || cp;
    }
    else if(tile->piece_type == PieceType::QUEEN){
        for(int d = 0; d < static_cast<int>(PieceMoveDir::NONE); d++){
            cp = gen_queen_move(tile, (PieceMoveDir)d, capture_only) || cp;
        }
    }
    return cp;
}

void Board::hint_avaliable_moves(bool capture_only){
    for(std::unordered_map<Tile*,Tile*>::iterator it = active_piece_moves.begin(); it != active_piece_moves.end(); it++){
        if(it->second != nullptr)
            it->second->mode = Tile::Mode::CAPTURE;
        if(!capture_only || it->second != nullptr)
            it->first->mode = Tile::Mode::HINT;
    }
}

void Board::update_movable_pieces(bool capture_only){
    bool cp;
    movable_pieces.clear();
    int i = static_cast<int>(active_player);
    for(auto it = pieces[i].begin(); it != pieces[i].end(); it++){
        reset_avaliable_moves();
        cp = gen_avaliable_moves(*it, capture_only) || cp;
        if(!active_piece_moves.empty())
            movable_pieces.push_back(*it);
    }
    reset_avaliable_moves();
    force_capture = cp;
    if(cp && !capture_only){
        update_movable_pieces(true);
    }
}