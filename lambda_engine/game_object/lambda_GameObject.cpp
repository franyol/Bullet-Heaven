#include "lambda_GameObject.h"
#include "game_logic/lambda_Game.h"
#include <SDL2/SDL_image.h>
#include <iostream>

void LE_SpriteFrame::query ( std::string* id, int* x, int* y, int* h, int* w) {
    if ( id != nullptr ) *id = m_id;
    if ( x != nullptr ) *x = m_x;
    if ( y != nullptr ) *y = m_y;
    if ( h != nullptr ) *h = m_h;
    if ( w != nullptr ) *w = m_w;
}

void LE_GameObject::query ( std::string* curFrame, int* x, int* y ) {
    if ( curFrame != nullptr ) *curFrame = currentFrame;
    if ( x != nullptr ) *x = posx;
    if ( y != nullptr ) *y = posy;
}

void LE_GameObject::loadSpriteSheet ( std::string spriteSheet ) {
    SDL_Surface* tempSurface = IMG_Load(spriteSheet.c_str());
    if ( tempSurface == nullptr ) {
        std::cerr << "Error loading image " << spriteSheet << ": "
                  << IMG_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface (
            LE_Game::Instance()->get_sdl_renderer(),
            tempSurface);
    if ( texture == nullptr ) {
        std::cerr << "Error creating texture from " << spriteSheet << ": "
                  << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface( tempSurface );
}

LE_GameObject::LE_GameObject ( std::string spriteSheet, int posx, int posy ) {
    loadSpriteSheet (spriteSheet);
    setpos (posx, posy);
    setFlip (false, false);
    setSizeMult(1.0);
    setup();
}

void LE_GameObject::render() {

    SDL_Rect src, dst;

    auto it = sprites.find (currentFrame);
    if (it == sprites.end()) {
        std::cerr << "Current sprite is invalid" << std::endl;
        return;
    }
    sprites[currentFrame]->query( nullptr, &src.x, &src.y, &src.h, &src.w );
    dst.x = posx;
    dst.y = posy;
    dst.h = (src.h * sizeMult);
    dst.w = (src.w * sizeMult);

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if ( flip_vertical )
        flip = SDL_FLIP_VERTICAL;
    else if ( flip_horizontal )
        flip = SDL_FLIP_HORIZONTAL;

    if ( ( SDL_RenderCopyEx(
                    LE_Game::Instance()->get_sdl_renderer(), texture,
                    &src, &dst, 0, NULL, flip) ) < 0 ) {
        std::cerr << "Error rendering texture: " << SDL_GetError() << std::endl;
    }
}
