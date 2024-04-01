#include "lambda_GameBase.h" // Already includes SDL

#ifndef _LAMBDA_ENGINE_GAME_H_
#define _LAMBDA_ENGINE_GAME_H_

    class LE_Game : public LE_GameBase
    {
        private:
            bool is_running;

            // Singleton
            static LE_Game* the_instance;
            LE_Game (): LE_GameBase() {}

        public:
            // Singleton
            static LE_Game* Instance() {
                if ( the_instance == 0 ) {
                    the_instance = new LE_Game();
                } 
                return the_instance;
            }

            bool running ( void ) { return is_running; }
            void set_running ( bool state ) { is_running = state; }

            virtual void handleEvents ();
            virtual void update ();
            virtual void render ();
            virtual void clean  ();
            virtual void exit   ();

            virtual void mainLoop ();
    };

#endif
