#include "lambda_GameBase.h"
#include <iostream>

using namespace std;

bool LE_GameBase::init ( const char *title, int window_height, 
                         int window_width, bool full_screen, bool resizable ) {
    
    // SDL init all subsystems
    if ( SDL_InitSubSystem ( SDL_INIT_EVERYTHING ) < 0 ) {
        cerr << "Error initializing SDL: " << SDL_GetError() << endl;
        return false;
    }

    // Initialize game window
    Uint32 flags = SDL_WINDOW_SHOWN;
    if ( full_screen ) {
        flags |= SDL_WINDOW_FULLSCREEN;
    } else if ( resizable ) {
        flags |= SDL_WINDOW_RESIZABLE;
    }

    sdl_Window = SDL_CreateWindow( title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   window_width, window_height, flags );

    if ( sdl_Window == NULL ) {
        cerr << "Error initializing window: " << SDL_GetError() << endl;
        return false;
    }

    // Initialize renderer
    sdl_Renderer = SDL_CreateRenderer ( sdl_Window, -1, SDL_RENDERER_ACCELERATED );

    if ( sdl_Renderer == NULL ) {
        cerr << "Error initializing renderer: " << SDL_GetError() << endl;
        return false;
    }

    initialized = true;
    return true;
}

LE_GameBase::~LE_GameBase () {
    if ( !initialized ) {
        // Nothing to do
        return;
    } else {
        // Cleanup: Destroy all SDL Elements created by the class
        SDL_DestroyRenderer( sdl_Renderer );
        SDL_DestroyWindow( sdl_Window );
        SDL_Quit();
    }
}
