// Title: Multi HX711
// Author: Alexis Devillard
// Date: 27/11/22
// Description: Read simultaneously the data from 8 HX711 modules
//              connected to 22-29 on a mega2560. (need to change
//              the port registers reading if using an other board)
#include "HX711.hpp"
#define NB_LOADCELL 6
int hx711_data_pin[NB_LOADCELL] = { 22, 23, 24, 25, 26, 27 };
int32_t res[NB_LOADCELL] = { 0 };
int32_t offset[8] = { 0 };
const int sclk = 2;
int32_t val[NB_LOADCELL];

void setup() {
  Serial.begin(38400);
  pinMode(sclk, OUTPUT);
  for (int i = 0; i < NB_LOADCELL; i++)
    pinMode(hx711_data_pin[i], INPUT_PULLUP);

  int count = 0;
  for (int i = 0; i < NB_LOADCELL; i++) {
    offset[i] = 0;
    res[i] = 0;
  }
  while (count < 10) {
    if (read_8hx(res, hx711_data_pin, NB_LOADCELL, sclk)) {
      count++;
      for (int i = 0; i < NB_LOADCELL; i++)
        offset[i] += res[i];
    }
  }
  for (int i = 0; i < NB_LOADCELL; i++)
    offset[i] /= count;
}

void loop() {

  if (read_8hx(res, hx711_data_pin, NB_LOADCELL, sclk)) {
    for (uint8_t i = 0; i < NB_LOADCELL; i++) {
      Serial.print(res[i] - offset[i]);
      Serial.print(" ");
    }
    Serial.println("");
  }
  //delay(10);
}
