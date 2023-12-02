#include <tile.hpp>

Tile::Tile(int x, int y, ALLEGRO_BITMAP *texture)
{
    pos[0] = x;
    pos[1] = y;
    this->texture = texture;
    const int texH = al_get_bitmap_height(texture);
    const int texW = al_get_bitmap_width(texture);
    draw_pos[0] = (-(1 + (abs(pos[1]) % 2)) * 0.5 + pos[0]) * texW;
    draw_pos[1] = (0.5 + pos[1]) * texH;
}
void Tile::draw(){
    al_draw_tinted_bitmap(texture, al_map_rgb(255, 255, 255), draw_pos[0], draw_pos[1], 0);
}