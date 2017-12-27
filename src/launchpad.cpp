#ifndef _LAUNCHPAD_CPP
#define _LAUNCHPAD_CPP

#include "launchpad.h"
#include <iostream>
#include <rtmidi/RtMidi.h>
#include <vector>

#define KEY 144
#define WIDTH_LAUNCHPAD 9
#define HEIGHT_LAUNCHPAD 9
#define ROSSO_MIDI 10 // Valore MIDI per il Launchpad per avere il rosso

// const int mappa[WIDTH_LAUNCHPAD][HEIGHT_LAUNCHPAD] = {
//     {0, 1, 2, 3, 4, 5, 6, 7},
//     {16, 17, 18, 19, 20, 21, 22, 23},
//     {32, 33, 34, 35, 36, 37, 38, 39},
//     {48, 49, 50, 51, 52, 53, 54, 55},
//     {64, 65, 66, 67, 68, 69, 70, 71},
//     {80, 81, 82, 83, 84, 85, 86, 87},
//     {96, 97, 98, 99, 100, 101, 102, 103},
//     {112, 113, 114, 115, 116, 117, 118, 119}};

using namespace std;

Launchpad::Launchpad(void (*_callback)(EVENTO)) : callback(_callback) {
  this->midiin = new RtMidiIn;
  this->midiout = new RtMidiOut;

  this->midiin->openVirtualPort(std::string("Launchpad virtuale"));
  this->midiout->openVirtualPort(std::string("Launchpad virtuale"));

  this->midiin->setCallback(launchpapMidiCallback, this);
  this->midiin->ignoreTypes(false, false, false);
}

Launchpad::~Launchpad() {
  this->cancella();
  this->midiin->closePort();
  this->midiout->closePort();
}

void Launchpad::send(int x, int y, midiBitT velocity) {
  // Escludi i casi in cui provi a mandare un messaggio in zone non
  // valide.
  if (x >= WIDTH_LAUNCHPAD || x < 0)
    return;
  if (y >= HEIGHT_LAUNCHPAD || y < 0)
    return;
  if (y == 0) {
    this->sendTop(x, velocity);
    return;
  }
  // Altrimenti scendi di uno, perche' la y * 16 tiene conto dell'8x8
  // e non del 9x9
  y--;
  vector<midiBitT> messaggio(3);
  messaggio[0] = KEY;
  messaggio[1] = x + 16 * y;
  messaggio[2] = velocity;
  this->midiout->sendMessage(&messaggio);
}

// void Launchpad::sendDestra(int y, midiBitT velocity) {
//   if (y >= HEIGHT_LAUNCHPAD || y < 0)
//     return;
//   vector<midiBitT> messaggio(3);
//   messaggio[0] = KEY;
//   messaggio[1] = 8 + y * 16;
//   messaggio[2] = velocity;
//   this->midiout->sendMessage(&messaggio);
// }

void Launchpad::sendTop(int x, midiBitT velocity) {
  if (x >= WIDTH_LAUNCHPAD || x < 0)
    return;
  vector<midiBitT> messaggio(3);
  messaggio[0] = 176;
  messaggio[1] = 104 + x;
  messaggio[2] = velocity;
  this->midiout->sendMessage(&messaggio);
}

void Launchpad::cancella() {
  vector<midiBitT> messaggio(3);
  messaggio[0] = KEY;
  messaggio[2] = 0;
  for (int i = 0; i < WIDTH_LAUNCHPAD; i++) {
    for (int j = 0; j < HEIGHT_LAUNCHPAD; j++) {
      messaggio[1] = i + 16 * j;
      this->midiout->sendMessage(&messaggio);
    }
  }
}

EVENTO Launchpad::mappaMidi(std::vector<unsigned char> message) {
  // Messagge[0] vale 0 se non si ha niente in input. almeno nel launchpad.
  if (message[0] == 0) {
    return {-1, -1, false};
  }
  // Converti il valore midi in coordinate del launchpad
  if (message[0] == KEY) {
    int x, y;
    x = message[1] % 16;
    y = (message[1] - x) / 16;
    cout << "x -> " << x << "y -> " << y << endl;
    return {x, y + 1, (message[2] != 0)};
  } else if (message[0] == 176) {
    return {message[1] - 104, 0, (message[2] != 0)};
  } else {
    cout << "Midi sconosciuto -> " << (int)message[0] << endl;
    return {-1, -1, false};
  }
}

EVENTO Launchpad::recv() {
  std::vector<unsigned char> message(3);
  this->midiin->getMessage(&message);
  return this->mappaMidi(message);
}

void Launchpad::midiRicevuto(std::vector<unsigned char> messaggio) {
  EVENTO ritorno = this->mappaMidi(messaggio);
  if (ritorno.x == -1)
    return;
  this->callback(ritorno);
}

inline void launchpapMidiCallback(double delta,
                                  std::vector<unsigned char> *messaggio,
                                  void *launchpointer) {
  Launchpad *launchpad = (Launchpad *)launchpointer;
  launchpad->midiRicevuto(*messaggio);
  // cout << "Messaggio[0] -> " << (int)(*messaggio)[0] << "; Messaggio[1] -> "
  // << (int)(*messaggio)[1] << "; messaggio[2] -> " << (int)(*messaggio)[2]
  // << endl;
}

#endif /* end of include guard: _LAUNCHPAD_CPP */
