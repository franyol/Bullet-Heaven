#include "lambda.h"
#include <iostream>

using namespace std;

Uint32 mainWindow;

class Background: public LE_GameObject
{
    public:
        Background () { setup(); }

        void setup () {
            LE_GameObject::setup();
            frames["day"] = { "bgTile", mainWindow };
            currentFrame = "day";
            h = 480;
            w = 640;
            scale = false;
        }
};

class Player: public LE_GameObject
{
    private:
        int speed;

        int x_speed;
        int y_speed;

        bool moving;
        int animFPS;

    public: 
        Player () { setup(); }

        void setup () {
            cout << "Running setup" << endl;
            LE_GameObject::setup();
            x = 300;
            y = 180;
            scale = true;
            h = w = 3;
            x_speed = y_speed = 0;
            speed = 170;
            moving = false;

            frames["stand"] = { "alien-stand", mainWindow };
            frames["step"] = { "alien-step", mainWindow };
            currentFrame = "stand";

            animFPS = 12;
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

            static double animTime = 0;
            static double dt; 
            dt = LE_GAME->getDeltaTime ();

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

            // Load player textures
            LE_TEXTURE->loadTexture ( 
                    mainWindow, 
                    "assets/tilemap-characters.png", 
                    "characterTexture" );
            LE_TEXTURE->addTile ( mainWindow, "characterTexture", 
                                    "alien-stand" , 2, 0, 24, 20 );
            LE_TEXTURE->addTile ( mainWindow, "characterTexture", 
                                    "alien-step" , 27, 0, 24, 20 );

            // Spawn the player
            addObject ( new Background(), "BackgroundInstance"  );
            addObject ( new Player(), "playerInstance" );
        }
};

int main () {
    if ( !LE_Init() ) {
        cerr << "Could not initialize Lambda Engine" << endl;
    }

    mainWindow = LE_GAME->createWindow ( "my game", 640, 480 );
    LE_GAME->fixFramerate ( 60 );
    LE_FSM->push_back ( new SimpleGame() );

    LE_GAME->mainLoop();

    LE_Quit();
    return 0;
}
