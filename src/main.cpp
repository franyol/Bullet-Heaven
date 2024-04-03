#include "lambda.h"
#include <iostream>

using namespace std;

int main () {
    if ( !LE_Init() ) {
        cerr << "Could not initialize Lambda Engine" << endl;
    }

    LE_Quit();
    return 0;
}
