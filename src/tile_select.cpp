#include <tile_select.hpp>
#include <iostream>

TileSelect::TileSelect(Camera *camera, ALLEGRO_BITMAP *texture)
{
    this->camera = camera;
    this->texture = texture;
    texWOff = al_get_bitmap_width(texture) / 2;
    texHOff = al_get_bitmap_height(texture) * 0.333;
    main_color = al_map_rgb(255, 0, 0);
    tail_color = al_map_rgb(128, 50, 50);
    current = nullptr;
}

void TileSelect::add_node(Tile *tile)
{
    if (tile != current && current != nullptr)
    {
        TailNode node;
        node.color = al_map_rgba_f(tail_color.r, tail_color.g, tail_color.b, 1);
        std::pair<int, int> cp = current->get_center_pos();
        node.draw_x = cp.first - texWOff;
        node.draw_y = cp.second - texHOff;
        node.spawn_time = al_get_time();
        tail.push_back(node);
    }
    if (tile != current && tile != nullptr)
    {
        std::pair<int, int> cp = tile->get_center_pos();
        draw_x = cp.first - texWOff;
        draw_y = cp.second - texHOff;
    }
    current = tile;
}

void TileSelect::draw()
{
    double time = al_get_time();
    while (!tail.empty() && time - tail.front().spawn_time > tail_lifeTime)
        tail.pop_front();
    for (TailNode node : tail)
    {
        node.color.a = 0.5 * (1 - (time - node.spawn_time) / tail_lifeTime);
        al_draw_tinted_bitmap(texture, node.color, node.draw_x, node.draw_y, 0);
    }

    if (current != nullptr)
    {
        al_draw_tinted_bitmap(texture, main_color, draw_x, draw_y, 0);
    }
}

void TileSelect::set_color(ALLEGRO_COLOR color){
    main_color = color;
    tail_color = al_map_rgb_f(color.r * 0.5, color.g * 0.5, color.b * 0.5);
}

void TileSelect::reset(){
    tail.clear();
    current = nullptr;
}