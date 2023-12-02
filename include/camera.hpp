#pragma once
#include <allegro5/allegro5.h>
#include <settings.hpp>

class Camera
{
    ALLEGRO_TRANSFORM transform;
    const float sensivity = 0.05;
    const float halfx = SCREEN_X * 0.5;
    const float halfy = SCREEN_Y * 0.5;
    float pos[2];
    float zoom;
    float rotation;
    bool pressed;
    bool mouse_in_display;
    void zoom_to_point(ALLEGRO_MOUSE_EVENT mouse);
    void drag(ALLEGRO_MOUSE_EVENT mouse);

public:
    Camera();
    void handle_event(ALLEGRO_EVENT event);
    void update();
};