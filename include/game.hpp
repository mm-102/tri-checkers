#pragma once
#include <piece.hpp>
#include <textures.hpp>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <camera.hpp>
#include <board.hpp>
#include <gui.hpp>
#include <vector>

class Game
{
    const ALLEGRO_COLOR light_tile_color = al_map_rgb(255, 255, 255);
    const ALLEGRO_COLOR dark_tile_color = al_map_rgb(78, 78, 78);

    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *event_queue;
    Textures *textures;

    Camera camera;
    Board* board;
    bool paused;
    std::vector<InterfaceElement*> gui;
    PauseMenu* pauseMenu;

    void handle_event(ALLEGRO_EVENT event);

public:
    Game();
    ~Game();
    void start();
    void set_paused();
    void set_paused(bool paused);
    void set_board_size(int size);
};