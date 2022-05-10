#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define ledTX 21
#define ledRX 15



void setup() {
  Serial.begin (115200);
  pinMode (ledTX, OUTPUT);
  pinMode (ledRX, OUTPUT);
  SerialBT.begin ("BlueTooh ESP32");

}

void loop() {
  
  if(Serial.available()){//esto funciona con el puerto serial
    SerialBT.write(SerialBT.print("1"));
    digitalWrite(ledTX,HIGH);
    delay(200);
    digitalWrite(ledTX,LOW);
  }

  else{
      //SerialBT.write(SerialBT.print("1"));
      SerialBT.print("0");
      digitalWrite(ledRX,HIGH);
      delay(200);
      digitalWrite(ledRX,LOW);
  }
 
  delay(20);
}
