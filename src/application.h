#ifndef _APPLICAZIONE_H
#define _APPLICAZIONE_H
#include "launchpad.h"
#include "sdlApp.h"
#include <rtmidi/RtMidi.h>

#define DIM_LAUNCH 9
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
  // quadrati accesi (griglia 9x9)
  SDL_Color attivi[DIM_LAUNCH][DIM_LAUNCH];

  // Classe del launchpad virtuale
  Launchpad *launchpad = new Launchpad(launchAppCallback);

  // coordinate del tasto premuto col mouse (da rilasciare al rilascio)
  int premutox;
  int premutoy;

  static SDL_Color midiToColor(midiBitT midiBit) {
    /*
       Velocity = (16 x Green)
                + Red
                + Flags
    */
    // Ignoring flags for now
    midiBitT red = midiBit % 16;
    midiBitT green = (midiBit - red) / 16;
    // std::cout << "Red -> " << (int)red << " green -> " << (int)green
    //           << std::endl;
    // Normalize values: from 4 bit to 8 bit (adding a bit of brightness)
    red += 4;
    green += 4;
    return {red * 16, green * 16, 0};
  }
};

extern Application *app;

#endif