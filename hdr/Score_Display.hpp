#ifndef SCORE_DISPLAY_H
#define SCORE_DISPLAY_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "Spacial_Object.hpp"

class Score_Display : public Spacial_Object {

    private:
        SDL_Color color;

        TTF_Font* font = nullptr;

        SDL_Texture* texture = nullptr;
        SDL_Renderer* renderer = nullptr;


    public:
        Score_Display(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, std::string text, Position position);
        ~Score_Display();
        void set_display_number(int score);
        void render(SDL_Renderer* renderer);

};

#endif