/**********************************************************
 * TITULO: Comandos AT con arduino para SIM800L
 * AUTOR: Jhimmy Astoraque Durán
 * DESCRIPCION: ...
 * CANAL YOUTUBE: https://www.youtube.com/c/jadsatv
 * © 2021
 * ********************************************************/
 
#include <SoftwareSerial.h>

SoftwareSerial SerialComSim800(8, 9); // RX -> 11 , TX -> 12

void setup()
{
    // Iniciamos comunicacion Serial
    Serial.begin(9600);
    SerialComSim800.begin(9600);
    delay(2000);
}

void loop()
{
    while (Serial.available())
    {
        byte dato = Serial.read();
        SerialComSim800.write(dato);
    }

    while (SerialComSim800.available())
    {
        byte dato = SerialComSim800.read();
        Serial.write(dato);
    }
}
