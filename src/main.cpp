#include "lambda.h"
#include <iostream>
#include <random>
#include <cmath>
#include <string>

using namespace std;

#define SCREEN_H 720
#define SCREEN_W 1080

Uint32 mainWindow;
int windowHeight, windowWidth;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dis(0, 100);

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

class Bullet: public LE_GameObject
{
    friend class SimpleGame;

    protected:
        int speed;
        int y_speed;
        int x_speed;
        double s_angle;

        int b_h;
        int b_w;

        int angSpeed;

        int lifeTime;
        int maxLifeTime;
        
        double dt;

    public:
        Bullet () { setup(); }

        void setup () {
            LE_GameObject::setup();
            int sign;
            if ( dis(gen) > 50 ) {
                sign = 1;
            } else {
                sign = -1;
            }
            x = SCREEN_W/2 + (dis(gen)*SCREEN_W/400 + SCREEN_W/4) * sign;
            if ( dis(gen) > 50 ) {
                sign = 1;
            } else {
                sign = -1;
            }
            y = SCREEN_H/2 + (dis(gen)*SCREEN_H/400 + SCREEN_H/4) * sign;
            speed = 100 + dis(gen) * 3;
            s_angle = dis(gen) * 2 * M_PI / 100;
            x_speed = speed * cos ( s_angle );
            y_speed = speed * sin ( s_angle );
            angSpeed = 10 * dis(gen) - 500;

            lifeTime = 0;
            maxLifeTime = 2500 + 200 * dis(gen) ;

            dt = 0;

            if ( dis(gen) > 50 ) {
                frames["s"] = { "bullet-small", mainWindow };
                b_h = b_w = 10;
            } else {
                frames["s"] = { "bullet", mainWindow };
                b_h = b_w = 14;
            }
            currentFrame = "s";
            h = w = 2;
        }

        void update () {
            dt = LE_GAME->getDeltaTime();
            lifeTime += dt;

            x += x_speed*dt/1000;
            y += y_speed*dt/1000; 

            angle += angSpeed*dt/1000;

            // Border limits
            if ( x > SCREEN_W - b_w ) { 
                x = SCREEN_W - b_w;
                x_speed *= -1;
            }
            else if ( x < 0 ) {
                x = 0;
                x_speed *= -1;
            }
            if ( y > SCREEN_H - b_h ) { 
                y = SCREEN_H - b_h ;
                y_speed *= -1;
            }
            else if ( y < 0 ) {
                y = 0;
                y_speed *= -1;
            }

            // Object Destroy condition
            if ( lifeTime > maxLifeTime ) destroy_me = true;
        }
};

class Player: public LE_GameObject
{
    friend class SimpleGame;

    private:
        int speed;

        int x_speed;
        int y_speed;
        int speed_comp;

        bool moving;
        int animFPS;

        double animTime;
        double dt;

        Uint32 dashTime;
        Uint32 dashCoolDown;
        Uint32 timeSinceLastDash;
        int dashSpeed;
        int dashSpeed_comp;
        bool dashEnable;
        bool dashing;

        int textureCount;
        int textureMax;

    public: 
        Player () { setup(); }

        void setup () {
            LE_GameObject::setup();
            x = 300;
            y = 180;
            scale = true;
            h = w = 2;
            x_speed = y_speed = 0;
            speed = 200;
            speed_comp = sqrt(2*speed*speed);
            moving = false;

            frames["stand"] = { "alien-stand0", mainWindow };
            frames["step"] = { "alien-step0", mainWindow };
            currentFrame = "stand";

            textureCount = 0;
            textureMax = 5;

            animFPS = 12;
            animTime = 0;

            dashCoolDown = 500;
            timeSinceLastDash = dashCoolDown;
            dashTime = 100;
            dashSpeed = 1100;
            dashSpeed_comp = sqrt(2*dashSpeed*dashSpeed);
            dashEnable = true;
            dashing = false;
        }

        void update () {
            // Move with user input
            if (LE_INPUT->getKeyState( SDLK_w ) == keyState::pressed)
                y_speed = -speed;
            else if (LE_INPUT->getKeyState( SDLK_s ) == keyState::pressed)
                y_speed = speed;
            else y_speed = 0;

            if (LE_INPUT->getKeyState( SDLK_a ) == keyState::pressed) {
                x_speed = -speed;
                fliph = false;
            }
            else if (LE_INPUT->getKeyState( SDLK_d ) == keyState::pressed) {
                x_speed = speed;
                fliph = true;
            }
            else x_speed = 0;

            moving = ( x_speed != 0 || y_speed != 0 );
            dt = LE_GAME->getDeltaTime ();

            if ( moving ) {

                if ( dashEnable && LE_INPUT->getKeyState( SDLK_SPACE ) == keyState::pressed ){
                    dashing = true;
                    timeSinceLastDash = 0;

                    textureCount = (textureCount+1)%textureMax;
                    frames["stand"] = {"alien-stand" + to_string(textureCount), mainWindow};
                    frames["step"] = {"alien-step" + to_string(textureCount), mainWindow};

                }

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

            if ( dashing ) {
                x_speed *= dashSpeed/speed;
                y_speed *= dashSpeed/speed;
                if ( dashTime < timeSinceLastDash ) {
                    dashing = false;
                }
            }

            if ( x_speed != 0 && y_speed != 0 ) {
                if ( dashing ) {
                    x_speed *= dashSpeed_comp/dashSpeed;
                    y_speed *= dashSpeed_comp/dashSpeed;
                } else {
                    x_speed *= speed_comp/speed;
                    y_speed *= speed_comp/speed;
                }
            }

            dashEnable = ( timeSinceLastDash > dashCoolDown );
            if ( !dashEnable ) timeSinceLastDash += dt;

            x += (x_speed * dt/1000);
            y += (y_speed * dt/1000);


            // Border limits
            if ( x > SCREEN_W - 20*w ) x = SCREEN_W - 20*w;
            else if ( x < 0 ) x = 0;
            if ( y > SCREEN_H - 24*h ) y = SCREEN_H - 24*h;
            else if ( y < 0 ) y = 0;
        }
};

class SimpleGame: public LE_GameState
{
    private:
        double dt;

        int bulletFreq;
        double bulletTime;

        int bulletCount;
        int bulletMax;

        Player* player = dynamic_cast<Player*>(getObject ( "playerInstance" ));
        Bullet* currentBullet;

    public:
        SimpleGame () {}

        void on_enter () {

            bulletFreq = 100;
            bulletTime = 0;
            bulletCount = 1;
            bulletMax = 100;

            // Load assets and game objects

            // Load Background
            LE_TILEMAP->loadFromXmlFile ( "assets/tilemaps/test.xml", mainWindow );

            // Blend background to a single tile and scalate to window size
            LE_TILEMAP->blendToTexture ( "day-background", "bgTexture" );
            LE_TEXTURE->addTile ( mainWindow, "bgTexture", "bgTile" );

            // Spawn the player
            player = new Player();
            addObject ( new Background(), "BackgroundInstance"  );
            addObject ( player, "playerInstance" );
            addObject ( new Bullet(), "cfirstBullet" );
        }
        
        void update () {
            LE_GameState::update();

            bulletTime += LE_GAME->getDeltaTime();

            if ( bulletTime > 100000/bulletFreq ) {
                addObject ( new Bullet, "cBullet" + to_string(bulletCount++)  );
                bulletTime = 0;
            }

            if (LE_INPUT->getKeyState( SDLK_ESCAPE ) == keyState::pressed)
                LE_GAME->exit();

            // Check hitboxes
            for ( auto it = gameObjects.begin(); it != gameObjects.end(); it++ ) {
                if ( it->first.find( "Bullet" ) != string::npos ) {
                    // Check hitbox of a single bullet
                    
                    currentBullet = dynamic_cast<Bullet*>(it->second);

                    if ( currentBullet->x < player->x + 20
                         && currentBullet->x + currentBullet->b_w > player->x
                         && currentBullet->y < player->y + 24
                         && currentBullet->y + currentBullet->b_h > player->y ) {
                        // COLLISION
                        
                        clean(); // Clear all objects
                        on_enter(); // Over again
                        break;
                    }
                }
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

    mainWindow = LE_GAME->createWindow ( "my game", SCREEN_W, SCREEN_H );
    LE_TEXTURE->createTargetTexture ( mainWindow, "mainView", SCREEN_H, SCREEN_W );
    LE_TEXTURE->addTile ( mainWindow, "mainView", "mainViewTile" );
    LE_TEXTURE->getWindowSize ( mainWindow, &windowHeight, &windowWidth );

    LE_GAME->fixFramerate ( 60 );
    LE_FSM->push_back ( new SimpleGame() );

    LE_GAME->mainLoop();

    LE_Quit();
    return 0;
}
