#include <board.hpp>
#include <settings.hpp>
#include <allegro5/allegro_primitives.h>
#include <utility>
#include <cmath>

Board::Board(ALLEGRO_BITMAP* tile_texture){
    this->tile_color = al_map_rgb(255, 205, 177);
    this->background_color = al_map_rgb(115, 23, 0);
    this->board_size_factor = 1;
    this->tile_texture = tile_texture;
    const int min_y = -BOARD_SIZE;
    const int max_y = (BOARD_SIZE-1)*2;
    int min_x = -BOARD_SIZE;
    int max_x = BOARD_SIZE+(BOARD_SIZE % 2);
    for(int y = min_y; y <= max_y; y++){
        std::vector<Tile*> row;
        for(int x = min_x; x <= max_x; x++){
            row.push_back(new Tile(x,y,tile_texture, tile_color));
        }
        tiles.push_back(row);
        if(abs(y) % 2)
            max_x--;
        else
            min_x++;
    }
    const std::pair<int, int> p1 = tiles[0].front()->get_center_pos();
    const std::pair<int, int> p2 = tiles[0].back()->get_center_pos();
    const std::pair<int, int> p3 = tiles.back()[0]->get_center_pos();
    const int texW = al_get_bitmap_width(tile_texture);
    const int texH = al_get_bitmap_height(tile_texture);
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

void Board::draw(){
    al_draw_filled_triangle(
        trangle_points[0],
        trangle_points[1],
        trangle_points[2],
        trangle_points[3],
        trangle_points[4],
        trangle_points[5],
        background_color
        );
    for(std::vector<Tile*> row : tiles){
        for(Tile* tile : row)
            tile->draw();
    }
}