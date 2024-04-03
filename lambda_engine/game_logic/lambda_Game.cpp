#include "lambda_Game.h"
#include "input_handler/lambda_InputHandler.h"
#include "lambda_FSM.h"
#include <iostream>

#define FPS 30
#define DELTA_T 1000.0f / FPS

using namespace std;

LE_Game* LE_Game::the_instance;

void LE_Game::handleEvents () {
    LE_InputHandler::Instance()->update();
}

void LE_Game::update () {
    LE_StateMachine::Instance()->update();
}

void LE_Game::render () {
    for ( Uint32 windowId : windows ) {
        LE_TEXTURE->fillBackground( windowId, 255, 255, 255, 255 );
    }

    LE_StateMachine::Instance()->render();

    for ( Uint32 windowId : windows ) {
        LE_TEXTURE->present( windowId );
    }
}

void LE_Game::mainLoop () {

    if (!LE_TEXTURE->EverythingWasInit()) {
        std::cerr << "Could not init texture manager" << std::endl;
        return;
    }

    Uint32 frameStart, frameTime;

    running = true;
    while ( running ) {
        frameStart = SDL_GetTicks();

        handleEvents();
        update();
        render();

        // Fix framerate
        frameTime = SDL_GetTicks() - frameStart;
        if ( frameTime < DELTA_T ) {
            SDL_Delay( (int)(DELTA_T - frameTime) );
        }
    }
    exit();
}

void LE_Game::clean () {
    LE_TEXTURE->clean();
    LE_InputHandler::Instance()->clean();
    delete the_instance;
}

void LE_Game::exit () {
    cout << "Cleaning up" << endl;
    clean();
}
