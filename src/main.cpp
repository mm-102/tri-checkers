#include<allegro5/allegro5.h>
#include<allegro5/allegro_image.h>
#include<game.hpp>

int main(){
    al_init();
    al_init_image_addon();

    Game game;
    game.start();
    return 0;
}