#include <Wire.h>
#include <EasyTransferI2C.h>

EasyTransferI2C et_x; 
struct SEND_DATA_STRUCTURE_X
{
  float time_x;
};
SEND_DATA_STRUCTURE_X data_x;

#define TRIG 11
#define ECHO 12

#define MY_ADDR 9
#define MASTER_ADDR 8

#define SYNC_X 7

#define LED 13

float time_x;

void setup() 
{
  Wire.begin(MY_ADDR);
  
  et_x.begin(details(data_x), &Wire);
  
  pinMode(TRIG, OUTPUT); 
  pinMode(ECHO, INPUT); 
  
  pinMode(SYNC_X, INPUT);
  
  pinMode(LED, OUTPUT);
}

void loop() 
{              
    if(digitalRead(SYNC_X) == HIGH)
    {      
      digitalWrite(TRIG, HIGH);   
      delayMicroseconds(10);
      digitalWrite(TRIG, LOW);         
      time_x = pulseIn(ECHO, HIGH);
      
      data_x.time_x = time_x / 1000000;
       
      while(digitalRead(SYNC_X) == LOW)
      {       
        digitalWrite(LED, HIGH);      
      }
       et_x.sendData(MASTER_ADDR); 
       
       digitalWrite(LED, LOW);        
    }   
}
