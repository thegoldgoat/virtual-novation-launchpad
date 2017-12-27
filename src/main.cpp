#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

#include "application.h"

int main() {
    app = new Application;
    app->runApp(); 
    return 0;
}