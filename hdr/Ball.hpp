#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Physics_Object.hpp"

class Ball : public Physics_Object {

    public:
        static const int BREADTH;
        static const SDL_Color COLOR;

        Ball(Position position);

        void render(SDL_Renderer* renderer);
        void update_position(float dt);
        void set_position(Position position);
        void set_velocity(Velocity velocity);

};

#endif