#ifndef _LAUNCHPAD_H
#define _LAUNCHPAD_H

#include <rtmidi/RtMidi.h>
#include <vector>

typedef unsigned char midiBitT;

struct EVENTO {
  int x;
  int y;
  bool premuto; // Vero se premuto, falso se rilasciato
};

inline void launchpapMidiCallback(double, std::vector<unsigned char> *, void *);

class Launchpad {
private:
  RtMidiIn *midiin = NULL;
  RtMidiOut *midiout = NULL;

public:
  Launchpad(void (*_callback)(EVENTO));
  ~Launchpad();
  // Invia un segnale midi al launchpad
  void send(int x, int y, midiBitT velocity);
  // Invia un segnale midi ai tasti a destra
  void sendDestra(int y, midiBitT velocity);
  // Cancella tutte le celle del launchpad
  void cancella();

  // Callback ricevuto quando arriva un messaggio midi.
  void midiRicevuto(std::vector<unsigned char>);

  // Funzione da chiamare quando si riceve un evento MIDI.
  void (*callback)(EVENTO);

  // Da un messaggio midi di RtMidi, ritorno l'evento.
  EVENTO mappaMidi(std::vector<unsigned char>);
  // Ricezione asincrona
  EVENTO recv();
};

#endif /* end of include guard: _LAUNCHPAD_H */
