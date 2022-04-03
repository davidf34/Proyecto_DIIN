/******************************************************************************************************************
 * TITULO: SMS desde Sim800L
 * AUTOR: Jhimmy Astoraque
 * DESCRIPCION: Sketch que permite el envío de un sms desde el SIM800L
 * CANAL YOUTUBE: https://www.youtube.com/c/jadsatv
 * © 2021
 * ****************************************************************************************************************/

#include <SoftwareSerial.h>

SoftwareSerial SerialComSim800(11, 12); // RX -> 11 , TX -> 12

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

  SerialComSim800.println("AT+CMGS=\"+573182587756\""); // Envía el sms al número especificado
  serialCheck();
  
  SerialComSim800.print("Hola desde SIM800L - JADSA");
  serialCheck();
  
  SerialComSim800.write(26);  // Convencion ^Z -> 'Ctrl+z' -> 26DEC (1AHEX)  ASCII -> SUB.
}

void loop()
{
}

void serialCheck()
{
  while (Serial.available()) 
  {
    SerialComSim800.write(Serial.read());
  }
  while(SerialComSim800.available()) 
  {
    Serial.write(SerialComSim800.read());
  }
  delay(700);
}
