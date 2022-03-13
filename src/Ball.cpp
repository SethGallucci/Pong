#include "Ball.hpp"

const int Ball::BREADTH = 9;
const SDL_Color Ball::COLOR = {
    .r = 0xff,
    .g = 0xff,
    .b = 0xff,
    .a = 0xff
};

Ball::Ball(Position position){
    this->position = position;
    dimensions = {
        .width = Ball::BREADTH,
        .height = Ball::BREADTH
    };
    velocity = {
        .x = 0.0f,
        .y = 0.0f
    };
    color = Ball::COLOR;
}

void Ball::render(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect spacials = {
        .x = (int)position.x,
        .y = (int)position.y,
        .w = (int)dimensions.width,
        .h = (int)dimensions.height
    };
    SDL_RenderFillRect(renderer, &spacials);
}

void Ball::update_position(float dt){
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
}

void Ball::set_position(Position position){
    this->position = position;
}

void Ball::set_velocity(Velocity velocity){
    this->velocity = velocity;
}