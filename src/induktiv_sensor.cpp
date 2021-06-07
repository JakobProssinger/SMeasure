#include "../include/induktiv_sensor.hpp"
#include "Arduino.h"
#define FORWARDS true;
#define BACKWARDS false;

induktiv_sensor::induktiv_sensor(int FP, int BP)
{
  this->Front_PIN = FP;
  this->Back_PIN = BP;

}


induktiv_sensor::~induktiv_sensor(){
  detachInterrupt(this->Front_PIN);
  detachInterrupt(this->Back_PIN);
}

