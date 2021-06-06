#include <Arduino.h>

#include "../include/induktiv_sensor.hpp"
#include "../include/file_handling.hpp"


#include <Wire.h> //I2C 
#include "SPIFFS.h" //Filesystem

#define START "START" //TODO set correct names for start and stop
#define STOP "STOP"

//#define FORWARDS true;
//#define BACKWARDS false;

const int Front_PIN=16; 
const int Back_PIN=17; //TODO set corrrect pin from Eagle Board

unsigned long FrontTime = 0, BackTime = 0, FrontTriggerCounter = 0, lastFrontTime;

unsigned long delta = 0;
double frequency = 0;


void IRAM_ATTR onTimer_FrontSensor(){
  FrontTriggerCounter++;
  FrontTime = millis();
}

void IRAM_ATTR onTimer_BackSensor() {
  BackTime = millis();
}

induktiv_sensor *SensorRightWeel = new induktiv_sensor(Front_PIN, Back_PIN);
String message = START;
void setup() {  
    
    pinMode(Front_PIN, INPUT);
    pinMode(Back_PIN, INPUT);
    Serial.begin(115200);

    delay(1000);
}

void getRotation(induktiv_sensor* aSensor){
    if(FrontTriggerCounter == 0)
    {
      aSensor->frequency = 0.0;
      aSensor->direction = FORWARDS;
      return;
    }

    aSensor->delta = millis() - aSensor->lastFrontTime;
    if((delta) >= 1000)
    {
      aSensor->frequency = ((double) aSensor->FrontTriggerCounter / (double) delta) * 1000.0 ;  //Rotation frequency in 1/Second
      if(aSensor->BackTime <= aSensor->FrontTime)
      {
        aSensor->direction = FORWARDS;
      } else 
      {
        aSensor->direction = BACKWARDS;
      }
      aSensor->FrontTriggerCounter = 0;
      aSensor->lastFrontTime = aSensor->FrontTime;
    }
}

void loop() {
  //message = getBTInput();

  if(message == START)
  {
    while(message != STOP)
    {
      //getGyro();
      getRotation(SensorRightWeel);
      /*
      if(Connection()){
        SendData();
      }else {
        StoreData();
      }
      message = getBTInput();*/
    }
  }

  delay(20);
}



