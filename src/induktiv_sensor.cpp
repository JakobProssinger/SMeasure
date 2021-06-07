#include "../include/induktiv_sensor.hpp"
#include "Arduino.h"
#define FORWARDS true;
#define BACKWARDS false;

induktiv_sensor::induktiv_sensor(int Front_PIN, int Back_PIN)
{
  this->Front_PIN = Front_PIN;
  this->Back_PIN = Back_PIN;
  attachInterrupt(this->Front_PIN, ISR_Front, RISING);
  attachInterrupt(this->Back_PIN, ISR_Back, RISING);
}


induktiv_sensor::~induktiv_sensor(){
  detachInterrupt(this->Front_PIN);
  detachInterrupt(this->Back_PIN);
}

