#pragma once
#include <piece.hpp>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#define TEXTURE_PATH "assets/textures/"

class Textures
{
    ALLEGRO_BITMAP **_piece;
public:
    ALLEGRO_BITMAP *TILE;
    ALLEGRO_BITMAP *TILE_SELECT;
    ALLEGRO_BITMAP *PAUSE_BUTTON;
    ALLEGRO_BITMAP *PAUSE_MENU[3];

    Textures()
    {
        TILE = al_load_bitmap(TEXTURE_PATH "tile.png");
        TILE_SELECT = al_load_bitmap(TEXTURE_PATH "tile_select.png");
        PAUSE_BUTTON = al_load_bitmap(TEXTURE_PATH "pause.png");
        PAUSE_MENU[0] = al_load_bitmap(TEXTURE_PATH "pause_background.png");
        PAUSE_MENU[1] = al_load_bitmap(TEXTURE_PATH "slider_bar.png");
        PAUSE_MENU[2] = al_load_bitmap(TEXTURE_PATH "pause_slider.png");

        _piece = new ALLEGRO_BITMAP*[static_cast<int>(PieceType::NONE)];
        _piece[static_cast<int>(PieceType::PAWN)] = al_load_bitmap(TEXTURE_PATH "pawn.png");
        _piece[static_cast<int>(PieceType::QUEEN)] = al_load_bitmap(TEXTURE_PATH "queen.png");
    }

    ~Textures()
    {
        al_destroy_bitmap(TILE);
        al_destroy_bitmap(TILE_SELECT);
        al_destroy_bitmap(PAUSE_BUTTON);
        for(int t = 0; t < 2; t++)
            al_destroy_bitmap(PAUSE_MENU[t]);
        for(int t = 0; t < static_cast<int>(PieceType::NONE); t++)
            al_destroy_bitmap(_piece[t]);
        delete[] _piece;
    }

    ALLEGRO_BITMAP* operator[](PieceType p)
    {
        return _piece[static_cast<int>(p)];
    }

};