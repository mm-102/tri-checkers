#pragma once
#include<allegro5/allegro5.h>

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
    bool paused;
    void onClick() override;
public:
    PauseButton(int x, int y, int w, int h, ALLEGRO_BITMAP *texture);
    void draw() override;
};