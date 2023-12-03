#include <camera.hpp>
#include <cmath>

Camera::Camera()
{
    pos[0] = halfx;
    pos[1] = halfy;
    zoom = 1;
    rotation = 0;
    pressed = false;
    mouse_in_display = false;
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
    case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
        mouse_in_display = false;
    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
        pressed = false;
        break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
        pressed = true;
        break;
    case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
        mouse_in_display = true;
        break;

    case ALLEGRO_EVENT_MOUSE_AXES:
        if (mouse_in_display)
        {
            zoom_to_point(event.mouse);
            if (pressed)
                drag(event.mouse);
        }
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
    pos[0] += mouse.dx;
    pos[1] += mouse.dy;
    update_inv_transform();
}

void Camera::rotate_to(float a)
{
    rotate(a - rotation);
}

void Camera::rotate(float da)
{
    al_translate_transform(&transform, -pos[0], -pos[1]);
    al_rotate_transform(&transform, da);
    al_translate_transform(&transform, pos[0], pos[1]);
    update_inv_transform();
}

void Camera::update()
{
    al_use_transform(&transform);
}

void Camera::revert_transform(float *x, float *y)
{
    al_transform_coordinates(&inv_transform, x, y);
}