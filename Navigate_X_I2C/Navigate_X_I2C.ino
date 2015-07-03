#include <Wire.h>
#include <EasyTransferI2C.h>

EasyTransferI2C et_x; 
struct SEND_DATA_STRUCTURE_X
{
  float time_x;
};
SEND_DATA_STRUCTURE_X data_x;

#define sputnic_T_2 11
#define sputnic_E_2 12

float time_x;

void setup() 
{
  Wire.begin(9);
  
  et_x.begin(details(data_x), &Wire);
  
  pinMode(sputnic_T_2, OUTPUT); 
  pinMode(sputnic_E_2, INPUT); 
  
  pinMode(7, INPUT);
  
  pinMode(13, OUTPUT);
}

void loop() 
{              
    if(digitalRead(7) == HIGH)
    {  
      digitalWrite(sputnic_T_2, HIGH);   
      digitalWrite(13, HIGH);
      delayMicroseconds(10);
      digitalWrite(sputnic_T_2, LOW);         
      time_x = pulseIn(sputnic_E_2, HIGH);
      digitalWrite(13, LOW);  
          
      data_x.time_x = time_x;
       
      et_x.sendData(8); 
    }   
}
