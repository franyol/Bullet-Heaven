#include "lambda_Game.h"
#include <iostream>
// #include "lambda_FSM.h"

#define FPS 30
#define DELTA_T 1000.0f / FPS

using namespace std;

int red_scale = 0;

LE_Game* LE_Game::the_instance;

void LE_Game::handleEvents () {
    SDL_Event event;
    if ( SDL_PollEvent (&event) )
    {
        // TODO: Event manager class singleton
        switch (event.type)
        {
            case SDL_QUIT:
                is_running = false;
                break;
            default:
                break;
        }
    }
}

void LE_Game::update () {
    // Updating
    // TODO: run fsm updates
    //
    red_scale = (red_scale + 30) % 200;
}

void LE_Game::render () {
    // TODO: run fsm renders
    // A render manager to handle views and layers
    //
    SDL_SetRenderDrawColor(get_sdl_renderer(), red_scale, 0, 0, 100);
    SDL_RenderClear(get_sdl_renderer());
    SDL_RenderPresent(get_sdl_renderer());
}

void LE_Game::mainLoop () {

    if ( !was_init() ) {
        cerr << "Main loop could not start because game object was not initialized." << endl;
        return;
    }

    Uint32 frameStart, frameTime;

    Instance()->set_running(true);
    while ( Instance()->running() ) {
        frameStart = SDL_GetTicks();

        Instance()->handleEvents();
        Instance()->update();
        Instance()->render();

        // Fix framerate
        frameTime = SDL_GetTicks() - frameStart;
        if ( frameTime < DELTA_T ) {
            SDL_Delay( (int)(DELTA_T - frameTime) );
        }
    }
}
