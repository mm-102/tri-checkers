#include <camera.hpp>
#include <cmath>
#include <iostream>

void Camera::init_shaders(){
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    shader = al_create_shader(ALLEGRO_SHADER_AUTO);
    if(!shader){
        std::cerr << "Could not create shader." << std::endl;
        return;
    }
    if(!al_attach_shader_source_file(shader, ALLEGRO_VERTEX_SHADER, SHADER_PATH "default.vert")){
        std::cerr << "Could not attach vertex shader." << std::endl << al_get_shader_log(shader) << std::endl;
        return;
    }
    if(!al_attach_shader_source_file(shader, ALLEGRO_PIXEL_SHADER, SHADER_PATH "shadow.frag")){
        std::cerr << "Could not attach fragment shader." << std::endl << al_get_shader_log(shader) << std::endl;
        return;
    }
    if(!al_build_shader(shader)){
        std::cerr << "Could not build shader." << std::endl;
        return;
    }
}

void Camera::set_use_shadow(bool use){
    if(use)
        al_use_shader(shader);
    else
        al_use_shader(nullptr);
}

Camera::Camera()
{
    zoom = 1;
    rotation = 0;
    pressed = false;
    interpolating = false;
    al_identity_transform(&transform);
    al_translate_transform(&transform, halfx, halfy);
    update_inv_transform();
}

void Camera::update_inv_transform()
{
    al_copy_transform(&inv_transform, &transform);
    al_invert_transform(&inv_transform);
}

void Camera::handle_event(ALLEGRO_EVENT event)
{
    switch (event.type)
    {
    case ALLEGRO_EVENT_TIMER:
        handle_interpolation();
        break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
        pressed = false;
        break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
        pressed = true;
        break;

    case ALLEGRO_EVENT_MOUSE_AXES:
        zoom_to_point(event.mouse);
        if (pressed)
            drag(event.mouse);
    
    }
}

void Camera::zoom_to_point(ALLEGRO_MOUSE_EVENT mouse)
{
    if (!mouse.dz)
        return;
    mz += mouse.dz * sensivity;
    const float wz = expf(mz);
    const float new_zoom = wz < min_zoom ? min_zoom : (wz > max_zoom ? max_zoom : wz);
    mz = logf(new_zoom);
    const float d_zoom = new_zoom - zoom;
    const float zoom_change = d_zoom / zoom;
    const float scale_change = 1 + zoom_change;
    const float mx = mouse.x - halfx, my = mouse.y - halfy;
    al_translate_transform(&transform, -halfx, -halfy);
    al_scale_transform(&transform, scale_change, scale_change);
    const float offX = -mx * zoom_change;
    const float offY = -my * zoom_change;
    al_translate_transform(&transform, offX + halfx, offY + halfy);
    update_inv_transform();
    zoom = new_zoom;
}

void Camera::drag(ALLEGRO_MOUSE_EVENT mouse)
{
    if(!mouse.dx && !mouse.dy)
        return;
    al_translate_transform(&transform, mouse.dx, mouse.dy);
    update_inv_transform();
}

void Camera::rotate_to(float a)
{
    a = a > 2*M_PI ? a - 2*M_PI : a;
    a = a < 0 ? a + 2*M_PI : a;
    rotate(a - rotation);
    rotation = a;
}

void Camera::rotate(float da)
{
    float ox = transform.m[3][0];
    float oy = transform.m[3][1];
    al_translate_transform(&transform, -ox, -oy);
    al_rotate_transform(&transform, da);
    al_translate_transform(&transform, ox, oy);
    update_inv_transform();
    al_use_shader(shader);
    al_set_shader_float("u_rot", rotation);
    al_use_shader(nullptr);
}

void Camera::update()
{
    al_use_transform(&transform);
}

void Camera::revert_transform(float *x, float *y)
{
    al_transform_coordinates(&inv_transform, x, y);
}

void Camera::interpolate_rotate_to(float a, bool only_clockwise){
    interpolating = true;
    inter_start_rot = rotation;
    inter_end_rot = a;
    if(only_clockwise && inter_end_rot < rotation)
        inter_end_rot += 2*M_PI;
    inter_start = al_get_time();
}

void Camera::handle_interpolation(){
    if(!interpolating)
        return;
    double d = (al_get_time() - inter_start);
    if(d >= 1){
        d = 1;
        interpolating = false;
    }

    double i_d = d * d * (3 - 2*d);

    rotate_to(inter_start_rot + (inter_end_rot-inter_start_rot)*i_d);
}