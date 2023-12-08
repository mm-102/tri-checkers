#include <tile.hpp>
#include <game.hpp>

Tile::Tile(int x, int y, Textures *textures, ALLEGRO_COLOR color, PieceType start_piece, PieceColor start_color)
{
    piece_type = start_piece;
    piece_color = start_color;
    pos[0] = x;
    pos[1] = y;
    this->textures = textures;
    this->color = color;
    ALLEGRO_BITMAP *tile_texture = textures->TILE;
    const int texH = al_get_bitmap_height(tile_texture);
    const int texW = al_get_bitmap_width(tile_texture);
    center_pos.first = (-(abs(pos[1]) % 2) * 0.5 + pos[0]) * texW;
    center_pos.second = (0.666 + pos[1]) * texH;
    draw_pos[0] = (-(1 + (abs(pos[1]) % 2)) * 0.5 + pos[0]) * texW;
    draw_pos[1] = (0.333 + pos[1]) * texH;
}
void Tile::draw()
{
    al_draw_tinted_bitmap(textures->TILE, color, draw_pos[0], draw_pos[1], 0);
    if (piece_type != PieceType::NONE)
        al_draw_tinted_bitmap((*textures)[piece_type], PieceDrawColor[piece_color], draw_pos[0], draw_pos[1], 0);
}
std::pair<int, int> Tile::get_center_pos()
{
    return center_pos;
}