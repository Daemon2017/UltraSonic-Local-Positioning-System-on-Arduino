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

//Для связи с модулем Z-координаты
EasyTransferI2C et_z; 
struct RECEIVE_DATA_STRUCTURE_Z
{
  float time_z;
};
RECEIVE_DATA_STRUCTURE_Z data_z;

//Разъемы подключения спутника
#define sputnic_T_1 3
#define sputnic_E_1 2

//Разъемы подключения приемника
#define client_T 11
#define client_E 12

//Разъемы синхросигналов
#define sync_x 7
#define sync_y 8
#define sync_z 9

float sonic, distance_base, coordinate, true_coord;
float x, y, z;

void setup() 
{
  Serial.begin(115200);  
  Wire.begin(8);
  Wire.onReceive(receive); 
  et_x.begin(details(data_x), &Wire);
  et_y.begin(details(data_y), &Wire);
  et_z.begin(details(data_z), &Wire);
  
  pinMode(sputnic_T_1, OUTPUT); 
  pinMode(sputnic_E_1, INPUT); 
  pinMode(client_T, OUTPUT); 
  pinMode(client_E, INPUT); 
  
  pinMode(sync_x, OUTPUT);
  pinMode(sync_y, OUTPUT);
  pinMode(sync_z, OUTPUT);
  
  pinMode(13, OUTPUT);
}

void loop() 
{        
  getTemp();
  getBase();
  getCoordY();
  getCoordZ();  
  getCoordX();
  delay (1500);
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
    
    case 3:
    sync = sync_z;
    break;
  }     
  delay(500);         
  digitalWrite(sync, HIGH); 
  delayMicroseconds(10);         
  getPriem();
  digitalWrite(sync, LOW); 
}

void led()
{
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW); 
}

void getTemp()
{
  float temperatureC = ((((analogRead(A1) * 5.0) / 1024.0) - 0.55) * 100);  
  Serial.println(" "); 
  Serial.print("Degrees: ");
  Serial.print(temperatureC); 
  
  sonic = sqrt(1.4 * 287 * (temperatureC + 273.15));
  Serial.println(" "); 
  Serial.print("Sonic speed: ");
  Serial.print(sonic); 
}

void getBase()
{
  digitalWrite(sputnic_T_1, HIGH);
  getPriem();
  digitalWrite(sputnic_T_1, LOW);
  float time_base = pulseIn(sputnic_E_1, HIGH);  
  distance_base = time_base * sonic / 1000000;
  Serial.println(" ");  
}

void getPriem()
{
  digitalWrite(client_T, HIGH);
  delayMicroseconds(10);
  digitalWrite(client_T, LOW);
}

void getCoordX()
{
  sendWave(1);    
  if(et_x.receiveData())
  {                 
    x = calc(1);
    Serial.print("Z: ");
    Serial.print(x, 3);
    Serial.println(" ");        
    led();
   }  
}

void getCoordY()
{ 
  sendWave(2);  
  if(et_y.receiveData())
  {                 
    y = calc(2);
    Serial.print("X: ");
    Serial.print(y, 3);
    Serial.println(" "); 
    trueCalc(1);
    Serial.print("X_true: ");
    Serial.print(true_coord, 3);
    Serial.println(" ");    
    led();
   }      
}

void getCoordZ()
{
  sendWave(3);   
  if(et_z.receiveData())
  {                    
    z = calc(3); 
    Serial.print("Y: ");
    Serial.print(z, 3);
    Serial.println(" ");      
    trueCalc(2);
    Serial.print("Y_true: ");
    Serial.print(true_coord, 3);
    Serial.println(" ");   
    led();
   }     
}

float calc(int peremennaja)
{
  float data, quad = 0.45;
  switch (peremennaja)
  {
    case 1:
    data = data_x.time_x;
    break;
    
    case 2:
    data = data_y.time_y;
    break;
    
    case 3:
    data = data_z.time_z;
    break;
  }   
  Serial.println(" ");    
  float distance = data * sonic / 1000000;
  float Sp = (quad + distance_base + distance) / 2;
  float Ar = sqrt(Sp * (Sp - quad) * (Sp - distance_base) * (Sp - distance));
  return coordinate = Ar / (quad / 2);
}

float trueCalc(int znachenie)
{
  float coordinata;
  switch (znachenie)
  {
    case 1:
    coordinata = y;
    break;
    
    case 2:
    coordinata = z;
    break;
  } 
  return true_coord = sqrt(pow(coordinata, 2) - pow(x, 2));   
}

void receive(int numBytes) {}
