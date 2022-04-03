
/**********************************************************
 * TITULO: Lectura de SMS entrante
 * AUTOR: Jhimmy Astoraque Durán
 * DESCRIPCION: ...
 * CANAL YOUTUBE: https://www.youtube.com/c/jadsatv
 * © 2021
 * ********************************************************/
 
#include <SoftwareSerial.h>

SoftwareSerial SerialComSim800(9, 8); // RX -> 11 , TX -> 12

void setup()
{
  Serial.begin(9600);
  SerialComSim800.begin(9600);

  Serial.println("Iniciando...");
  delay(2000);

  SerialComSim800.println("AT"); //check OK
  serialCheck();

  SerialComSim800.println("AT+CMGF=1"); // Formato SMS texto
  serialCheck();

  SerialComSim800.println("AT+CNMI=1,2,0,0,0"); // Manejo de nuevos mensajes sms entrantes
  serialCheck();
}

void loop()
{
  serialCheck();
}

void serialCheck()
{
  delay(500);
  while (Serial.available())
  {
    SerialComSim800.write(Serial.read()); 
  }
  while (SerialComSim800.available())
  {
    Serial.write(SerialComSim800.read());
  }  
}
  
