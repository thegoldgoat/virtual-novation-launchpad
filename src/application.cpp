#include "application.h"
#include "global.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>
using namespace std;

Application *app;

const SDL_Color coloreSpento = {102, 102, 102};

Application::Application() : SDLApp(WINDOW_TITLE, W, H, FPS) {
  for (int i = 0; i < DIM_LAUNCH; i++) {
    for (int j = 0; j < DIM_LAUNCH; j++)
      this->attivi[i][j] = coloreSpento;
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
  double radCircles = offsetx > offsety ? offsety / 2 : offsetx / 2;
  radCircles -= PADDING_TASTI;
  // Top and right
  for (int i = 0; i < DIM_LAUNCH - 1; i++) {
    // Top
    temp.x = offsetx / 2 + offsetx * i;
    temp.y = PADDING_TASTI + offsety / 2;
    filledCircleRGBA(renderer, temp.x, temp.y, radCircles, attivi[i][0].r,
                     attivi[i][0].g, attivi[i][0].b, 255);
    // Right
    temp.x = w - offsetx / 2;
    temp.y = offsety * (i + 1) + offsety / 2;
    // Because the indexes to the right start from 1
    i++;
    filledCircleRGBA(renderer, temp.x, temp.y, radCircles, attivi[8][i].r,
                     attivi[8][i].g, attivi[8][i].b, 255);
    i--;
  }

  // Central matrix
  for (int i = 0; i < DIM_LAUNCH - 1; i++)
    for (int j = 1; j < DIM_LAUNCH; j++) {
      // Non disegnare l'angolo in alto a destra
      if (i == 8 && j == 0)
        continue;
      temp.x = i * offsetx + PADDING_TASTI;
      temp.y = j * offsety + PADDING_TASTI;
      roundedBoxRGBA(renderer, temp.x, temp.y, temp.x + temp.w, temp.y + temp.h,
                     10, attivi[i][j].r, attivi[i][j].g, attivi[i][j].b, 255);
    }
}

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
  //  << ev.premuto << endl;
  if (ev.cancella) {
    // Cancella tutte le celle
    for (int i = 0; i < DIM_LAUNCH; i++) {
      for (int j = 0; j < DIM_LAUNCH; j++) {
        this->attivi[i][j] = coloreSpento;
      }
    }
  } else {
    this->attivi[ev.x][ev.y] =
        ev.premuto == true ? this->midiToColor(ev.velocity) : coloreSpento;
  }
}

void launchAppCallback(EVENTO ev) { app->passaEventoLaunch(ev); }