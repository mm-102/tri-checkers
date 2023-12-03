#pragma once
#include <allegro5/allegro5.h>
#include <settings.hpp>

class Camera
{
    ALLEGRO_TRANSFORM transform;
    ALLEGRO_TRANSFORM inv_transform;
    const float sensivity = 0.05;
    const float halfx = SCREEN_X * 0.5;
    const float halfy = SCREEN_Y * 0.5;
    const double min_zoom = 0.25;
    const double max_zoom = 4;
    double mz;
    double pos[2];
    double zoom;
    float rotation;
    bool pressed;
    bool mouse_in_display;
    void zoom_to_point(ALLEGRO_MOUSE_EVENT mouse);
    void drag(ALLEGRO_MOUSE_EVENT mouse);
    void update_inv_transform();

public:
    Camera();
    void handle_event(ALLEGRO_EVENT event);
    void rotate_to(float a);
    void rotate(float da);
    void update();
};