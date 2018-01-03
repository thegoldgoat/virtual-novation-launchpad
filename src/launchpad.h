#ifndef _LAUNCHPAD_H
#define _LAUNCHPAD_H

#include <rtmidi/RtMidi.h>
#include <vector>

typedef unsigned char midiBitT;

struct EVENTO {
  EVENTO(int _x, int _y, bool _premuto, bool _cancella, midiBitT _velocity)
      : x(_x), y(_y), premuto(_premuto), cancella(_cancella),
        velocity(_velocity) {}

  // Posizione x nella matrice 9x9
  int x;
  // Posizione y nella matrice 9x9
  int y;
  // Vero se premuto, falso se rilasciato
  bool premuto;
  // Vero se il launchpad si deve cancellare
  bool cancella;
  // Velocity MIDI
  midiBitT velocity;
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
  // void sendDestra(int y, midiBitT velocity);
  // Invia un segnale midi ai tasti a sopra
  void sendTop(int x, midiBitT velocity);
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
