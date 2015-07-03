#include <Wire.h>
#include <EasyTransferI2C.h>

EasyTransferI2C ET_Z; 
struct SEND_DATA_STRUCTURE_Z
{
  float time_z;
};
SEND_DATA_STRUCTURE_Z data_z;

#define sputnic_T_2 11
#define sputnic_E_2 12

float time_z;

void setup() 
{
  Wire.begin(11);
  
  ET_Z.begin(details(data_z), &Wire);
  
  pinMode(sputnic_T_2, OUTPUT); 
  pinMode(sputnic_E_2, INPUT); 
  
  pinMode(9, INPUT);
  
  pinMode(13, OUTPUT);
}

void loop() 
{              
    if(digitalRead(9) == HIGH)
    {  
      digitalWrite(sputnic_T_2, HIGH);   
      delayMicroseconds(10);
      digitalWrite(sputnic_T_2, LOW);       
      time_z = pulseIn(sputnic_E_2, HIGH);
          
      data_z.time_z = time_z;
       
      ET_Z.sendData(8); 
      
      digitalWrite(13, HIGH);
      delay(200);
      digitalWrite(13, LOW);  
    }   
}
