#include "Pong.hpp"


const std::string Pong::SCORE_FONT_PATH = "fnt/Open 24 Display St.ttf";
const int Pong::SCORE_FONT_SIZE = 40;
const SDL_Color Pong::SCORE_FONT_COLOR = {
            .r = 0xff,
            .g = 0xff,
            .b = 0xff,
            .a = 0x30
};

const float Pong::SERVE_SPEED = 400;
const float Pong::SPEED_MULTIPLIER = 1.15;

const std::string Pong::COLLISION_SOUND_EFFECTS_PATH = "snd/Plink";


bool Pong::initalize_SDL_elements(){


    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    if(TTF_Init() == -1){
        printf("TTF failed to initialize!\nTTF Error: %s\n", TTF_GetError());
        return false;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        printf("SDL could not initialize! SDL Error: %s\n", Mix_GetError());
        return false;
    }


    screen_spacials = {
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        720,
        405
    };

    window = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screen_spacials.w,
        screen_spacials.h,
        (Uint32) NULL
    );

    if(window == nullptr){
        printf("Window could not be created!\nSDL Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if(renderer == nullptr){
        printf("Renderer could not be created!\nSDL Error: %s\n", SDL_GetError());
        return false;
    }

    score_font = TTF_OpenFont(SCORE_FONT_PATH.c_str(), SCORE_FONT_SIZE);

    if(score_font == nullptr){
        printf("Font could not be opened!\nSDL_TTF Error: %s\n", TTF_GetError());
    }


    using directory_entry = std::experimental::filesystem::v1::__cxx11::directory_entry;
    using directory_iterator = std::experimental::filesystem::directory_iterator;
    
    try{
        for(const directory_entry& directory_entry : directory_iterator(COLLISION_SOUND_EFFECTS_PATH)){
            collision_sound_effects.push_back(Mix_LoadWAV(directory_entry.path().c_str()));

            if(collision_sound_effects.back() == nullptr){
                printf("Failed to load sound effect: %s\nMix Error: %s\n", directory_entry.path().c_str(), Mix_GetError());
                return false;
            }
        }
    }
    catch(const std::exception& e){
        printf("%s\n", e.what());
        return false;
    }

    return true;

}

float Pong::calculate_opponent_paddle_target_y_location(){
    
    float paddle_height = opponent_paddle->get_dimensions().height;

    if(ball->get_velocity().x > 0){

        // Use a triangle wave to calculate the destination of the ball.
        // https://en.wikipedia.org/wiki/Triangle_wave#Expression_in_trigonometric_functions
        float wave_height = screen_spacials.h - ball->get_dimensions().height;
        float ball_path_slope = ball->get_velocity().y / ball->get_velocity().x;
        if(ball_path_slope == 0.0f){
            ball_path_slope = FLT_MIN;
        }
        float period = 2 * wave_height / ball_path_slope;
        float phase_shift = ( ball->get_position().y - wave_height/2 ) / ball_path_slope;
        float horizontal_distance = opponent_paddle->get_position().x - ball->get_dimensions().width - ball->get_position().x;

        float ball_destination_y = (wave_height/M_PI) * asin(sin( (2*M_PI/period) * (horizontal_distance + phase_shift) )) + (wave_height/2);

        float random_offset = ((float)rand())/RAND_MAX * paddle_height*2/3 - paddle_height/3;

        return ball_destination_y + random_offset + ball->get_dimensions().height/2 - paddle_height/2;
    }
    else{
        return screen_spacials.h/2 - paddle_height/2;
    }
    
}

Velocity Pong::calculate_post_hit_ball_velocity(Ball* ball, Paddle* paddle){

    float ball_center_x = ball->get_position().x + ball->get_dimensions().width/2;
    float ball_center_y = ball->get_position().y + ball->get_dimensions().height/2;

    float paddle_center_x = paddle->get_position().x + paddle->get_dimensions().width/2;
    float paddle_center_y = paddle->get_position().y + paddle->get_dimensions().height/2;

    float x_distance = ball_center_x - paddle_center_x;
    float y_distance = ball_center_y - paddle_center_y;
    float modified_y_distance = y_distance / 2;

    float ball_velocity_x = ball->get_velocity().x;
    float ball_velocity_y = ball->get_velocity().y;

    float ball_speed = sqrt(ball_velocity_x*ball_velocity_x + ball_velocity_y*ball_velocity_y);

    float post_impact_angle = atan2(modified_y_distance, x_distance);

    float new_ball_speed = ball_speed * SPEED_MULTIPLIER;

    float new_ball_velocity_x = cos(post_impact_angle) * new_ball_speed;
    float new_ball_velocity_y = sin(post_impact_angle) * new_ball_speed;

    return {
        .x = new_ball_velocity_x,
        .y = new_ball_velocity_y
    };

}

void Pong::play_random_collision_sound_effect(){

    try{
        int random_sound_efffect_index = rand()%5;
        Mix_PlayChannel(-1, collision_sound_effects[random_sound_efffect_index], 0);
    }
    catch(const std::exception& e){
        printf("%s\n", e.what());
    }
    
}

void Pong::set_up_rally(){
    
    ball->set_position({
        .x = (float)screen_spacials.w/2 - ball->get_dimensions().width/2,
        .y = (float)screen_spacials.h/2 - ball->get_dimensions().height/2
    });

    ball->set_velocity({
        .x = 0,
        .y = 0
    });

    opponent_paddle->set_target_y_location((float)screen_spacials.h/2 - opponent_paddle->get_dimensions().height/2);

    current_game_state = Game_States::INITIATING_RALLY;

}

void Pong::initiate_rally(float dt){

    SDL_Event* event = nullptr;
    int number_of_events_returned = SDL_PeepEvents(event, 1, SDL_PEEKEVENT, SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONDOWN);
    
    if(number_of_events_returned == 0){
        
        int mouse_y_position;
        SDL_GetMouseState(nullptr, &mouse_y_position);
        player_paddle->set_target_y_location(mouse_y_position - player_paddle->get_dimensions().height/2);
        player_paddle->update_position(dt);
        opponent_paddle->update_position(dt);

    }
    else if(number_of_events_returned > 0){

        float bounded_random_angle = (M_PI_4 * ((float)rand()) / RAND_MAX + (rand()%2 == 0 ? M_PI_4 * 7.0f/2 : M_PI_4 * -1.0f/2));

        ball->set_velocity({
            .x = SERVE_SPEED*cos(bounded_random_angle),
            .y = SERVE_SPEED*sin(bounded_random_angle)
        });

        opponent_paddle->set_target_y_location(calculate_opponent_paddle_target_y_location());

        current_game_state = Game_States::PLAYING_RALLY;
    }
    else {
        printf("Error with SDL_PeepEvents\nSDL Error: %s", SDL_GetError());
    }
}

void Pong::play_rally(float dt){

    int mouse_y_position;
    SDL_GetMouseState(nullptr, &mouse_y_position);
    player_paddle->set_target_y_location(mouse_y_position - player_paddle->get_dimensions().height/2);
    player_paddle->update_position(dt);
    opponent_paddle->update_position(dt);

    ball->update_position(dt);
    
    if(ball->get_position().y < 0){

        play_random_collision_sound_effect();

        ball->set_position({
            .x = ball->get_position().x,
            .y = -ball->get_position().y
        });
        ball->set_velocity({
            .x = ball->get_velocity().x,
            .y = ball->get_velocity().y * -1
        });
    }
    else if(ball->get_position().y > screen_spacials.h - ball->get_dimensions().height){

        play_random_collision_sound_effect();

        ball->set_position({
            .x = ball->get_position().x,
            .y = 2*(screen_spacials.h - ball->get_dimensions().height) - ball->get_position().y
        });
        ball->set_velocity({
            .x = ball->get_velocity().x,
            .y = ball->get_velocity().y * -1
        });
    }


    SDL_Rect player_paddle_spacials = player_paddle->get_spacials();
    SDL_Rect opponent_paddle_spacials = opponent_paddle->get_spacials();
    SDL_Rect ball_spacials = ball->get_spacials();

    if(SDL_HasIntersection(&player_paddle_spacials, &ball_spacials)){

        play_random_collision_sound_effect();

        ball->set_position({
            .x = player_paddle->get_position().x + player_paddle->get_dimensions().width,
            .y = ball->get_position().y
        });

        ball->set_velocity(calculate_post_hit_ball_velocity(ball, player_paddle));

        opponent_paddle->set_target_y_location(calculate_opponent_paddle_target_y_location());

    }
    else if(SDL_HasIntersection(&opponent_paddle_spacials, &ball_spacials)){

        play_random_collision_sound_effect();

        ball->set_position({
            .x = opponent_paddle->get_position().x - ball->get_dimensions().width,
            .y = ball->get_position().y
        });

        ball->set_velocity(calculate_post_hit_ball_velocity(ball, opponent_paddle));

        opponent_paddle->set_target_y_location((float)screen_spacials.h/2 - opponent_paddle->get_dimensions().height/2);

    }

    if(ball->get_position().x <= 0){

        score.opponent += 1;

        current_game_state = Game_States::UPDATING_SCORE_DISPLAYS;

    }
    else if(ball->get_position().x >= screen_spacials.w - ball->get_dimensions().width){

        score.player += 1;

        current_game_state = Game_States::UPDATING_SCORE_DISPLAYS;

    }

}

void Pong::update_score_displays(){
    player_score_display->set_display_number(score.player);
    opponent_score_display->set_display_number(score.opponent);

    current_game_state = Game_States::SETTING_UP_RALLY;
}

void Pong::step(float dt){
    
    switch(current_game_state){
        case Game_States::SETTING_UP_RALLY:
            set_up_rally();
                break;
        case Game_States::INITIATING_RALLY:
            initiate_rally(dt);
                break;
        case Game_States::PLAYING_RALLY:
            play_rally(dt);
                break;
        case Game_States::UPDATING_SCORE_DISPLAYS:
            update_score_displays();
                break;
    }

}

void Pong::render(){
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(renderer);

    player_score_display->render(renderer);
    opponent_score_display->render(renderer);

    player_paddle->render(renderer);
    opponent_paddle->render(renderer);

    ball->render(renderer);
        
    SDL_RenderPresent(renderer);
}

Pong::Pong(){

    srand(time(NULL));

    if(!initalize_SDL_elements()){
        printf("Failed to initialzie all SDL elements!\n");
        exit(EXIT_FAILURE);
    }

    player_paddle = new Paddle({
        .x = (float)Paddle::WIDTH,
        .y = ((float)screen_spacials.h)/2 - ((float)Paddle::HEIGHT)/2
    });

    opponent_paddle = new Paddle({
        .x = (float)screen_spacials.w - 2*Paddle::WIDTH,
        .y = ((float)screen_spacials.h)/2 - ((float)Paddle::HEIGHT)/2
    });

    ball = new Ball({
        .x = ((float)screen_spacials.w)/2 - ((float)Ball::BREADTH)/2,
        .y = ((float)screen_spacials.h)/2 - ((float)Ball::BREADTH)/2
    });

    current_game_state = Game_States::SETTING_UP_RALLY;

    score = {
        .player = 0,
        .opponent = 0
    };

    player_score_display = new Score_Display(
        renderer,
        score_font,
        SCORE_FONT_COLOR,
        std::to_string(score.player),
        {.x=0, .y=0}
    );
    player_score_display->set_position({
        .x = screen_spacials.w/4 - player_score_display->get_dimensions().width/2,
        .y = screen_spacials.h/2 - player_score_display->get_dimensions().height/2
    });

    opponent_score_display = new Score_Display(
        renderer,
        score_font,
        SCORE_FONT_COLOR,
        std::to_string(score.opponent),
        {.x=0, .y=0}
    );
    opponent_score_display->set_position({
        .x = screen_spacials.w*3/4 - opponent_score_display->get_dimensions().width/2,
        .y = screen_spacials.h/2 - opponent_score_display->get_dimensions().height/2
    });

}

Pong::~Pong(){

    delete opponent_score_display;
    opponent_score_display = nullptr;

    delete player_score_display;
    player_score_display = nullptr;

    delete ball;
    ball = nullptr;

    delete opponent_paddle;
    opponent_paddle = nullptr;

    delete player_paddle;
    player_paddle = nullptr;

    for(uint i = 0; i < collision_sound_effects.size(); i++){
        if(collision_sound_effects[i] != nullptr){
            Mix_FreeChunk(collision_sound_effects[i]);
            collision_sound_effects[i] = nullptr;
        }
    }
    collision_sound_effects.clear();

    TTF_CloseFont(score_font);
    score_font = nullptr;
    
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

void Pong::play(){

    bool should_quit = false;
    SDL_Event* quit_event = nullptr;

    Uint32 previous_tick = SDL_GetTicks();

    Uint32 target_ticks_per_update = 16;
    Uint32 tick_accumulator = 0;

    bool has_updated = false;
    
    while(!should_quit){
        tick_accumulator += SDL_GetTicks() - previous_tick;
        previous_tick = SDL_GetTicks();
        has_updated = false;

        SDL_PumpEvents();

        while(tick_accumulator >= target_ticks_per_update){
            step(0.001f * target_ticks_per_update);
            tick_accumulator -= target_ticks_per_update;
            has_updated = true;
        }

        if(has_updated){
            render();
        }

        if(SDL_PeepEvents(quit_event, 1, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) == 1){
            should_quit = true;
        }
        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    }
}