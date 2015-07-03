#include <Wire.h>
#include <EasyTransferI2C.h>

EasyTransferI2C et_z; 
struct RECEIVE_DATA_STRUCTURE_Z
{
  float x, y, z;
  float r1, r2, r3;
};
RECEIVE_DATA_STRUCTURE_Z data_z;

float time_z;

void setup() 
{
  Serial.begin(9600);
  
  Wire.begin(11);
  Wire.onReceive(receive);
  
  et_z.begin(details(data_z), &Wire);
    
  pinMode(13, OUTPUT);
}

void loop() 
{              
  if(et_z.receiveData())
  {    
    Serial.print("r1: ");
    Serial.println(data_z.r1, 3);
    Serial.print("r2: ");
    Serial.println(data_z.r2, 3);
    Serial.print("r3: ");    
    Serial.println(data_z.r3, 3);
    
    Serial.print("x: ");
    Serial.println(data_z.x, 3);
    Serial.print("y: ");
    Serial.println(data_z.y, 3);
    Serial.print("z: ");
    Serial.println(data_z.z, 3);
    Serial.println();
  }
}

void receive(int numBytes) {}
