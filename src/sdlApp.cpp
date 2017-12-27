#include "sdlApp.h"
#include <SDL2/SDL.h>
#include <iostream>

// Costruttore dell'applicazione
SDLApp::SDLApp(std::string titolo, int _w, int _h, int _fps) : fps(_fps){
  window =
      SDL_CreateWindow(titolo.c_str(), SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, _w, _h, SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!window || !renderer) {
    std::cout << "Errore in inizializzazione di window o renderer: "
              << SDL_GetError() << std::endl;
    close(-1);
  }
  this->continua = true;
}

// Application's core: return the value sent with close()
int SDLApp::runApp() {
  Uint32 tempoPrecedente;
  while (continua) {
    tempoPrecedente = SDL_GetTicks();
    SDL_GetWindowSize(this->window, &w, &h);
    draw();
    events();
    SDL_RenderPresent(this->renderer);
    if (SDL_GetTicks() - tempoPrecedente < floor(1000 / fps)) {
      SDL_Delay(floor(1000 / this->fps - (SDL_GetTicks() - tempoPrecedente)));
    }
  }
  return ritorno;
}

// Close the application with _ritorno value.
void SDLApp::close(int _ritorno) {
  ritorno = _ritorno;
  continua = false;
  SDL_DestroyRenderer(this->renderer);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}
