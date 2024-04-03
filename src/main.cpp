#include "visual/lambda_TileMap.h"
#include "sdl_wrappers/lambda_TextureManager.h"

#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

int main () {
    if (!LE_TEXTURE->EverythingWasInit()) {
        cerr << "Could not init texture manager" << endl;
        return 1;
    }

    Uint32 mainWindow = LE_TEXTURE->addWindow( "Test", 640, 480 );

    LE_TILEMAP->loadFromXmlFile ( "assets/tilemaps/test.xml", mainWindow );
    LE_TILEMAP->blendToTexture( "day-background", "blendedBG" );

    LE_TILEMAP->drawMap ( "day-background" );

    LE_TEXTURE->addTile ( mainWindow, "blendedBG", "BGTile"  );
    LE_TEXTURE->draw ( mainWindow, "BGTile", 300, 300 );

    LE_TEXTURE->present( mainWindow );
    SDL_Delay (5000);

    return 0;
}
