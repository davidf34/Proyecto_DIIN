#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8); //RX || TX

char caracter; //Caracter que vamos a obtener

void setup() {
  Serial.begin(9600); //Establecemos la comunicación serial del monitor arduino
  SIM900.begin(9600); //Establecemos la comunicación serial del SIM900
  //Encender SIM900*****
  //digitalWrite(9,HIGH);
  //delay(1000);
  //digitalWrite(9,LOW);
  delay(1000);
  //********************
}

void loop() {
  
  if(SIM900.available()){ //Si hay caracteres
    caracter = SIM900.read(); //Leer caracteres
    Serial.print(caracter); //Mostrar los caracteres
  }
  if(Serial.available()){
    caracter =
