/*
  Program to control LED (ON/OFF) from ESP32 using Serial Bluetooth
  by Daniel Carrasco -> https://www.electrosoftcloud.com/
*/
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
#define LED_CONECTADO 23
#define LED_DESCONECTADO 19
#define MOTOR_ON  10
#define MOTOR_OFF 11
#define Alarma 2


int band_on;
int band_off;
int band_rta;
BluetoothSerial BT; // Objeto Bluetooth


void callback_function(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
if (event == ESP_SPP_START_EVT) {
    Serial.println("Inicializado ESP");
  }
if (event == ESP_SPP_SRV_OPEN_EVT ) {
        int rta;
        band_on = 1;
        band_off=0;
        Serial.println("Cliente conectado");
        BT.println("1");
        digitalWrite(LED_DESCONECTADO,LOW);
        digitalWrite(LED_CONECTADO,HIGH);
        
        BT.println("¡Bienvenido al sistema de seguridad para bicicletas!");
        BT.println("¿Desea asegurar su bicicleta? (Si/No)");
        rta = BT.read(); 
        if(rta == "Si"){
          band_on=1;
        }
        else if(rta == "No"){
         // band_off=1;
          band_rta =1;
        }
        else{
           BT.println("Por favor ingrese una respuesta válida.");
        }
        
        } 

else if (event == ESP_SPP_CLOSE_EVT && band_on==1){
        band_alarma = 1;
        
        Serial.println("Cliente desconectado");
        BT.println("0");
        digitalWrite(LED_CONECTADO,LOW);
        digitalWrite(LED_DESCONECTADO,HIGH);
        }
}

  /*else if (event == ESP_SPP_DATA_IND_EVT ) {
    Serial.println("Datos recibidos");
    /*while (BT.available()) { // Mientras haya datos por recibir
      int incoming = BT.read(); // Lee un byte de los datos recibidos
      Serial.print("Recibido: ");
      Serial.println(incoming);
      if (incoming == 49) { // 1 en ASCII
        digitalWrite(LED, HIGH); // Encender el LED
        BT.println("LED encendido"); // Envía el texto a través del puerto Serial del BT
      }
      else if (incoming == 48) { // 0 en ASCII
        digitalWrite(LED, LOW); // Apagar el LED
        BT.println("LED apagado"); // Envía el texto a través del puerto Serial del BT
      }
    }
  }*/

void setup() {
  Serial.begin(115200); // Inicializando la conexión serial para debug
  BT.begin("ESP32_LED_Control"); // Nombre de tu Dispositivo Bluetooth y en modo esclavo
  Serial.println("El dispositivo Bluetooth está listo para emparejar");
  BT.register_callback(callback_function); // Registramos la función "callback_function" como función callback.
  pinMode (LED_CONECTADO, OUTPUT); // Cambia el PIN del led a OUTPUT
  pinMode (LED_DESCONECTADO, OUTPUT);
  pinMode (MOTOR_ON, OUTPUT);
  pinMode (MOTOR_OFF, OUTPUT);
  pinMode (Alarma, OUTPUT);
  
}
void loop() {
  //PARA ENROLLAR EL MOTOR
  if (band_off == 1){
    
    digitalWrite(MOTOR_ON,HIGH);
    delay(2000);//Tiempo en el que se va  a enrrollar el motor, hay que hacer pruebas para saber cuanto tiempo se puede estimar
    digitalWrite(MOTOR_ON,LOW);
  }
  //PARA DESENRROLLAR EL MOTOR
  else if(band_on == 1){
    digitalWrite(MOTOR_OFF, HIGH);
    delay(2000);
    digitalWrite(MOTOR_OFF,LOW);
    
  }
  else if(band_rta == 1){
    BT.println("¡Peligro!, seguridad desactivada.");
  }
  else if(band_alarma == 1){
     digitalWrite(Alarma,HIGH);
  }
  
  
}
