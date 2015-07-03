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

//Разъемы подключения спутника
#define sputnic_T_1 3
#define sputnic_E_1 2

//Разъемы подключения приемника
#define client_T 11
#define client_E 12

float quad;
float time_base, time_x, time_y;
float x, y;
float sonic;
float distance_base;
int temperatureC;

#define I2C_SLAVE_ADDRESS2 8 //I2C адрес спутника-базы
#define I2C_SLAVE_ADDRESS 9 //I2C адрес спутника X
#define I2C_SLAVE_ADDRESS3 10 //I2C адрес спутника Y

void setup() 
{
  Serial.begin(115200);
  Wire.begin(I2C_SLAVE_ADDRESS2);
  
  ET_X.begin(details(data_x), &Wire);
  ET_Y.begin(details(data_y), &Wire);

  Wire.onReceive(receive);
  
  temperatureC;
  quad = 0.44; //Сторона квадрата
  
  pinMode(sputnic_T_1, OUTPUT); 
  pinMode(sputnic_E_1, INPUT); 

  pinMode(client_T, OUTPUT); 
  pinMode(client_E, INPUT); 
  
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  
  pinMode(13, OUTPUT);
}

void loop() 
{      
  delay(250);  
  
  temperatureC = (( ( (analogRead(A1) * 5.0) / 1024.0) - 0.55) * 100);  
  Serial.println(" "); 
  Serial.print("Degrees: ");
  Serial.print(temperatureC); 
    
  sonic=sqrt(1.4 * 287 * (temperatureC + 273.15));
  Serial.println(" "); 
  Serial.print("Sonic speed: ");
  Serial.print(sonic); 
  
  delay(250);  
  digitalWrite(client_T, HIGH);
  digitalWrite(sputnic_T_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(client_T, LOW);
  digitalWrite(sputnic_T_1, LOW);
  time_base = pulseIn(sputnic_E_1, HIGH);  
  distance_base = time_base * sonic / 1000000;
  
  Serial.println(" ");  
  Serial.print("time_base: ");
  Serial.print(time_base, 0);
  Serial.println(" "); 
  
  delay(250);       
  digitalWrite(7, HIGH); 
  delayMicroseconds(10);         
  digitalWrite(client_T, HIGH);
  delayMicroseconds(10);
  digitalWrite(client_T, LOW);    
  digitalWrite(7, LOW);

  if(ET_X.receiveData())
  {      
    time_x = data_x.time_x;
            
    Serial.print("time_x: ");
    Serial.print(time_x, 0);
    Serial.println(" ");  
         
    float distance_x = time_x * sonic / 1000000;
    float Sp = (quad + distance_base + distance_x) / 2;
    float Ar = sqrt(Sp * (Sp - quad) * (Sp - distance_base) * (Sp - distance_x));
    float x = Ar / (quad / 2);
            
    Serial.print("x: ");
    Serial.print(x, 3);
    Serial.println(" ");    
    
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
   }  
   
  delay(250);       
  digitalWrite(8, HIGH); 
  delayMicroseconds(10);         
  digitalWrite(client_T, HIGH);
  delayMicroseconds(10);
  digitalWrite(client_T, LOW);    
  digitalWrite(8, LOW);

  if(ET_Y.receiveData())
  {      
    time_y = data_y.time_y;
            
    Serial.print("time_y: ");
    Serial.print(time_y, 0);
    Serial.println(" ");  
         
    float distance_y = time_y * sonic / 1000000;
    float Sp = (quad + distance_base + distance_y) / 2;
    float Ar = sqrt(Sp * (Sp - quad) * (Sp - distance_base) * (Sp - distance_y));
    float y = Ar / (quad / 2);
            
    Serial.print("y: ");
    Serial.print(y, 3);
    Serial.println(" ");  
  
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);  
   }     
}

void receive(int numBytes) {}
