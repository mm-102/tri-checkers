#include <game.hpp>
#include <settings.hpp>
#include <iostream>

Game::Game()
{
    al_set_new_display_flags(ALLEGRO_PROGRAMMABLE_PIPELINE | ALLEGRO_OPENGL);
    this->display = al_create_display(SCREEN_X, SCREEN_Y);
    this->camera.init_shaders();
    this->event_queue = al_create_event_queue();
    this->timer = al_create_timer(1.0 / FPS);
    this->textures = new Textures();
    this->board = new Board(textures, &camera, 4);
    this->paused = false;
    this->pauseMenu = new PauseMenu(SCREEN_X/2 - 200, SCREEN_Y/2 - 250, 400, 500, textures->PAUSE_MENU, [&](int size){set_board_size(size);});
    this->gui.push_back((InterfaceElement*)new PauseButton(SCREEN_X-60,10,50,50,textures->PAUSE_BUTTON, [&](){set_paused();}));
    this->gui.push_back((InterfaceElement*)this->pauseMenu);
    // this->gui.push_back((InterfaceElement*)new Slider(SCREEN_X-500, 50, 256, 64, 67, 64, textures->PAUSE_MENU[0], textures->PAUSE_MENU[1]));
}
Game::~Game()
{
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    delete textures;
    delete board;
    for(InterfaceElement* ele : gui){
        delete ele;
    }
}
void Game::handle_event(ALLEGRO_EVENT event)
{
    camera.handle_event(event);
    if(event.type == ALLEGRO_EVENT_KEY_DOWN){
        // std::cout << event.keyboard.keycode << std::endl;
        if(event.keyboard.keycode == 59){ //esc
            set_paused();
        }
    }
    if (event.type == ALLEGRO_EVENT_TIMER)
    {
        camera.update();
        board->draw();
        camera.disable();
        for(InterfaceElement* ele : gui){
            ele->draw();
        }

        al_flip_display();
        al_clear_to_color(al_map_rgb(255, 227, 181));
    }
    else
    {
        if(!paused)
            board->handle_event(event);
        for(InterfaceElement* ele : gui){
            ele->update(event);
        }
    }
}
void Game::start()
{
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_start_timer(timer);
    bool running = true;
    while (running)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;
        else
            handle_event(event);
    }
}
void Game::set_paused(){set_paused(!paused);}
void Game::set_paused(bool paused){
    this->paused = paused;
    this->pauseMenu->set_active(paused);
    camera.set_paused(paused);
    std::cout << paused << std::endl;
}

void Game::set_board_size(int size){
    std::cout << size << std::endl;
    delete board;
    this->board = new Board(textures, &camera, size);
    this->camera.rotate_to(0);
}