#define LED 13

//Разъемы подключения приемника
#define client_T 11
#define client_E 12

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
    int robotSync = Serial.read();
    
    if (robotSync == 'V')
    {   
      digitalWrite(client_T, HIGH);
      digitalWrite(LED, HIGH);
      delayMicroseconds(10);
      digitalWrite(client_T, LOW);
      digitalWrite(LED, LOW); 
    }
  }
}
