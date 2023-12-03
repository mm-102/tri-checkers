#include <tile_select.hpp>

TileSelect::TileSelect(Camera *camera)
{
    this->camera = camera;
    main_color = al_map_rgb(255, 0, 0);
    draw_main = false;
}

void TileSelect::add_node(Tile *tile)
{
    // TODO
}