#include <board.hpp>
#include <settings.hpp>

Board::Board(ALLEGRO_BITMAP* tile_texture){
    this->tile_texture = tile_texture;
    const int min_y = -BOARD_SIZE;
    const int max_y = (BOARD_SIZE-1)*2;
    int min_x = -BOARD_SIZE;
    int max_x = BOARD_SIZE+(BOARD_SIZE % 2);
    for(int y = min_y; y <= max_y; y++){
        std::vector<Tile*> row;
        for(int x = min_x; x <= max_x; x++){
            row.push_back(new Tile(x,y,tile_texture));
        }
        tiles.push_back(row);
        if(abs(y) % 2)
            max_x--;
        else
            min_x++;
    }
}

void Board::draw(){
    for(std::vector<Tile*> row : tiles){
        for(Tile* tile : row)
            tile->draw();
    }
}