#include "lambda.h"

#define BSCALE 5

class Background : public LE_GameObject
{
    private:
        typedef struct Tile {
            std::string frameId;
            int x;
            int y;
        } Tile;

        std::vector<Tile> tiles;
    public:
        Background( std::string s, int a, int b ): LE_GameObject(s, a, b) {
        }

        void addTile ( int x, int y, std::string frameId ) {
            Tile temp;

            temp.x = x;
            temp.y = y;
            temp.frameId = frameId;

            tiles.push_back( temp );
        }

        void render () {

            SDL_Rect src, dst;

            for ( Tile tile : tiles ) {
                auto it = sprites.find ( tile.frameId );
                if ( it == sprites.end() ) 
                    continue;

                sprites[tile.frameId]->query( nullptr, &src.x, &src.y, &src.h, &src.w );
                dst.x = tile.x;
                dst.y = tile.y;
                dst.h = src.h * sizeMult;
                dst.w = src.w * sizeMult;

                SDL_RenderCopyEx (
                        LE_Game::Instance()->get_sdl_renderer(), texture,
                        &src, &dst, 0, NULL, SDL_FLIP_NONE
                        );
            }
        }

        void clean () {
            LE_GameObject::clean();
            tiles.clear();
        }
};


class Alien : public LE_GameObject 
{
    private:
        int frameCount;
        std::string frame;

        int xspeed, yspeed;

    public:
        Alien( std::string s, int a, int b ): LE_GameObject(s, a, b) {
            setup();
        }

        void setup () {
            frameCount = 0;
            frame = "f1";
            setFrame("f1");
            setSizeMult(2);
            xspeed = yspeed = 0;
        }

        void update () {
            frameCount = (frameCount+1)%10;  // Change animation-frame every 10 frames

            if ( frameCount == 9 ) {
                if (frame == "f1") {
                    frame = "f2";
                } else {
                    frame = "f1";
                }
                setFrame ( frame );
            }

            if ( LE_InputHandler::Instance()->getKeyState( SDLK_UP ) == keyState::pressed )
                yspeed = -2;
            else if ( LE_InputHandler::Instance()->getKeyState( SDLK_DOWN ) == keyState::pressed )
                yspeed = 2;
            else yspeed = 0;

            if ( LE_InputHandler::Instance()->getKeyState( SDLK_LEFT ) == keyState::pressed )
            {
                setFlip (false, false);
                xspeed = -2;
            }
            else if ( LE_InputHandler::Instance()->getKeyState( SDLK_RIGHT ) == keyState::pressed )
            {
                setFlip (false, true);
                xspeed = 2;
            }
            else xspeed = 0;

            static int x, y;
            static bool first = true;

            if ( first ) {
                query ( nullptr, &x, &y );
                first = false;
            }
            x += xspeed;
            y += yspeed;
            setpos ( x, y );

        }
};

class MyState: public LE_GameState 
{
    public:
        MyState () {}

        void on_enter () {
            LE_GameObject *background;

            LE_GameObject *myAlien;
            LE_SpriteFrame *alien_frame1, *alien_frame2;

            myAlien = new Alien( "assets/tilemap-characters.png", 300, 200 );
            alien_frame1 = new LE_SpriteFrame ( "f1", 2, 0, 24, 20 );
            alien_frame2 = new LE_SpriteFrame ( "f2", 27, 0, 24, 20 );

            myAlien->addFrame ( alien_frame1, "f1" );
            myAlien->addFrame ( alien_frame2, "f2" );
            
            addObject ( myAlien, "myAlien" );

            background = new Background ( "assets/tilemap-backgrounds.png", 0, 0 );
            LE_SpriteFrame *sunset1, *sunset2, *sunset3, *sunset4;

            sunset1 = new LE_SpriteFrame ( "sunset1", 100, 0, 24, 24 );
            sunset2 = new LE_SpriteFrame ( "sunset2", 100, 25, 24, 24 );
            sunset3 = new LE_SpriteFrame ( "sunset3", 125, 25, 24, 24 );
            sunset4 = new LE_SpriteFrame ( "sunset4", 100, 50, 24, 24 );

            background->addFrame ( sunset1, "sunset1" );
            background->addFrame ( sunset2, "sunset2" );
            background->addFrame ( sunset3, "sunset3" );
            background->addFrame ( sunset4, "sunset4" );

            Background* b = dynamic_cast<Background*>(background);
            for ( int i = 0; i < 640; i+=24*BSCALE ) {
                b->addTile ( i, 0, "sunset1" );
                b->addTile ( i, 24*BSCALE*2, "sunset4" );
                b->addTile ( i, 24*BSCALE*3, "sunset4" );
            }
            for ( int i = 0; i < 640; i+=48*BSCALE ) {
                    b->addTile ( i, 24*BSCALE, "sunset2" );
                    b->addTile ( i+24*BSCALE, 24*BSCALE, "sunset3" );
            }

            background->setSizeMult(BSCALE);
            addObject ( background, "background" );
        }
};

int main (int argc, char *argv[]) {
    LE_Game::Instance()->init("Test", 480, 640, false, true);

    LE_StateMachine::Instance()->push_back(new MyState());

    LE_Game::Instance()->mainLoop();

    delete LE_Game::Instance();
    return 0;
}
