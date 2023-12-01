#pragma once
#include<allegro5/allegro5.h>
#include<allegro5/allegro_image.h>

#define TEXTURE_PATH "../assets/textures/"

class Game{
    enum TextureName{
        TEST1,
        TEXTURES_NO
    };
    ALLEGRO_DISPLAY* display;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_BITMAP** textures;

    void handle_event(ALLEGRO_EVENT event);
    void load_textures();
    public:
    Game();
    ~Game();
    void start();
};