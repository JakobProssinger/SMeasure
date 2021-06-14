#ifndef INDUKTKIV_SENSOR_HPP
#define INDUKTKIV_SENSOR_HPP

#define FORWARDS true
#define BACKWARDS false

#include <Arduino.h>
#include <string.h>
class induktiv_sensor
{

public:
  int Front_PIN, Back_PIN;
  double frequency = 0;
  volatile unsigned long FrontTime = 0, BackTime = 0, FrontTriggerCounter = 0, lastFrontTime = 0;
  unsigned long delta = 0;
  bool direction = FORWARDS;
  induktiv_sensor(int, int);
  ~induktiv_sensor();
  void (*ISR_Front)(void);
  void (*ISR_Back)(void);
};

#endif // INDUKTKIV_SENSOR_HPP