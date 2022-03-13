#include "Score_Display.hpp"

Score_Display::Score_Display(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, std::string text, Position position){
    this->renderer = renderer;
    this->font = font;
    this->color = color;
    set_display_number(0);
}

Score_Display::~Score_Display(){
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void Score_Display::set_display_number(int score){
    
    SDL_Surface* surface = TTF_RenderText_Blended(font, std::to_string(score).c_str(), color);
    
    if(texture){
        SDL_DestroyTexture(texture);
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureAlphaMod(texture, color.a);

    position = position;
    dimensions = {
        .width = (float)surface->w,
        .height = (float)surface->h
    };
    
    SDL_FreeSurface(surface);
}

void Score_Display::render(SDL_Renderer* renderer){
    SDL_Rect spacials = {
        .x = (int)position.x,
        .y = (int)position.y,
        .w = (int)dimensions.width,
        .h = (int)dimensions.height
    };

    SDL_RenderCopy(renderer, texture, NULL, &spacials);
}