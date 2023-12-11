#include <tile.hpp>
#include <game.hpp>
#include <iostream>

Tile::Tile(int x, int y, Textures *textures, PieceType start_piece, PieceColor start_color)
{
    piece_type = start_piece;
    piece_color = start_color;
    pos[0] = x;
    pos[1] = y;
    this->textures = textures;
    colors = new ALLEGRO_COLOR[3]{al_map_rgb(255, 205, 177), al_map_rgb(128, 102, 88), al_map_rgb(200,180,140)};
    mode = Mode::NORMAL;
    ALLEGRO_BITMAP *tile_texture = textures->TILE;
    const int texH = al_get_bitmap_height(tile_texture);
    const int texW = al_get_bitmap_width(tile_texture);
    center_pos.first = (-(abs(pos[1]) % 2) * 0.5 + pos[0]) * texW;
    center_pos.second = (0.666 + pos[1]) * texH;
    draw_pos[0] = (-(1 + (abs(pos[1]) % 2)) * 0.5 + pos[0]) * texW;
    draw_pos[1] = (0.333 + pos[1]) * texH;
    neighbours = new Tile*[static_cast<int>(PieceMoveDir::NONE)];
}
Tile::~Tile()
{
    delete neighbours; // not delete[] as tiles are deleted by board
}
void Tile::draw()
{
    al_draw_tinted_bitmap(textures->TILE, colors[static_cast<int>(mode)], draw_pos[0], draw_pos[1], 0);
    if (piece_type != PieceType::NONE)
        al_draw_tinted_bitmap((*textures)[piece_type], PieceDrawColor[piece_color], draw_pos[0], draw_pos[1], 0);
}
std::pair<int, int> Tile::get_center_pos()
{
    return center_pos;
}

Tile* Tile::move(PieceMoveDir move_dir, PieceColor p_col)
{
    if (move_dir == PieceMoveDir::NONE)
        return this;
    int m = static_cast<int>(move_dir);
    if (p_col == PieceColor::GREEN)
        m = (m + 4) % static_cast<int>(PieceMoveDir::NONE);
    else if(p_col == PieceColor::BLUE)
        m = (m + 2) % static_cast<int>(PieceMoveDir::NONE);
    return neighbours[m];
}