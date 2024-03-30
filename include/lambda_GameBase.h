#include <iostream>
#include <SDL2/SDL.h>

#ifndef _LAMBDA_ENGINE_GAME_BASE_H_
#define _LAMBDA_ENGINE_GAME_BASE_H_

    class LE_GameBase
    {
        private:
            SDL_Window*   sdl_Window;
            SDL_Renderer* sdl_Renderer;

            // The window may be resizable, but these will save the original desired
            // window dimentions
            int window_height;
            int window_width;

            // Tells if the game onject has been initialized
            bool initialized;

        public:

            // Constructor & Destructor
            LE_GameBase () { 
                initialized = false; 
                sdl_Window = NULL; 
                sdl_Renderer = NULL; 
            }
            virtual ~LE_GameBase ();

            // Getter methods
            bool          was_init         ( void ) const { return initialized; }
            SDL_Window*   get_sdl_window   ( void ) const { return sdl_Window; }
            SDL_Renderer* get_sdl_renderer ( void ) const { return sdl_Renderer; }
            int           get_width        ( void ) const { return window_width; }
            int           get_height       ( void ) const { return window_height; }

            // SDL Window and renderer initialization
            virtual bool init ( const char *title, int window_height, int window_width, 
                           bool full_screen = false, bool resizable = false );

            // Game loop mehtods
            virtual void handleEvents () = 0;
            virtual void update () = 0;
            virtual void render () = 0;
    };

#endif
