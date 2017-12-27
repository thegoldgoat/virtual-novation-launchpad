#ifndef _APPLICAZIONE_H
#define _APPLICAZIONE_H
#include "launchpad.h"
#include "sdlApp.h"
#include <rtmidi/RtMidi.h>

#define DIM_LAUNCH 8
#define PADDING_TASTI 5


void launchAppCallback(EVENTO);

class Application : public SDLApp {
public:
  // Public variables and method
  Application();
  void draw(void);
  void events(void);

  void passaEventoLaunch(EVENTO);

private:
  // quadrati accesi
  bool attivi[DIM_LAUNCH][DIM_LAUNCH];

  // Cerchi accesi sopra
  bool attiviSopra[DIM_LAUNCH];
  // Classe del launchpad virtuale
  Launchpad *launchpad = new Launchpad(launchAppCallback);

  // coordinate del tasto premuto col mouse (da rilasciare al rilascio)
  int premutox;
  int premutoy;

  // Rettangolo dove sta la matrice
  SDL_Rect matrice;

};

extern Application *app;

#endif