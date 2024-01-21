#pragma once
#include<allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <functional>

class InterfaceElement{
public:
    virtual void draw() = 0;
    virtual void update(ALLEGRO_EVENT event) = 0;
};

class Button : InterfaceElement{
protected:
    int pos[2];
    int size[2];
    bool hover;
    virtual void onClick() = 0;

public:
    Button(int x, int y, int w, int h);
    void update(ALLEGRO_EVENT event) override;
    virtual void draw() = 0;
};

class PauseButton : public Button{
    ALLEGRO_BITMAP* texture;
    std::function<void ()> set_paused;
    void onClick() override;
public:
    PauseButton(int x, int y, int w, int h, ALLEGRO_BITMAP *texture, std::function<void ()> set_paused);
    void draw() override;
};

class BoardSizeButton : public Button{
    ALLEGRO_BITMAP* texture;
    std::function<void ()> set_size;
    void onClick() override;
public:
    BoardSizeButton(int x, int y, int w, int h, ALLEGRO_BITMAP *texture, std::function<void ()> set_size);
    void draw() override;
};

class Slider : InterfaceElement{
protected:
    int pos[2];
    int size[2];
    int slider_size[2];
    bool hover;
    bool pressed;
    int min_val;
    int max_val;
    int val;
    int slider_x;
    ALLEGRO_BITMAP *bar_texture;
    ALLEGRO_BITMAP *slider_texture;
    std::function<void (int)> set_board_size;
    void update_slider_x();
    void update_val(ALLEGRO_MOUSE_EVENT mouse);
public:
    Slider(int x, int y, int w, int h, int slider_w, int slider_h, ALLEGRO_BITMAP *bar_texture, ALLEGRO_BITMAP *slider_texture, std::function<void (int)> set_board_size);
    void draw() override;
    void update(ALLEGRO_EVENT event) override;
};

class PauseMenu : InterfaceElement{
    BoardSizeButton* boardSizeButton;
    Slider* boardSizeSlider;
    ALLEGRO_BITMAP* background;
    ALLEGRO_FONT* font[2];
    int pos[2];
    int size[2];
    bool active;
    int board_size;
    std::function<void (int)> set_board_size;

public:
    PauseMenu(int x, int y, int w, int h, ALLEGRO_BITMAP **textures, std::function<void (int)> set_board_size);
    ~PauseMenu();
    void draw() override;
    void update(ALLEGRO_EVENT event) override;
    void set_active(bool active);
    void change_board_size(int size);
};