#include "lambda.h"
#include <iostream>
#include <random>
#include <cmath>
#include <cstring>

using namespace std;

#define SCREEN_H 720
#define SCREEN_W 1080

Uint32 mainWindow;
int windowHeight, windowWidth;

class Background: public LE_GameObject
{
    public:
        Background () { setup(); }

        void setup () {
            LE_GameObject::setup();
            frames["day"] = { "bgTile", mainWindow };
            currentFrame = "day";
            h = SCREEN_H;
            w = SCREEN_W;
            scale = false;
        }
};

class Player: public LE_GameObject
{
    private:
        int speed;
        int compspeed;

        int x_speed;
        int y_speed;

        bool moving;
        int animFPS;

        double animTime;
        double dt;

    public: 
        Player () { setup(); }

        void setup () {
            LE_GameObject::setup();
            x = 300;
            y = 180;
            scale = true;
            h = w = 2;
            x_speed = y_speed = 0;
            speed = 170;
            compspeed = sqrt(2)*speed;
            moving = false;

            frames["stand"] = { "alien-stand", mainWindow };
            frames["step"] = { "alien-step", mainWindow };
            currentFrame = "stand";

            animFPS = 12;
            animTime = 0;
        }

        void update () {
            // Move with user input
            if (LE_INPUT->getKeyState( SDLK_UP ) == keyState::pressed)
                y_speed = -speed;
            else if (LE_INPUT->getKeyState( SDLK_DOWN ) == keyState::pressed)
                y_speed = speed;
            else y_speed = 0;

            if (LE_INPUT->getKeyState( SDLK_LEFT ) == keyState::pressed) {
                x_speed = -speed;
                fliph = false;
            }
            else if (LE_INPUT->getKeyState( SDLK_RIGHT ) == keyState::pressed) {
                x_speed = speed;
                fliph = true;
            }
            else x_speed = 0;

            moving = ( x_speed != 0 || y_speed != 0 );
            if ( x_speed != 0 && y_speed != 0 ) {
                x_speed *= compspeed/speed;
                y_speed *= compspeed/speed;
            }

            dt = LE_GAME->getDeltaTime ();
            //std::cout << 1000/dt << std::endl;

            if ( moving ) {
                // step animation
                animTime += dt;
                if ( animTime > 1000.0f/animFPS ) {
                    if ( currentFrame == "stand" ) {
                        currentFrame = "step";
                    } else {
                        currentFrame = "stand";
                    }
                    animTime = 0;
                }
            } else {
                // keep on stand frame when not moving
                currentFrame = "stand";
                animTime = 0;
            }

            x += (x_speed * dt/1000);
            y += (y_speed * dt/1000);

            if ( x > SCREEN_W - 20*w ) x = SCREEN_W - 20*w;
            else if ( x < 0 ) x = 0;
            if ( y > SCREEN_H - 24*h ) y = SCREEN_H - 24*h;
            else if ( y < 0 ) y = 0;
        }
};

class SimpleGame: public LE_GameState
{
    public:
        SimpleGame () {}

        void on_enter () {
            // Load assets and game objects

            // Load Background
            LE_TILEMAP->loadFromXmlFile ( "assets/tilemaps/test.xml", mainWindow );

            // Blend background to a single tile and scalate to window size
            LE_TILEMAP->blendToTexture ( "day-background", "bgTexture" );
            LE_TEXTURE->addTile ( mainWindow, "bgTexture", "bgTile" );

            // Spawn the player
            addObject ( new Background(), "BackgroundInstance"  );
            addObject ( new Player(), "playerInstance" );
        }
        
        void update () {
            LE_GameState::update();
            
            static char ninstances = 'a';
            char pin[] = "pin";
            static char nstring[2] = "x";
            static bool released = true;
 
            if (LE_INPUT->getKeyState( SDLK_ESCAPE ) == keyState::pressed) { LE_GAME->exit(); }
            if ( released ) {
                if (LE_INPUT->getKeyState( SDLK_SPACE ) == keyState::pressed) {
                    nstring[0] = ninstances++;
                    addObject ( new Player(), strcat(pin, nstring) );
                    released = false;
                }
                if (LE_INPUT->getKeyState( SDLK_BACKSPACE ) == keyState::pressed) {
                    nstring[0] = --ninstances;
                    popObject ( strcat( pin, nstring ) );
                    released = false;
                }
            }
            if (LE_INPUT->getKeyState( SDLK_ESCAPE ) == keyState::pressed) {
                released = true;
            }
        }

        void render () {
            // Scale main view to screen size
            LE_TEXTURE->setRenderTarget ( mainWindow, "mainView" );

            LE_GameState::render (); // Normal rendering

            LE_TEXTURE->restoreRenderTarget ( mainWindow );
            LE_TEXTURE->draw ( mainWindow, "mainViewTile", 0, 0, 
                    windowHeight, windowWidth, false );
        }
};

int main () {
    if ( !LE_Init() ) {
        cerr << "Could not initialize Lambda Engine" << endl;
    }

    mainWindow = LE_GAME->createWindow ( "my game", SCREEN_W, SCREEN_H, true );
    LE_TEXTURE->createTargetTexture ( mainWindow, "mainView", SCREEN_H, SCREEN_W );
    LE_TEXTURE->addTile ( mainWindow, "mainView", "mainViewTile" );
    LE_TEXTURE->getWindowSize ( mainWindow, &windowWidth, &windowHeight );

    LE_GAME->fixFramerate ( 60 );
    LE_FSM->push_back ( new SimpleGame() );

    LE_GAME->mainLoop();

    SDL_Delay (400);
    LE_Quit();
    return 0;
}
