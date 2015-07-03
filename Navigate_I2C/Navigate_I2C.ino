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

float sonic;
float distance_base;
float z;

void setup() 
{
  Serial.begin(115200);
  
  Wire.begin(8);
  
  et_x.begin(details(data_x), &Wire);
  et_y.begin(details(data_y), &Wire);
  et_z.begin(details(data_z), &Wire);

  Wire.onReceive(receive);
  
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
  getCoordZ();  
  getCoordX();
  getCoordY();
}

void sendWave()
{
  delayMicroseconds(10);         
  digitalWrite(client_T, HIGH);
  delayMicroseconds(10);
  digitalWrite(client_T, LOW);   
}

void led()
{
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW); 
}

void getTemp()
{
  float temperatureC = (( ( (analogRead(A1) * 5.0) / 1024.0) - 0.55) * 100);  
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
  digitalWrite(client_T, HIGH);
  digitalWrite(sputnic_T_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(client_T, LOW);
  digitalWrite(sputnic_T_1, LOW);
  float time_base = pulseIn(sputnic_E_1, HIGH);  
  distance_base = time_base * sonic / 1000000;
  
  Serial.println(" ");  
  Serial.print("time_base: ");
  Serial.print(time_base, 0);
  Serial.println(" "); 
}

void getCoordX()
{
  delay(500);         
  digitalWrite(sync_x, HIGH); 
  sendWave();   
  digitalWrite(sync_x, LOW);
  
  if(et_x.receiveData())
  {                 
    Serial.print("time_x: ");
    Serial.print(data_x.time_x, 0);
    Serial.println(" ");  
    
    float quad_x = 0.44;     
    float distance = data_x.time_x * sonic / 1000000;
    float Sp = (quad_x + distance_base + distance) / 2;
    float Ar = sqrt(Sp * (Sp - quad_x) * (Sp - distance_base) * (Sp - distance));
    float x = Ar / (quad_x / 2);
    float true_x = sqrt(pow(x, 2) - pow(z, 2));
            
    Serial.print("Z: ");
    Serial.print(true_x, 3);
    Serial.println(" ");    
    
    led();
   }  
}

void getCoordY()
{
  delay(500);         
  digitalWrite(sync_y, HIGH); 
  sendWave();
  digitalWrite(sync_y, LOW);
  
  if(et_y.receiveData())
  {                 
    Serial.print("time_y: ");
    Serial.print(data_y.time_y, 0);
    Serial.println(" ");  
         
    float quad_y = 0.44;  
    float distance = data_y.time_y * sonic / 1000000;
    float Sp = (quad_y + distance_base + distance) / 2;
    float Ar = sqrt(Sp * (Sp - quad_y) * (Sp - distance_base) * (Sp - distance));
    float y = Ar / (quad_y / 2);
    float true_y = sqrt(pow(y, 2) - pow(z, 2));
            
    Serial.print("X: ");
    Serial.print(true_y, 3);
    Serial.println(" ");  
  
    led();
   }      
}

void getCoordZ()
{
  delay(500);         
  digitalWrite(sync_z, HIGH); 
  sendWave(); 
  digitalWrite(sync_z, LOW);
  
  if(et_z.receiveData())
  {                    
    Serial.print("time_z: ");
    Serial.print(data_z.time_z, 0);
    Serial.println(" ");  
         
    float quad_z = 0.50;       
    float distance = data_z.time_z * sonic / 1000000;
    float Sp = (quad_z + distance_base + distance) / 2;
    float Ar = sqrt(Sp * (Sp - quad_z) * (Sp - distance_base) * (Sp - distance));
    z = Ar / (quad_z / 2);
            
    Serial.print("Y: ");
    Serial.print(z, 3);
    Serial.println(" ");  
  
    led();
   }     
}

void receive(int numBytes) {}
