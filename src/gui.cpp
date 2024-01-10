#include <gui.hpp>
#include <iostream>

Button::Button(int x, int y, int w, int h){
    pos[0] = x;
    pos[1] = y;
    size[0] = w;
    size[1] = h;
    hover = false;
}

void Button::update(ALLEGRO_EVENT event){
    if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && hover){
        this->onClick();
        return;
    }
    if(event.type != ALLEGRO_EVENT_MOUSE_AXES)
        return;
    
    hover = true;
    if(event.mouse.x < pos[0]) hover = false;
    else if(event.mouse.x > pos[0] + size[0]) hover = false;
    else if(event.mouse.y < pos[1]) hover = false;
    else if(event.mouse.y > pos[1] + size[1]) hover = false;
}

PauseButton::PauseButton(int x, int y, int w, int h, ALLEGRO_BITMAP *texture):Button(x,y,w,h){
    this->texture = texture;
};

void PauseButton::onClick(){
    std::cout << "XD" << std::endl;
}

void PauseButton::draw(){
    al_draw_bitmap_region(texture, 0, size[1]*hover,size[0],size[1],pos[0],pos[1],0);
}