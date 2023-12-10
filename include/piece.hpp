#pragma once
#include <allegro5/allegro5.h>

enum class PieceType
{
    PAWN,
    QUEEN,
    NONE
};

enum class PieceColor
{
    RED,
    GREEN,
    BLUE,
    NONE
};

class
{
    ALLEGRO_COLOR _color[3];
    bool constructed{
        [this]
        {
            _color[0] = al_map_rgb(255, 0, 0);
            _color[1] = al_map_rgb(67, 149, 0);
            _color[2] = al_map_rgb(0, 0, 255);
            _color[3] = al_map_rgba_f(0, 0, 0, 0);
            return true;
        }()};

public:
    const ALLEGRO_COLOR &operator[](PieceColor c) const
    {
        return _color[static_cast<int>(c)];
    }
} PieceDrawColor;