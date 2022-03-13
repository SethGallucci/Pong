#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H


#include <SDL2/SDL.h>

#include "Spacial_Object.hpp"

struct Velocity {
    float x;
    float y;
};

class Physics_Object : public Spacial_Object {
    protected:
        struct Velocity velocity;
    
    public:
        Velocity get_velocity();

        virtual void render(SDL_Renderer* renderer) = 0;
    
};

#endif