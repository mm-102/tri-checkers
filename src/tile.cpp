#include <tile.hpp>

Tile::Tile(int x, int y, ALLEGRO_BITMAP *texture, ALLEGRO_COLOR color)
{
    pos[0] = x;
    pos[1] = y;
    this->texture = texture;
    this->color = color;
    const int texH = al_get_bitmap_height(texture);
    const int texW = al_get_bitmap_width(texture);
    center_pos.first = (-(abs(pos[1]) % 2) * 0.5 + pos[0]) * texW;
    center_pos.second = (0.666 + pos[1]) * texH;
    draw_pos[0] = (-(1 + (abs(pos[1]) % 2)) * 0.5 + pos[0]) * texW;
    draw_pos[1] = (0.333 + pos[1]) * texH;
}
void Tile::draw(){
    al_draw_tinted_bitmap(texture, color, draw_pos[0], draw_pos[1], 0);
}
std::pair<int, int> Tile::get_center_pos(){
    return center_pos;
}