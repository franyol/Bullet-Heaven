#include <vector>
#include <map>
#include <string>
#include "lambda_GameObject.h"

#include <iostream>

#ifndef _LAMBDA_ENGINE_FSM_H_
#define _LAMBDA_ENGINE_FSM_H_

    class LE_GameState
    {
        private:
            std::map<std::string, LE_GameObject*> gameObjects;

        public:

            LE_GameState () {}
            virtual ~LE_GameState () { clean(); }

            virtual void on_enter () = 0; // Load game objects
            virtual void on_exit () { clean(); }

            void addObject ( LE_GameObject* newObject, std::string objId ) {
                gameObjects[objId] = newObject;
            }
            void popObject ( std::string objId ) {
                auto it = gameObjects.find(objId);
                if (it != gameObjects.end()) {
                    delete it->second;
                    gameObjects.erase(it);
                }
            }

            virtual void update ();
            virtual void render ();
            virtual void clean () { 
                for ( auto it = gameObjects.begin(); it != gameObjects.end(); it++ )
                    delete it->second;
                gameObjects.clear(); 
            }
    };

    class LE_StateMachine
    {
        private:
            std::vector<LE_GameState*> statePool;

            // Singleton
            static LE_StateMachine* the_instance;

            LE_StateMachine () {};
        public:

            ~LE_StateMachine () { clean(); }

            static LE_StateMachine* Instance ( void ) {
                if ( the_instance == 0 ) {
                    the_instance = new LE_StateMachine();
                }
                return the_instance;
            }


            void push_back ( LE_GameState* newState );
            void pop_back  ();

            void update ();
            void render ();
            void clean ();
    };

#endif
