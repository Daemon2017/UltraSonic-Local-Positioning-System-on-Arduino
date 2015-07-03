#include <Wire.h>
#include <EasyTransferI2C.h>

//Для связи с модулем Х-координаты
EasyTransferI2C ET_X; 
struct RECEIVE_DATA_STRUCTURE_X
{
  float time_x;
};
RECEIVE_DATA_STRUCTURE_X data_x;

//Для связи с модулем Y-координаты
EasyTransferI2C ET_Y; 
struct RECEIVE_DATA_STRUCTURE_Y
{
  float time_y;
};
RECEIVE_DATA_STRUCTURE_Y data_y;

//Для связи с модулем Z-координаты
EasyTransferI2C ET_Z; 
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

float quad_x, quad_y, quad_z;

void setup() 
{
  Serial.begin(115200);
  
  Wire.begin(8);
  
  ET_X.begin(details(data_x), &Wire);
  ET_Y.begin(details(data_y), &Wire);
  ET_Z.begin(details(data_z), &Wire);

  Wire.onReceive(receive);
  
  quad_x = 0.44; //Сторона квадрата
  quad_y = 0.44;
  quad_z = 0.50;
  
  pinMode(sputnic_T_1, OUTPUT); 
  pinMode(sputnic_E_1, INPUT); 

  pinMode(client_T, OUTPUT); 
  pinMode(client_E, INPUT); 
  
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
  pinMode(13, OUTPUT);
}

void loop() 
{        
  float temperatureC = (( ( (analogRead(A1) * 5.0) / 1024.0) - 0.55) * 100);  
  Serial.println(" "); 
  Serial.print("Degrees: ");
  Serial.print(temperatureC); 
    
  float sonic = sqrt(1.4 * 287 * (temperatureC + 273.15));
  Serial.println(" "); 
  Serial.print("Sonic speed: ");
  Serial.print(sonic); 
//-----------------------------  
  digitalWrite(client_T, HIGH);
  digitalWrite(sputnic_T_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(client_T, LOW);
  digitalWrite(sputnic_T_1, LOW);
  float time_base = pulseIn(sputnic_E_1, HIGH);  
  float distance_base = time_base * sonic / 1000000;
  
  Serial.println(" ");  
  Serial.print("time_base: ");
  Serial.print(time_base, 0);
  Serial.println(" "); 
//-----------------------------
  delay(500);       
  
  float z;
  
  digitalWrite(9, HIGH); 
  delayMicroseconds(10);         
  digitalWrite(client_T, HIGH);
  delayMicroseconds(10);
  digitalWrite(client_T, LOW);    
  digitalWrite(9, LOW);

  if(ET_Z.receiveData())
  {                    
    Serial.print("time_z: ");
    Serial.print(data_z.time_z, 0);
    Serial.println(" ");  
         
    float distance_z = data_z.time_z * sonic / 1000000;
    float Sp_z = (quad_z + distance_base + distance_z) / 2;
    float Ar_z = sqrt(Sp_z * (Sp_z - quad_z) * (Sp_z - distance_base) * (Sp_z - distance_z));
    z = Ar_z / (quad_z / 2);
            
    Serial.print("Y: ");
    Serial.print(z, 3);
    Serial.println(" ");  
  
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);  
   }     
//----------------------------- 
  delay(500);       
  
  digitalWrite(7, HIGH); 
  delayMicroseconds(10);         
  digitalWrite(client_T, HIGH);
  delayMicroseconds(10);
  digitalWrite(client_T, LOW);    
  digitalWrite(7, LOW);

  if(ET_X.receiveData())
  {                 
    Serial.print("time_x: ");
    Serial.print(data_x.time_x, 0);
    Serial.println(" ");  
         
    float distance_x = data_x.time_x * sonic / 1000000;
    float Sp_x = (quad_x + distance_base + distance_x) / 2;
    float Ar_x = sqrt(Sp_x * (Sp_x - quad_x) * (Sp_x - distance_base) * (Sp_x - distance_x));
    float x = Ar_x / (quad_x / 2);
    float x_true = sqrt(pow(x, 2) - pow(z, 2));
            
    Serial.print("Z: ");
    Serial.print(x_true, 3);
    Serial.println(" ");    
    
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
   }  
//-----------------------------
  delay(500);       
  
  digitalWrite(8, HIGH); 
  delayMicroseconds(10);         
  digitalWrite(client_T, HIGH);
  delayMicroseconds(10);
  digitalWrite(client_T, LOW);    
  digitalWrite(8, LOW);

  if(ET_Y.receiveData())
  {                 
    Serial.print("time_y: ");
    Serial.print(data_y.time_y, 0);
    Serial.println(" ");  
         
    float distance_y = data_y.time_y * sonic / 1000000;
    float Sp_y = (quad_y + distance_base + distance_y) / 2;
    float Ar_y = sqrt(Sp_y * (Sp_y - quad_y) * (Sp_y - distance_base) * (Sp_y - distance_y));
    float y = Ar_y / (quad_y / 2);
    float y_true = sqrt(pow(y, 2) - pow(z, 2));
            
    Serial.print("X: ");
    Serial.print(y_true, 3);
    Serial.println(" ");  
  
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);  
   }         
}

void receive(int numBytes) {}
