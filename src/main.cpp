#include "lambda.h"

class Alien : public LE_GameObject 
{
    private:
        int frameCount;
        std::string frame;

    public:
        Alien( std::string s, int a, int b ): LE_GameObject(s, a, b) {
            setup();
        }

        void setup () {
            frameCount = 0;
            frame = "f1";
            setFrame("f1");
            setSizeMult(5);
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
        }
};

class MyState: public LE_GameState 
{
    public:
        MyState () {}

        void on_enter () {
            LE_GameObject *myAlien;
            LE_SpriteFrame *alien_frame1, *alien_frame2;

            myAlien = new Alien( "assets/tilemap-characters.png", 300, 200 );
            alien_frame1 = new LE_SpriteFrame ( "f1", 2, 0, 24, 20 );
            alien_frame2 = new LE_SpriteFrame ( "f2", 27, 0, 24, 20 );

            myAlien->addFrame ( alien_frame1, "f1" );
            myAlien->addFrame ( alien_frame2, "f2" );
            
            addObject ( myAlien, "myAlien" );
        }
};

int main (int argc, char *argv[]) {

    LE_Game::Instance()->init("Test", 480, 640, false, true);

    LE_StateMachine::Instance()->push_back(new MyState());

    LE_Game::Instance()->mainLoop();

    delete LE_Game::Instance();
    return 0;
}
