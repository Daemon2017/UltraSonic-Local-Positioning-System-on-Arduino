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

//Разъемы подключения спутника
#define sputnic_T_1 3
#define sputnic_E_1 2

//Разъемы синхросигналов
#define sync_x 7
#define sync_y 8

#define LED 13

float sonic, coordinate, quad = 0.45;
float r1, r2, r3;
float x, y, z;

void setup() 
{
  Serial.begin(9600);  
  
  Wire.begin(8);
  Wire.onReceive(receive); 
  
  et_x.begin(details(data_x), &Wire);
  et_y.begin(details(data_y), &Wire);
  
  pinMode(sputnic_T_1, OUTPUT); 
  pinMode(sputnic_E_1, INPUT); 
  
  pinMode(sync_x, OUTPUT);
  pinMode(sync_y, OUTPUT);
  
  pinMode(LED, OUTPUT);
}

void loop() 
{        
  getTemp();
  getCoordO();
  getCoordX();
  getCoordY();
  trilateration();
}

void led()
{
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW); 
}

void getTemp()
{
  float temperatureC = ((((analogRead(A1) * 5.0) / 1024.0) - 0.55) * 100);  
  Serial.print("Degrees: ");
  Serial.println(temperatureC); 
  
  sonic = sqrt(1.4 * 287 * (temperatureC + 273.15));
  Serial.print("Sonic speed: ");
  Serial.println(sonic); 
}

void getCoordO()
{
  digitalWrite(sputnic_T_1, HIGH);
  robotSync();
  digitalWrite(sputnic_T_1, LOW);
  float time_base = pulseIn(sputnic_E_1, HIGH);  
  r1 = time_base * sonic / 1000000;
  Serial.print("r1: ");
  Serial.println(r1, 3);
}

void getCoordX()
{
  sendWave(1);    
  if(et_x.receiveData())
  {                 
    r2 = calc(1);
    Serial.print("r2: ");
    Serial.println(r2, 3);
    led();
   }  
}

void getCoordY()
{ 
  sendWave(2);  
  if(et_y.receiveData())
  {                 
    r3 = calc(2);
    Serial.print("r3: ");
    Serial.println(r3, 3);
    led();
   }      
}

void sendWave(int number)
{      
  float sync;
  switch (number)
  {
    case 1:
    sync = sync_x;
    break;
    
    case 2:
    sync = sync_y;
    break;
  }     
  digitalWrite(sync, HIGH);
  robotSync();
  digitalWrite(sync, LOW); 
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
  return distance = data * sonic / 1000000;
}

void trilateration()
{
  x = ( pow(r1, 2) - pow(r2, 2) + pow(quad, 2) ) / ( 2 * quad );
  Serial.print("x: ");
  Serial.println(x, 3);
  
  y = ( pow(r1, 2) - pow(r3, 2) + pow(quad, 2) ) / ( 2 * quad );
  Serial.print("y: ");
  Serial.println(y, 3);
  
  z = sqrt( pow(r1, 2) - pow(x, 2) - pow(y, 2) );
  Serial.print("z: ");
  Serial.println(z, 3);
}

void robotSync()
{
  Serial.println(123);
}

void receive(int numBytes) {}
