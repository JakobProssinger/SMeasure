#ifndef INDUKTKIV_SENSOR_HPP
#define INDUKTKIV_SENSOR_HPP

#define FORWARDS true
#define BACKWARDS false

#include <Arduino.h>
#include <string.h>

class induktiv_sensor
{

public:
  int frontPIN, backPIN, centerPin;
  double frequency = 0;
  unsigned long frontTime = 0, backTime = 0, frontTriggerCounter = 0;
  unsigned long lastFrontTime = 0;
  unsigned long delta = 0;
  unsigned long measureIntervallMS;
  bool direction;
  induktiv_sensor(int, int, int, unsigned long);
  ~induktiv_sensor();
};

#endif // INDUKTKIV_SENSOR_HPP