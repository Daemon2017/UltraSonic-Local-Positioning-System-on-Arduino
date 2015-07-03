#include <Wire.h>
#include <EasyTransferI2C.h>

EasyTransferI2C ET_Y; 
struct SEND_DATA_STRUCTURE_Y
{
  float time_y;
};
SEND_DATA_STRUCTURE_Y data_y;

#define sputnic_T_2 11
#define sputnic_E_2 12

float time_y;

#define I2C_SLAVE_ADDRESS2 8
#define I2C_SLAVE_ADDRESS3 10

void setup() 
{
  Wire.begin(I2C_SLAVE_ADDRESS3);
  
  ET_Y.begin(details(data_y), &Wire);
  
  pinMode(sputnic_T_2, OUTPUT); 
  pinMode(sputnic_E_2, INPUT); 
  
  pinMode(8, INPUT);
  
  pinMode(13, OUTPUT);
}

void loop() 
{              
    if(digitalRead(8) == HIGH)
    {  
      digitalWrite(sputnic_T_2, HIGH);   
      delayMicroseconds(10);
      digitalWrite(sputnic_T_2, LOW);       
      time_y = pulseIn(sputnic_E_2, HIGH);
          
      data_y.time_y = time_y;
       
      ET_Y.sendData(I2C_SLAVE_ADDRESS2); 
      
      digitalWrite(13, HIGH);
      delay(200);
      digitalWrite(13, LOW);  
    }   
}
