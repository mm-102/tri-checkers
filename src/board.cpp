#include <board.hpp>
#include <game.hpp>
#include <settings.hpp>
#include <allegro5/allegro_primitives.h>
#include <utility>
#include <cmath>
#include <iostream>

Board::Board(Textures *textures, Camera *camera)
{
    this->camera = camera;
    this->tile_color = al_map_rgb(255, 205, 177);
    this->background_color = al_map_rgb(115, 23, 0);
    this->board_size_factor = 1;
    tileSelect = new TileSelect(camera, textures->TILE_SELECT);
    ALLEGRO_BITMAP* tile_texture = textures->TILE;
    texW = al_get_bitmap_width(tile_texture);
    texH = al_get_bitmap_height(tile_texture);
    const int min_y = -BOARD_SIZE;
    const int max_y = (BOARD_SIZE - 1) * 2;
    tiles = std::vector<std::vector<Tile *>>(max_y-min_y+1);
    int min_x = 0;
    int max_x = 0;
    for (int y = max_y; y >= min_y; y--)
    {
        std::vector<Tile *> row;
        for (int x = min_x; x <= max_x; x++){
            PieceType type = PieceType::NONE;
            PieceColor color = PieceColor::NONE;
            if (max_y - y < BOARD_SIZE) // bottom
            {
                type = PieceType::PAWN;
                color = PieceColor::RED;
            }
            else if (x - min_x < BOARD_SIZE - (y - min_y)) // top left
            {
                type = PieceType::PAWN;
                color = PieceColor::BLUE;
            }
            else if (max_x - x < BOARD_SIZE - (y - min_y)) // top right
            {
                type = PieceType::PAWN;
                color = PieceColor::GREEN;
            }

            row.push_back(new Tile(x, y, textures, tile_color, type, color));
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
            t->neighbours[2] = (y > 0 && x < tiles[y].size() - 1) ? tiles[y-1][x+1] : nullptr; // F_RIGHT
            t->neighbours[3] = (x < tiles[y].size() - 1) ? tiles[y][x+1] : nullptr; // RIGHT
            t->neighbours[4] = (y < tiles.size() - 1) ? tiles[y+1][x] : nullptr; // B_RIGHT
            t->neighbours[5] = (x > 0 && y < tiles.size() - 1) ? tiles[y+1][x-1] : nullptr; // B_LEFT
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
    int col = floorf(x / texW) + floorf(BOARD_SIZE * 0.5) - 1 - (1 - (BOARD_SIZE % 2)) * (abs(row) % 2);
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
    int vec_y = y + BOARD_SIZE;
    int vec_x = x + BOARD_SIZE - vec_y / 2;
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
        if(tile != nullptr)
            tile = tile->move(PieceMoveDir::B_RIGHT, PieceColor::RED);
        tileSelect->add_node(tile);
    }
}