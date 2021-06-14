#include "../include/induktiv_sensor.hpp"
#include "Arduino.h"

induktiv_sensor::induktiv_sensor(int FP, int BP, unsigned long MI)
{
  this->frontPIN = FP;
  this->backPIN = BP;
  this->measureIntervallMS = MI;
}

induktiv_sensor::~induktiv_sensor()
{
  detachInterrupt(this->frontPIN);
  detachInterrupt(this->backPIN);
}