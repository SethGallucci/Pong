#include "Paddle.hpp"

const int Paddle::WIDTH = 10;
const int Paddle::HEIGHT = 45;

const SDL_Color Paddle::COLOR = {
    .r = 0xff,
    .g = 0xff,
    .b = 0xff,
    .a = 0xff
};

const float Paddle::MOVING_SPEED = 500;

Paddle::Paddle(Position position){
    this->position = position;
    dimensions = {
        .width = Paddle::WIDTH,
        .height = Paddle::HEIGHT
    };
    color = Paddle::COLOR;
    velocity = {
        .x = 0.0f,
        .y = 0.0f
    };
    target_y_location = 0;
}

void Paddle::render(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect spacials = {
        .x = (int)position.x,
        .y = (int)position.y,
        .w = (int)dimensions.width,
        .h = (int)dimensions.height
    };
    SDL_RenderFillRect(renderer, &spacials);
}

void Paddle::update_position(float dt){
    if(abs(target_y_location - position.y) < abs(velocity.y * dt)){
        position.y = target_y_location;
        velocity.y = 0;
    }
    else{
        position.y += velocity.y * dt;
        velocity.y = (target_y_location > position.y) ? Paddle::MOVING_SPEED : -Paddle::MOVING_SPEED;
    }
}

void Paddle::set_target_y_location(float target_y_location){
    this->target_y_location = target_y_location;
}