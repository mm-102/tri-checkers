#include <game.hpp>

Game::Game()
{
    this->display = al_create_display(800, 600);
    this->event_queue = al_create_event_queue();
    this->timer = al_create_timer(1.0 / 30);
    textures = new ALLEGRO_BITMAP *[TEXTURES_NO];
    load_textures();
}
Game::~Game()
{
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    for (int t = 0; t != TEXTURES_NO; t++)
    {
        al_destroy_bitmap(textures[t]);
    }
    delete[] textures;
}
void Game::load_textures()
{
    textures[TEST1] = al_load_bitmap(TEXTURE_PATH "diamond.png");
}
void Game::handle_event(ALLEGRO_EVENT event)
{
    if (event.type == ALLEGRO_EVENT_TIMER)
    {
        al_draw_bitmap(textures[TEST1], 0, 0, 0);

        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }
}
void Game::start()
{
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

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