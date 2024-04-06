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

class Number: public LE_GameObject
{
    friend class SimpleGame;

    public:
        Number ( int x_, int y_ ) { 
            setup(); 
            x = x_; 
            y = y_;  
        }

        void setup () {
            LE_GameObject::setup();

            string str;
            for ( int i = 0; i < 10; i++ ) {
                str = to_string(i);
                frames[str] = { str, mainWindow };
            }
            currentFrame = "0";
            h = w = 3;
        }
};

class Coin: public LE_GameObject
{
    friend class SimpleGame;

    public:
        Coin () { setup(); }

        void setup () {
            LE_GameObject::setup();
            frames["coin"] = { "coin", mainWindow };
            currentFrame = "coin";
            h = 24;
            w = 24;
            scale = false;
            relocate();
        }

        void relocate () {
            x = SCREEN_W/4 + dis(gen)*SCREEN_W/200;
            y = SCREEN_H/4 + dis(gen)*SCREEN_H/200;
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
                LE_AUDIO->playChunk ( "bounce", 0 );
            }
            else if ( y < 0 ) {
                y = 0;
                y_speed *= -1;
                LE_AUDIO->playChunk ( "bounce", 0 );
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

            dashCoolDown = 700;
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
                    LE_AUDIO->playChunk ( "dash", 0 );
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

        Player* player;
        Coin* coin;
        Bullet* currentBullet;

        Number *cent, *dec, *unit;

        int score;

    public:
        SimpleGame () {}

        void set_score ( int score ) {

            int n_cent, n_dec;
            n_cent = score - (score%100);
            score = score - n_cent;
            n_dec = score - (score%10);
            score = score - n_dec;
            cent->currentFrame = ( to_string ( (n_cent)/100 ) );
            dec->currentFrame = ( to_string ( (n_dec)/10 ) );
            unit->currentFrame = ( to_string ( score ) );
        }

        void on_enter () {

            bulletFreq = 10;
            bulletTime = 0;
            bulletCount = 1;
            bulletMax = 100;
            score = 0;

            // Load assets and game objects
            
            // Load Audio
            static bool once = true;
            if ( once ) {
                LE_AUDIO->loadTrack ( "mainMusic", "assets/mixkit-dreaming-big-31.mp3" );
                LE_AUDIO->loadChunk ( "dash", "assets/mixkit-arcade-retro-jump-223.wav" );
                LE_AUDIO->loadChunk ( "hit","assets/mixkit-arcade-mechanical-bling-210.wav" );
                LE_AUDIO->loadChunk ( "coin", "assets/mixkit-quick-positive.wav" );
                LE_AUDIO->loadChunk ( "bounce", "assets/bounce.wav" );
                once = false;
            }

            // Load Background
            LE_TILEMAP->loadFromXmlFile ( "assets/tilemaps/test.xml", mainWindow );

            // Blend background to a single tile and scalate to window size
            LE_TILEMAP->blendToTexture ( "day-background", "bgTexture" );
            LE_TEXTURE->addTile ( mainWindow, "bgTexture", "bgTile" );

            // Spawn the player
            player = new Player();
            coin = new Coin();

            cent = new Number ( 20, 20 );
            dec = new Number ( 60, 20 );
            unit = new Number ( 100, 20 );

            addObject ( new Background(), "aBackgroundInstance" );
            addObject ( player, "bplayerInstance" );
            addObject ( new Bullet(), "cfirstBullet" );
            addObject ( coin, "bcoin" );

            addObject ( cent, "zcent" );
            addObject ( dec, "zdec" );
            addObject ( unit, "zunit" );

            LE_AUDIO->playTrack ( "mainMusic", -1 );
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
            if ( coin->x < player->x + 40
                 && coin->x + 25 > player->x
                 && coin->y < player->y + 48
                 && coin->y + 25 > player->y ) {
                // COIN COLLISION
                LE_AUDIO->playChunk ( "coin", 0 );
                score = (score + 1) % 1000;
                set_score (score);
                bulletFreq += 1; // It gets harder
                coin->relocate();
            }

            for ( auto it = gameObjects.begin(); it != gameObjects.end(); it++ ) {
                if ( it->first.find( "Bullet" ) != string::npos ) {
                    // Check hitbox of a single bullet
                    
                    currentBullet = dynamic_cast<Bullet*>(it->second);

                    if ( currentBullet->x < player->x + 40
                         && currentBullet->x + currentBullet->b_w > player->x
                         && currentBullet->y < player->y + 48
                         && currentBullet->y + currentBullet->b_h > player->y ) {
                        // COLLISION
                        LE_AUDIO->playChunk ( "hit", 0 );
                        
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
            static int renderHeight, renderWidth;
            static int w_x, w_y;

            if (windowHeight > windowWidth) {
                renderWidth = windowWidth;
                renderHeight = SCREEN_H * windowWidth / SCREEN_W;
                w_x = 0;
                w_y = (windowHeight - renderHeight)/2;
            } else {
                renderHeight = windowHeight;
                renderWidth = SCREEN_W * windowHeight / SCREEN_H;
                w_y = 0;
                w_x = (windowWidth - renderWidth)/2;
            }
            LE_TEXTURE->draw ( mainWindow, "mainViewTile", w_x, w_y, 
                    renderHeight, renderWidth, false );
        }
};

int main () {
    if ( !LE_Init() ) {
        cerr << "Could not initialize Lambda Engine" << endl;
    }

    mainWindow = LE_GAME->createWindow ( "my game", SCREEN_W, SCREEN_H, true );
    LE_TEXTURE->createTargetTexture ( mainWindow, "mainView", SCREEN_H, SCREEN_W );
    LE_TEXTURE->addTile ( mainWindow, "mainView", "mainViewTile" );
    LE_TEXTURE->getWindowSize ( mainWindow, &windowHeight, &windowWidth );

    LE_GAME->fixFramerate ( 60 );
    LE_FSM->push_back ( new SimpleGame() );

    LE_GAME->mainLoop();

    LE_Quit();
    return 0;
}
