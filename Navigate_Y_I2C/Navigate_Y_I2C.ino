#include <Wire.h>
#include <EasyTransferI2C.h>

EasyTransferI2C et_y; 
struct SEND_DATA_STRUCTURE_Y
{
  float time_y;
};
SEND_DATA_STRUCTURE_Y data_y;

#define TRIG 11
#define ECHO 12

#define MY_ADDR 10
#define MASTER_ADDR 8

#define SYNC_Y 8

#define LED 13

float time_y;

void setup() 
{
  Wire.begin(MY_ADDR);
  
  et_y.begin(details(data_y), &Wire);
  
  pinMode(TRIG, OUTPUT); 
  pinMode(ECHO, INPUT); 
  
  pinMode(SYNC_Y, INPUT);
  
  pinMode(LED, OUTPUT);
}

void loop() 
{              
    if(digitalRead(SYNC_Y) == HIGH)
    {  
      digitalWrite(LED, HIGH);
      digitalWrite(TRIG, HIGH);       
      delayMicroseconds(10);
      digitalWrite(TRIG, LOW);       
      time_y = pulseIn(ECHO, HIGH);
      digitalWrite(LED, LOW);  
          
      data_y.time_y = time_y / 1000000;
             
      et_y.sendData(MASTER_ADDR);       
    }   
}
