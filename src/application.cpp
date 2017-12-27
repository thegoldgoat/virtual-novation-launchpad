#include "application.h"
#include "global.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>
using namespace std;

Application *app;

Application::Application() : SDLApp(WINDOW_TITLE, W, H, FPS) {
  for (int i = 0; i < DIM_LAUNCH; i++) {
    this->attiviSopra[i] = true;
    for (int j = 0; j < DIM_LAUNCH; j++)
      this->attivi[i][j] = false;
  }
}

void Application::draw() {
  SDL_SetRenderDrawColor(this->renderer, 51, 51, 51, 255);
  SDL_RenderClear(this->renderer);
  SDL_Rect temp;
  double offsetx = (double)w / DIM_LAUNCH;
  double offsety = (double)h / DIM_LAUNCH;
  temp.w = offsetx - PADDING_TASTI * 2;
  temp.h = offsety - PADDING_TASTI * 2;
  for (int i = 0; i < DIM_LAUNCH; i++)
    for (int j = 0; j < DIM_LAUNCH; j++) {
      temp.x = i * offsetx + PADDING_TASTI;
      temp.y = j * offsety + PADDING_TASTI;
      if (this->attivi[i][j])
        roundedBoxRGBA(renderer, temp.x, temp.y, temp.x + temp.w,
                       temp.y + temp.h, 10, 255, 51, 51, 255);
      else
        roundedBoxRGBA(renderer, temp.x, temp.y, temp.x + temp.w,
                       temp.y + temp.h, 10, 102, 102, 102, 255);
    }
}

/*
void Application::draw() {
  SDL_SetRenderDrawColor(this->renderer, 51, 51, 51, 255);
  SDL_RenderClear(this->renderer);
  // Calcola il rettangolo della matrice
  double altezzaTasti = (double)h / (DIM_LAUNCH + 1);
  double larghezzaTasti = (double)w / (DIM_LAUNCH + 1);
  matrice.x = 0;
  matrice.y = altezzaTasti;
  matrice.w = larghezzaTasti * 8;
  matrice.h = altezzaTasti * 8;
  // DISEGNA I TASTI SOPRA
  int tempx, tempy;
  for (int i = 0; i < DIM_LAUNCH; i++) {
    if (attiviSopra[i]) {
      tempx = i * larghezzaTasti + PADDING_TASTI;
      tempy = PADDING_TASTI;
      roundedBoxRGBA(
          renderer, tempx, tempy, tempx + larghezzaTasti - PADDING_TASTI * 2,
          tempy + altezzaTasti - PADDING_TASTI * 2, 15, 255, 51, 51, 255);
    } else
      roundedBoxRGBA(
          renderer, tempx, tempy, tempx + larghezzaTasti - PADDING_TASTI * 2,
          tempy + altezzaTasti - PADDING_TASTI * 2, 15, 102, 102, 102, 255);
  }
  // DISEGNA I TASTI A DESTRA
  for (int i = 0; i < DIM_LAUNCH; i++) {
    if (attiviSopra[i]) {
      tempy = (i + 1) * altezzaTasti + PADDING_TASTI;
      tempx = w - larghezzaTasti;
      roundedBoxRGBA(
          renderer, tempx, tempy, tempx + larghezzaTasti - PADDING_TASTI * 2,
          tempy + altezzaTasti - PADDING_TASTI * 2, 15, 255, 51, 51, 255);
    } else
      roundedBoxRGBA(
          renderer, tempx, tempy, tempx + larghezzaTasti - PADDING_TASTI * 2,
          tempy + altezzaTasti - PADDING_TASTI * 2, 15, 102, 102, 102, 255);
  }
  // DISEGNA I TASTI AL CENTRO
  for (int i = 0; i < DIM_LAUNCH; i++)
    for (int j = 0; j < DIM_LAUNCH; j++) {
      tempx = i * larghezzaTasti + PADDING_TASTI;
      tempy = (j + 1) * altezzaTasti + PADDING_TASTI;
      if (attivi[i][j])
        roundedBoxRGBA(
            renderer, tempx, tempy, tempx + larghezzaTasti - PADDING_TASTI * 2,
            tempy + altezzaTasti - PADDING_TASTI * 2, 15, 255, 51, 51, 255);
      else
        roundedBoxRGBA(
            renderer, tempx, tempy, tempx + larghezzaTasti - PADDING_TASTI * 2,
            tempy + altezzaTasti - PADDING_TASTI * 2, 15, 102, 102, 102, 255);
    }
}
*/
void Application::events() {
  SDL_Event ev;
  while (SDL_PollEvent(&ev)) {
    switch (ev.type) {
    case SDL_QUIT:
      this->close(0);
      break;
    case SDL_KEYDOWN:
      // carattere premuto -> ev.key.keysym.sym;
      break;
    case SDL_MOUSEBUTTONUP:
      this->launchpad->send(premutox, premutoy, 0);
      break;
    case SDL_MOUSEBUTTONDOWN:
      // coordinate mouse -> ev.button.x ev.button.y
      int mx = ev.button.x;
      int my = ev.button.y;
      double offsetx = (double)w / DIM_LAUNCH;
      double offsety = (double)h / DIM_LAUNCH;
      int indx = floor(mx / offsetx), indy = floor(my / offsety);
      // cout << "indx -> " << indx << " indy -> " << indy << endl;
      this->launchpad->send(indx, indy, 127);
      premutox = indx;
      premutoy = indy;
      break;
    }
  }
}

void Application::passaEventoLaunch(EVENTO ev) {
  // cout << "ev.x -> " << ev.x << " ev.y -> " << ev.y << " premuto -> "
  //      << ev.premuto << endl;
  this->attivi[ev.x][ev.y] = ev.premuto;
}

void launchAppCallback(EVENTO ev) { app->passaEventoLaunch(ev); }