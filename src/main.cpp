#include <Arduino.h>

#include "../include/induktiv_sensor.hpp"
#include "../include/file_handling.hpp"


#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
  #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#include <Wire.h> //I2C 
#include "SPIFFS.h" //Filesystem

#define START "START" //TODO set correct names for start and stop
#define STOP "STOP"

//#define FORWARDS true;
//#define BACKWARDS false;

const int Front_PIN=16; 
const int Back_PIN=17; //TODO set corrrect pin from Eagle Board

induktiv_sensor *SensorRightWeel = new induktiv_sensor(Front_PIN, Back_PIN);

void IRAM_ATTR onTimer_FrontSensor()
{
  SensorRightWeel->FrontTriggerCounter++;
  SensorRightWeel->FrontTime = millis();
}

void IRAM_ATTR onTimer_BackSensor() 
{
  SensorRightWeel->BackTime = millis();
}

BluetoothSerial SerialBT;
String message = STOP;

void setup() 
{  
    pinMode(Front_PIN, INPUT);
    pinMode(Back_PIN, INPUT);
    Serial.begin(115200);

    attachInterrupt(SensorRightWeel->Front_PIN, onTimer_FrontSensor, RISING);
    attachInterrupt(SensorRightWeel->Back_PIN, onTimer_BackSensor, RISING);

    SerialBT.begin("ESP32test"); //Name des ESP32
    delay(1000);
}

void getRotation(induktiv_sensor* aSensor)
{
  aSensor->FrontTime = millis();
  aSensor->delta = aSensor->FrontTime - aSensor->lastFrontTime;
  if(aSensor->delta >= 1000)
  {
    if(aSensor->FrontTriggerCounter == 0)
    {
      aSensor->frequency = 0.0;
      aSensor->direction = FORWARDS;
      return;
    }
    aSensor->frequency = ((double) aSensor->FrontTriggerCounter / (double) aSensor->delta) * 1000.0 ;  //Rotation frequency in 1/Second
    if(aSensor->BackTime <= aSensor->FrontTime)
    {
      aSensor->direction = FORWARDS;
    } else 
    {
      aSensor->direction = BACKWARDS;
    }
    aSensor->FrontTriggerCounter = 0;
    aSensor->lastFrontTime = aSensor->FrontTime;
    aSensor->delta = 0;

  }
}

String getBTInput(){
  if(SerialBT.available()){
      return SerialBT.readString();
  }
  return message;
}

bool test = true;
void loop() 
{
  message = getBTInput();
  if(message == START)
  {
    
    while(message != STOP)
    {
      //getGyro();
      getRotation(SensorRightWeel);

      if(SerialBT.available()){
       // SendData();
      }else {
       // StoreData();
      }
      message = getBTInput();
    }
  }

  delay(20);
}



