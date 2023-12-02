#pragma once
#include<allegro5/allegro5.h>
#include<allegro5/allegro_image.h>
#include<camera.hpp>

#define TEXTURE_PATH "../assets/textures/"

class Game{
    enum TextureName{
        TILE,
        TEXTURES_NO
    };
    const ALLEGRO_COLOR light_tile_color = al_map_rgb(255, 255, 255);
    const ALLEGRO_COLOR dark_tile_color = al_map_rgb(78, 78, 78);

    ALLEGRO_DISPLAY* display;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_BITMAP** textures;

    Camera camera;

    void handle_event(ALLEGRO_EVENT event);
    void load_textures();
    public:
    Game();
    ~Game();
    void start();
};