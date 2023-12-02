#include <camera.hpp>

Camera::Camera()
{
    pos[0] = 0;
    pos[1] = 0;
    zoom = 1;
    rotation = 0;
    pressed = false;
    mouse_in_display = false;
    al_identity_transform(&transform);
    al_translate_transform(&transform, halfx, halfy);
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
    const float zoom_change = mouse.dz * sensivity;
    const float scale_change = 1 + zoom_change / zoom;
    zoom += zoom_change;
    al_translate_transform(&transform, -halfx, -halfy);
    al_scale_transform(&transform, scale_change, scale_change);
    const float offX = (halfx - mouse.x) * zoom_change;
    const float offY = (halfy - mouse.y) * zoom_change;
    pos[0] += offX;
    pos[1] += offY;
    al_translate_transform(&transform, offX + halfx, offY + halfy);
}

void Camera::drag(ALLEGRO_MOUSE_EVENT mouse)
{
    al_translate_transform(&transform, mouse.dx, mouse.dy);
    pos[0] += mouse.dx;
    pos[1] += mouse.dy;
}

void Camera::update()
{
    al_use_transform(&transform);
}