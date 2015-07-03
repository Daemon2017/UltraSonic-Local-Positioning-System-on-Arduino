#include <Wire.h>
#include <EasyTransferI2C.h>

//Для связи с модулем Х-координаты
EasyTransferI2C et_x; 
struct RECEIVE_DATA_STRUCTURE_X
{
  float time_x;
};
RECEIVE_DATA_STRUCTURE_X data_x;

//Для связи с модулем Y-координаты
EasyTransferI2C et_y; 
struct RECEIVE_DATA_STRUCTURE_Y
{
  float time_y;
};
RECEIVE_DATA_STRUCTURE_Y data_y;

EasyTransferI2C et_z; 
struct SEND_DATA_STRUCTURE_Z
{
  float x, y, z;
  float r1, r2, r3;
};
SEND_DATA_STRUCTURE_Z data_z;

//Разъемы подключения спутника
#define TRIG 3
#define ECHO 2

//Разъемы синхросигналов
#define SYNC_X 7
#define SYNC_Y 8

#define LED 13

#define WAIT 874

#define MY_ADDR 8

float sonic, coordinate, quad = 0.45;
float r1, r2, r3;
float x, y, z;

void setup() 
{
  Serial.begin(9600);  
  
  Wire.begin(MY_ADDR);
  Wire.onReceive(receive); 
  
  et_x.begin(details(data_x), &Wire);
  et_y.begin(details(data_y), &Wire);
  et_z.begin(details(data_z), &Wire);
  
  pinMode(TRIG, OUTPUT); 
  pinMode(ECHO, INPUT); 
  
  pinMode(SYNC_X, OUTPUT);
  pinMode(SYNC_Y, OUTPUT);
  
  pinMode(LED, OUTPUT);
}

void loop() 
{        
  getTemp();
  
  Serial.println("V");
  
  delayMicroseconds(WAIT);
  
  digitalWrite(SYNC_X, HIGH);  
  digitalWrite(SYNC_Y, HIGH); 
  delayMicroseconds(5);
  digitalWrite(SYNC_X, LOW); 
  digitalWrite(SYNC_Y, LOW); 
  
  getCoordO();
  getCoordY();
  delay(250);
  getCoordX();
  
  trilateration();
  
  delay(500);
}

void led()
{
  digitalWrite(LED, HIGH);
  delayMicroseconds(10);
  digitalWrite(LED, LOW); 
}

void getTemp()
{
  float temperatureC = ((((analogRead(A1) * 5.0) / 1024.0) - 0.55) * 100);  
  
  sonic = sqrt(1.4 * 287 * (temperatureC + 273.15));
}

void getCoordO()
{  
  digitalWrite(TRIG, HIGH);  
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  float time_base = pulseIn(ECHO, HIGH);  
  r1 = time_base * sonic / 1000000;
}

void getCoordX()
{
  if(et_x.receiveData())
  {                 
    r2 = calc(1);
    led();
   }  
}

void getCoordY()
{ 
  if(et_y.receiveData())
  {                 
    r3 = calc(2);
    led();
   }      
}

float calc(int peremennaja)
{
  float data, distance;
  switch (peremennaja)
  {
    case 1:
    data = data_x.time_x;
    break;
    
    case 2:
    data = data_y.time_y;
    break;
  }   
  return distance = data * sonic;
}

void trilateration()
{  
  x = ( pow(r1, 2) - pow(r3, 2) + pow(quad, 2) ) / ( 2 * quad );  
  y = ( pow(r1, 2) - pow(r2, 2) + pow(quad, 2) ) / ( 2 * quad );  
  z = sqrt( pow(r1, 2) - pow(x, 2) - pow(y, 2) );
  
  data_z.r1 = r1;
  data_z.r2 = r2;
  data_z.r3 = r3;
  data_z.x = x;  
  data_z.y = y;
  data_z.z = z;   
  et_z.sendData(11); 
}

void receive(int numBytes) {}
