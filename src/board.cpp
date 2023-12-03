#include <board.hpp>
#include <settings.hpp>
#include <allegro5/allegro_primitives.h>
#include <utility>
#include <cmath>
#include <iostream>

Board::Board(ALLEGRO_BITMAP *tile_texture, Camera *camera)
{
    this->camera = camera;
    this->tile_color = al_map_rgb(255, 205, 177);
    this->background_color = al_map_rgb(115, 23, 0);
    this->board_size_factor = 1;
    this->tile_texture = tile_texture;
    texW = al_get_bitmap_width(tile_texture);
    texH = al_get_bitmap_height(tile_texture);
    const int min_y = -BOARD_SIZE;
    const int max_y = (BOARD_SIZE - 1) * 2;
    int min_x = -BOARD_SIZE;
    int max_x = BOARD_SIZE + (BOARD_SIZE % 2);
    for (int y = min_y; y <= max_y; y++)
    {
        std::vector<Tile *> row;
        for (int x = min_x; x <= max_x; x++)
        {
            row.push_back(new Tile(x, y, tile_texture, tile_color));
        }
        tiles.push_back(row);
        if (abs(y) % 2)
            max_x--;
        else
            min_x++;
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
    for (std::vector<Tile *> row : tiles)
    {
        for (Tile *tile : row)
            tile->draw();
    }
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
    int col = floorf(x / texW);
    return get_tile_from_coords(col, row);
}

Tile *Board::get_tile_from_coords(int x, int y)
{
    int vec_y = y + BOARD_SIZE;
    int vec_x = x + BOARD_SIZE - vec_y / 2;
    // std::cout << vec_x << " " << vec_y << std::endl;
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
        if (tile != nullptr)
        {
            // std::cout << tile->get_coords()[0] << " " << tile->get_coords()[1] << std::endl;
        }
    }
}