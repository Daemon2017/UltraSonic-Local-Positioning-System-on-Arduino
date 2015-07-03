#include <Wire.h>
#include <EasyTransferI2C.h>

EasyTransferI2C et_y; 
struct SEND_DATA_STRUCTURE_Y
{
  float time_y;
};
SEND_DATA_STRUCTURE_Y data_y;

#define sputnic_T_2 11
#define sputnic_E_2 12

float time_y;

void setup() 
{
  Wire.begin(10);
  
  et_y.begin(details(data_y), &Wire);
  
  pinMode(sputnic_T_2, OUTPUT); 
  pinMode(sputnic_E_2, INPUT); 
  
  pinMode(8, INPUT);
  
  pinMode(13, OUTPUT);
}

void loop() 
{              
    if(digitalRead(8) == HIGH)
    {  
      digitalWrite(13, HIGH);
      digitalWrite(sputnic_T_2, HIGH);       
      delayMicroseconds(10);
      digitalWrite(sputnic_T_2, LOW);       
      time_y = pulseIn(sputnic_E_2, HIGH);
      digitalWrite(13, LOW);  
          
      data_y.time_y = time_y;
             
      et_y.sendData(8);       
    }   
}
