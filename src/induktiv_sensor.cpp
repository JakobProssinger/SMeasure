#include "../include/induktiv_sensor.hpp"
#include "Arduino.h"

induktiv_sensor::induktiv_sensor(int FP, int BP, unsigned long MI)
{
  this->Front_PIN = FP;
  this->Back_PIN = BP;
  this->measureIntervallMS = MI;
}

induktiv_sensor::~induktiv_sensor()
{
  detachInterrupt(this->Front_PIN);
  detachInterrupt(this->Back_PIN);
}