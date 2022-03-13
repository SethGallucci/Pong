#ifndef PADDLE_H
#define PADDLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Physics_Object.hpp"

class Paddle : public Physics_Object {

    private:
        float target_y_location;

    public:
        static const int WIDTH;
        static const int HEIGHT;
        static const SDL_Color COLOR;
        static const float MOVING_SPEED;

        Paddle(Position position);
        
        void render(SDL_Renderer* renderer);
        void update_position(float dt);
        void set_target_y_location(float target_y_location);
};

#endif