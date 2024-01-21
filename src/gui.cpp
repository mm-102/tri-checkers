#include <gui.hpp>
#include <iostream>
#include <settings.hpp>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

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

PauseButton::PauseButton(int x, int y, int w, int h, ALLEGRO_BITMAP *texture, std::function<void ()> set_paused):Button(x,y,w,h){
    this->texture = texture;
    this->set_paused = set_paused;
}

void PauseButton::onClick(){
    set_paused();
}

void PauseButton::draw(){
    al_draw_bitmap_region(texture, 0, size[1]*hover,size[0],size[1],pos[0],pos[1],0);
}

BoardSizeButton::BoardSizeButton(int x, int y, int w, int h, ALLEGRO_BITMAP *texture, std::function<void ()> set_size):Button(x,y,w,h){
    this->texture = texture;
    this->set_size = set_size;
}

void BoardSizeButton::draw(){
    al_draw_bitmap(texture, pos[0], pos[1], 0);
}

void BoardSizeButton::onClick(){
    set_size();
}

Slider::Slider(int x, int y, int w, int h, int slider_w, int slider_h, ALLEGRO_BITMAP *bar_texture, ALLEGRO_BITMAP *slider_texture,std::function<void (int)> set_board_size){
    this->set_board_size = set_board_size;
    pos[0] = x;
    pos[1] = y;
    size[0] = w;
    size[1] = h;
    slider_size[0] = slider_w;
    slider_size[1] = slider_h;
    hover = false;
    pressed = false;
    min_val = 1;
    max_val = 10;
    val = 4;
    this->bar_texture = bar_texture;
    this->slider_texture = slider_texture;
    update_slider_x();
}

void Slider::update_slider_x(){
    float p = float(val - min_val) / (max_val - min_val);
    slider_x = (size[0] - slider_size[0]) * p + pos[0];
}

void Slider::draw(){
    al_draw_bitmap(bar_texture, pos[0], pos[1],0);
    al_draw_bitmap(slider_texture, slider_x, pos[1], 0);
}

void Slider::update_val(ALLEGRO_MOUSE_EVENT mouse){
    float mx = mouse.x - 0.5*slider_size[0];
    float p = float(mx - pos[0])/(size[0]- slider_size[0]);
    int new_val = int((max_val - min_val) * p + 0.5) + min_val;
    if (val == new_val)
        return;
    val = new_val;
    update_slider_x();
    set_board_size(val);
}

void Slider::update(ALLEGRO_EVENT event){
    if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && hover){
        pressed = true;
        update_val(event.mouse);
        return;
    }
    if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
        pressed = false;
    }
    if(event.type != ALLEGRO_EVENT_MOUSE_AXES)
        return;
    
    hover = true;
    if(event.mouse.x < pos[0] + 0.5*slider_size[0]) hover = false;
    else if(event.mouse.x > pos[0] + size[0] - 0.5*slider_size[0]) hover = false;
    else if(event.mouse.y < pos[1]) hover = false;
    else if(event.mouse.y > pos[1] + size[1]) hover = false;

    if(hover && pressed)
        update_val(event.mouse);
}

PauseMenu::PauseMenu(int x, int y, int w, int h, ALLEGRO_BITMAP **textures, std::function<void (int)> set_board_size){
    this->background = textures[0];
    this->font[0] = al_load_ttf_font("assets/font/coolvetica.ttf", 48, 0);
    this->font[1] = al_load_ttf_font("assets/font/Freshman.ttf", 24, 0);
    this->set_board_size = set_board_size;
    // boardSizeButton = new BoardSizeButton(x,y,64,64,textures[3],[&](){});
    boardSizeSlider = new Slider(x + w/2 - 128, y+96, 256, 64, 67, 64, textures[1], textures[2], [&](int size){change_board_size(size);});
    pos[0] = x;
    pos[1] = y;
    size[0] = w;
    size[1] = h;
    active = false;
    board_size = 4;
}
PauseMenu::~PauseMenu(){
    delete boardSizeButton;
    delete boardSizeSlider;
    al_destroy_font(font[0]);
    al_destroy_font(font[1]);
}

void PauseMenu::change_board_size(int size){
    board_size = size;
    set_board_size(size);
}

void PauseMenu::draw(){
    if(!active)
        return;
    al_draw_bitmap(background, pos[0], pos[1],0);
    boardSizeSlider->draw();
    al_draw_text(font[0], al_map_rgb(0,0,0), pos[0]+30, pos[1]+20,0,"Choose board size");
    char buf[4];
    _itoa_s(board_size, buf, 10);
    al_draw_text(font[0], al_map_rgb(0,0,0), pos[0]+30, pos[1]+100,0,buf);
    al_draw_text(font[1], al_map_rgb(0,0,0), pos[0]+50, pos[1]+200,0,"CHANGING BOARD SIZE");
    al_draw_text(font[1], al_map_rgb(0,0,0), pos[0]+50, pos[1]+230,0,"WILL RESET THE GAME");
    // boardSizeButton->draw();
}

void PauseMenu::update(ALLEGRO_EVENT event){
    if(!active)
        return;
    if(event.type != ALLEGRO_EVENT_MOUSE_AXES &&
     event.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && 
     event.type != ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        return;
    if(event.mouse.x < pos[0]) return;
    if(event.mouse.x > pos[0] + size[0]) return;
    if(event.mouse.y < pos[1]) return;
    if(event.mouse.y > pos[1] + size[1]) return;

    boardSizeSlider->update(event);
    // boardSizeButton->update(event);
}

void PauseMenu::set_active(bool active){
    this->active = active;
}