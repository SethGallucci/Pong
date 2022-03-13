#include "Spacial_Object.hpp"

Position Spacial_Object::get_position(){
    return position;
}

Dimensions Spacial_Object::get_dimensions(){
    return dimensions;
}

SDL_Rect Spacial_Object::get_spacials(){
    return {
        .x = (int)position.x,
        .y = (int)position.y,
        .w = (int)dimensions.width,
        .h = (int)dimensions.height
    };
}

void Spacial_Object::set_position(Position position){
    this->position = position;
}