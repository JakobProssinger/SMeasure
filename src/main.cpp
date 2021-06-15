#include <Arduino.h>

#include "../include/induktiv_sensor.hpp"
#include "../include/file_handling.hpp"
#include <Wire.h>          //I2C
#include "SPIFFS.h"        //Filesystem
#include <MPU6050_light.h> // MPU6050
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define START "start"
#define STOP "stop"

const int frontPIN = 16;
const int backPIN = 17;
unsigned long aIntervall = 1000;

induktiv_sensor *SensorRightWheel = new induktiv_sensor(frontPIN, backPIN, aIntervall);

void IRAM_ATTR onTimer_FrontSensor()
{
  SensorRightWheel->frontTriggerCounter++;
  SensorRightWheel->frontTime = millis();
}

void IRAM_ATTR onTimer_BackSensor()
{
  SensorRightWheel->backTime = millis();
}

BluetoothSerial SerialBT;
String message = START; //TDOO set to STOP
volatile unsigned long BT_Time = 0;
const unsigned long BT_INTERVALL = 1000;

volatile unsigned long MPU_Time = 0;
const unsigned long MPU_INTERVALL = 1000;
MPU6050 mpu(Wire);
String GyroData = "";

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Project: SMeasure");
  byte MPUstatus = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(MPUstatus);
  while (MPUstatus != 0)
  {
  } // stop everything if could not connect to MPU6050

  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  //mpu.upsideDownMounting = true;
  mpu.calcOffsets(true, true); // gyro and accelero
  Serial.println("MPU6050 Setup Done!\n");

  pinMode(frontPIN, INPUT);
  pinMode(backPIN, INPUT);

  attachInterrupt(SensorRightWheel->frontPIN, onTimer_FrontSensor, FALLING);
  attachInterrupt(SensorRightWheel->backPIN, onTimer_BackSensor, FALLING);

  SerialBT.begin("SMeasure"); //Name des ESP32
  Serial.println("The device started, now you can pair it with bluetooth!");
  delay(1000);
}

void getRotation(induktiv_sensor *aSensor)
{
  aSensor->delta = millis() - aSensor->lastFrontTime;
  noInterrupts();
  if (aSensor->delta >= aSensor->measureIntervallMS)
  {
    aSensor->frequency = ((double)aSensor->frontTriggerCounter / (double)aSensor->delta) * 1000.0; //Rotation frequency in 1/s
    
    if (aSensor->backTime < aSensor->frontTime)
    {
      aSensor->direction = FORWARDS;
    }
    else
    {
      aSensor->direction = BACKWARDS;
    }
    aSensor->frontTriggerCounter = 0;
    aSensor->lastFrontTime = aSensor->frontTime;
    aSensor->delta = 0;
    //Serial.println("B: " + (String)SensorRightWheel->backTime + " |F: " + (String)SensorRightWheel->frontTime + " |D: " + SensorRightWheel->direction + " | " + (String)(SensorRightWheel->frontTime - SensorRightWheel->backTime));
  }
  interrupts()
}

String getBTInput()
{
  if (SerialBT.available())
  {
    return SerialBT.readStringUntil('\n');
  }
  return message;
}

String getGyro()
{
  if ((millis() - MPU_Time) >= MPU_INTERVALL)
  {
    MPU_Time = millis();
    return "Pitch, Roll = " + (String)mpu.getAngleX() + "  " + (String)mpu.getAngleY() + " || ";
  }
  return GyroData;
}

bool SendData(String aMessage)
{
  SerialBT.print(aMessage);
  if (SerialBT.available())
  {
    SerialBT.print(aMessage);
    return true;
  }
  return false;
}

void loop()
{
  mpu.update();
  message = getBTInput();  
  if (message == START)
  {
    while (message != STOP)
    {
      mpu.update();
      GyroData = getGyro();
      getRotation(SensorRightWheel);

      if ((millis() - BT_Time) >= BT_INTERVALL)
      {
        if (!SendData(GyroData+ " Frequenz = " + (String)SensorRightWheel->frequency + ":"))
        {
          //TODOPrint in Error file
          Serial.println("ERROR ");
        }
         BT_Time = millis();
      }
      else
      {
        // StoreData();
      }
      
      message = getBTInput();
      
    }
    SerialBT.println("\n");
  }

  delay(20);
}
