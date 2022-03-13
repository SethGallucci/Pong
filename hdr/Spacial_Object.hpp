#ifndef SPACIAL_OBJECT_H
#define SPACIAL_OBJECT_H

#include <SDL2/SDL.h>

struct Position {
    float x;
    float y;
};

struct Dimensions {
    float width;
    float height;
};

class Spacial_Object {
    protected:
        struct Position position;
        struct Dimensions dimensions;
        SDL_Color color;
    
    public:
        Position get_position();
        Dimensions get_dimensions();
        SDL_Rect get_spacials();
        void set_position(Position position);

        virtual void render(SDL_Renderer* renderer) = 0; 
};

#endif