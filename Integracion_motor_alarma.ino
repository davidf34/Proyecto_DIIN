/*
  Program to control LED (ON/OFF) from ESP32 using Serial Bluetooth
  by Daniel Carrasco -> https://www.electrosoftcloud.com/
*/
#include "BluetoothSerial.h"
#include "esp_adc_cal.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run 'make menuconfig' to and enable it
#endif
#define LED_CONECTADO 19
#define MOTOR_ON  4
#define MOTOR_OFF 5
#define Alarma 23 //Alarma y led

int band_quitar_alarma;
int band_on;
int band_off;
int band_rta;
int band_alarma;
int band_asegurar;
int band_desasegurar;
int band_return;
int band_afirmativo;
int band_preguntar;
int band_salida;
BluetoothSerial BT; 

void enunciado();
void enunciado_asegurar();
void quitar_alarma();
void salir();

void callback_function(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_START_EVT) {
    Serial.println("Inicializado ESP");
  }
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    band_alarma = 0;
    band_on = 1;
   /* band_alarma = 0;*/
    
      /*band_off=0;*/
    Serial.println("Cliente conectado");
    digitalWrite(Alarma, LOW);
    digitalWrite(LED_CONECTADO, HIGH);

    BT.println("¡Bienvenido al sistema de seguridad para bicicletas!");
    BT.println("¿Desea asegurar su bicicleta? (Si/No)");
    band_asegurar = 1;
    

  }
  if (event == ESP_SPP_CLOSE_EVT && band_on == 1) {
    band_alarma = 1;
    Serial.println("Cliente desconectado");
    digitalWrite(LED_CONECTADO, LOW);
    //digitalWrite(Alarma, HIGH);
  }
}
/*****************************************************************************************************/
void enunciado(){

  if(band_asegurar == 0){
        BT.println("¿Desea desasegurar su bicicleta? (Si/No)");  
        band_desasegurar = 1;
      }
  if(band_asegurar == 2){
        BT.println("¿Desea asegurar su bicicleta? (Si/No)");
        band_asegurar = 1;
      } 
}

void quitar_alarma(){
  
  if(band_quitar_alarma == 1){
    BT.println("¿Desea desactivar alarma? (Si/No)");
    char rta = BT.read();
    if (rta == 'S' || rta == 'N'){
      if (rta == 'S') {
        digitalWrite(Alarma, LOW);
      }
      if (rta == 'N') {
        band_alarma = 1;
      }
    }
  }
  
}

void salir(){
  if(band_salida == 1){
    BT.println("\n ¿Desea salir del sistema de seguridad?");
    band_salida =0;
  }
  
}
/*****************************************************************************************************/
void setup() {
  Serial.begin(9600); // Inicializando la conexión serial para debug
  BT.begin("ESP32_LED_Control"); // Nombre de tu Dispositivo Bluetooth y en modo esclavo
  Serial.println("El dispositivo Bluetooth está listo para emparejar");
  BT.register_callback(callback_function); // Registramos la función "callback_function" como función callback.
  pinMode (LED_CONECTADO, OUTPUT); // Cambia el PIN del led a OUTPUT
//  pinMode (LED_DESCONECTADO, OUTPUT);
  pinMode (MOTOR_ON, OUTPUT);
  pinMode (MOTOR_OFF, OUTPUT);
  pinMode (Alarma, OUTPUT);

}
void loop() {
  if(band_alarma == 1){
    digitalWrite(Alarma, HIGH);
  }
  
  if (band_off == 1) {

    digitalWrite(MOTOR_OFF, HIGH);
    delay(2000);
    digitalWrite(MOTOR_OFF, LOW);
    band_off = 0;
  }
  

  /*-------------------------------------------------------------------------------------------------------------------------------------------------------------*/
if(band_asegurar == 1) {
  
    char rta = BT.read();
    if (rta == 'S' || rta == 'N'){
      if (rta == 'S') {
        digitalWrite(MOTOR_ON, HIGH);
        delay(2000);
        digitalWrite(MOTOR_ON, LOW);  
        BT.println("\n Su bicicleta se encuentra actualmente asegurada1.");
        band_asegurar = 0;
        enunciado();
      }
      if (rta == 'N') {
        BT.println("\n ¡Peligro!, seguridad desactivada1.");
        band_asegurar = 2;
        enunciado();
      }
    }
  }


if(band_desasegurar == 1) {
  
    char rta = BT.read();
    if (rta == 'S' || rta == 'N'){
      if (rta == 'S') {
        digitalWrite(MOTOR_OFF, HIGH);
        delay(2000);
        digitalWrite(MOTOR_OFF, LOW);  
        BT.println("\n ¡Peligro!, seguridad desactivada2.");
        band_asegurar = 2;
        enunciado();
      }
      if (rta == 'N') {
        BT.println("\n Su bicicleta se encuentra actualmente asegurada2.");
        band_asegurar = 0;
        enunciado();
      }
    }
  }
  
  /******************************************************************************************************************/
  
  /*char rta = BT.read();
  if (rta == 'S' || rta == 'N'){
    band_salida = 1;
    salir();
    //char rta = BT.read();
    if(rta == 'S'){
      if(band_asegurar == 0){
        digitalWrite(MOTOR_OFF, HIGH);
        delay(2000);
        digitalWrite(MOTOR_OFF, LOW);  
        BT.println("\n Hasta luego.");
        exit(-1);
      }
      if(band_desasegurar == 2){
        BT.println("\n Hasta luego.");
        exit(-1);
      }
    }
    if(rta == 'N'){
    }
    
  }*/
}
