#ifndef PONG_H
#define PONG_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <time.h>
#include <vector>
#include <experimental/filesystem>

#include "Paddle.hpp"
#include "Ball.hpp"
#include "Score_Display.hpp"


enum Game_States{
    SETTING_UP_RALLY,
    INITIATING_RALLY,
    PLAYING_RALLY,
    UPDATING_SCORE_DISPLAYS
};

struct Score {
    int player, opponent;
};

class Pong {

    private:
        SDL_Rect screen_spacials;

        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;

        Paddle* player_paddle = nullptr;
        Paddle* opponent_paddle = nullptr;

        Ball* ball = nullptr;

        static const float SERVE_SPEED;
        static const float SPEED_MULTIPLIER;

        int current_game_state;

        struct Score score;

        static const std::string SCORE_FONT_PATH;
        static const int SCORE_FONT_SIZE;
        static const SDL_Color SCORE_FONT_COLOR;
        
        TTF_Font* score_font = nullptr;

        Score_Display* player_score_display = nullptr;
        Score_Display* opponent_score_display = nullptr;

        static const std::string COLLISION_SOUND_EFFECTS_PATH;
        std::vector<Mix_Chunk*> collision_sound_effects;

        bool initalize_SDL_elements();
        float calculate_opponent_paddle_target_y_location();
        Velocity calculate_post_hit_ball_velocity(Ball* ball, Paddle* paddle);
        void play_random_collision_sound_effect();
        void set_up_rally();
        void initiate_rally(float dt);
        void play_rally(float dt);
        void update_score_displays();
        void step(float dt);
        void render();


    public:
        Pong();
        ~Pong();
        void play();

};

#endif