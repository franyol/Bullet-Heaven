#include "lambda.h"

using namespace std;

int main (int argc, char *argv[]) {

    LE_Game::Instance()->init("Test", 480, 640, false, true);
    LE_Game::Instance()->mainLoop();

    delete LE_Game::Instance();
    return 0;
}
