#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <map>

#ifndef _LAMBDA_ENGINE_GAMEOBJ_H_
#define _LAMBDA_ENGINE_GAMEOBJ_H_

    class LE_SpriteFrame
    {
        private:
            std::string m_id;
            int m_x;
            int m_y;
            int m_w;
            int m_h;
        public:
            LE_SpriteFrame ( std::string id, int x, int y, int h, int w ):
                m_id(id), m_x(x), m_y(y), m_w(w), m_h(h) {}
            ~LE_SpriteFrame () {} 
            
            void query ( std::string* id = nullptr, int* x = nullptr, 
                         int* y = nullptr, int* h = nullptr, int* w = nullptr );
    };

    class LE_GameObject
    {
        private:
            SDL_Texture* texture;
            std::string currentFrame;

            std::map<std::string, LE_SpriteFrame*> sprites;

            int posx;
            int posy;
            bool flip_vertical;
            bool flip_horizontal;
            double sizeMult;
        public:

            LE_GameObject ( std::string spriteSheet, int posx, int posy );
            virtual ~LE_GameObject () { clean(); }

            void setFlip (bool v, bool h) { flip_vertical = v; flip_horizontal = h; }
            void setSizeMult (double multipier) { sizeMult = multipier; }
            void addFrame ( LE_SpriteFrame* newFrame, std::string frameId ) {
                sprites[frameId] = newFrame;
            }
            void setFrame ( std::string frameId ) { currentFrame = frameId; }
            void popFrame ( std::string frameId ) {
                auto it = sprites.find(frameId);
                if (it != sprites.end()) {
                    delete it->second;
                    sprites.erase(it);
                }
            }

            virtual void query ( std::string* curFrame = nullptr ,int *x = nullptr, 
                                int *y = nullptr );

            virtual void loadSpriteSheet ( std::string spriteSheet );
            virtual void setpos ( int x, int y ) { posx = x; posy = y; }

            virtual void setup () {}
            virtual void update () {}
            virtual void render ();
            virtual void clean () {
                for ( auto it = sprites.begin(); it != sprites.end(); it++ )
                    delete it->second;
                sprites.clear();
            }
    };

#endif
