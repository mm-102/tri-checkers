#pragma once
#include <allegro5/allegro5.h>
#include <settings.hpp>

#define SHADER_PATH "assets/shaders/"

class Camera
{
    ALLEGRO_SHADER* shader;
    ALLEGRO_TRANSFORM transform;
    ALLEGRO_TRANSFORM indentity;
    ALLEGRO_TRANSFORM inv_transform;
    const float sensivity = 0.05;
    const float halfx = SCREEN_X * 0.5;
    const float halfy = SCREEN_Y * 0.5;
    const double min_zoom = 0.25;
    const double max_zoom = 4;
    double mz;
    double zoom;
    float rotation;
    bool pressed;

    bool interpolating;
    double inter_start;
    float inter_start_rot;
    float inter_end_rot;

    void zoom_to_point(ALLEGRO_MOUSE_EVENT mouse);
    void drag(ALLEGRO_MOUSE_EVENT mouse);
    void update_inv_transform();
    void handle_interpolation();

public:
    Camera();
    void init_shaders();
    void set_use_shadow(bool use);
    void handle_event(ALLEGRO_EVENT event);
    void rotate_to(float a);
    void rotate(float da);
    void interpolate_rotate_to(float a, bool only_clockwise=true);
    void update();
    void disable();
    void revert_transform(float *x, float *y);
};