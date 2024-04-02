#include "sdl_wrappers/lambda_TextureManager.h"
#include <iostream>
#include <SDL2/SDL.h>

int main (int argc, char *argv[]) {

    // Init Texture manager
    Uint32 mainWindow, secondaryWindow;
    LE_TEXTURE->init();

    if ( !LE_TEXTURE->EverythingWasInit() ) {
        std::cout << "Could not initialize texture manager " << std::endl;
    }

    mainWindow = LE_TEXTURE->addWindow( "Hello Lambda Engine!", 640, 480 );
    secondaryWindow = LE_TEXTURE->addWindow( "Hello Lambda Engine from another view!", 480, 640 );
    std::cout << "Main window ID is: " << mainWindow << std::endl;
    std::cout << "Secondary window ID is: " << secondaryWindow << std::endl;

    LE_TEXTURE->loadTexture( mainWindow, "assets/tilemap-characters.png", "myTexture" );
    LE_TEXTURE->addTile( mainWindow, "myTexture", "alien_standing", 0, 0, 25, 25 );
    LE_TEXTURE->loadTexture( secondaryWindow, "assets/tilemap-characters.png", "myTexture" );
    LE_TEXTURE->addTile( secondaryWindow, "myTexture", "alien_standing", 25, 0, 25, 25 );

    LE_TEXTURE->fillBackground( mainWindow, 255, 255, 255, 255 );
    LE_TEXTURE->draw( mainWindow, "alien_standing", 100, 100, 3, 3, true );

    LE_TEXTURE->fillBackground( secondaryWindow, 255, 0, 255, 100 );
    LE_TEXTURE->draw( secondaryWindow, "alien_standing", 100, 100, 3, 4, true, true, false, 130 );


    if ( !LE_TEXTURE->present ( mainWindow ) )
        std::cout << "Could not present";
    if ( !LE_TEXTURE->present ( secondaryWindow ) )
        std::cout << "Could not present";

    SDL_Delay ( 5000 );

    LE_TEXTURE->clean();

    return 0;
}
