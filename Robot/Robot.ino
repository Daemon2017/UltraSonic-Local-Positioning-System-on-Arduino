#define LED 13

//Разъемы подключения приемника
#define client_T 11
#define client_E 12

int robotSync;

void setup() 
{
  Serial.begin(9600);
  
  pinMode(client_T, OUTPUT); 
  pinMode(client_E, INPUT); 
  
  pinMode(LED,OUTPUT);
}

void loop() 
{
  if(Serial.available())
  {
    robotSync = Serial.read();
    
    if (robotSync == 123)
    {
      digitalWrite(client_T, HIGH);
      delayMicroseconds(10);
      digitalWrite(client_T, LOW);
      
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(LED, LOW); 
    }
  }
}
