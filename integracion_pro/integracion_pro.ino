/*
  Program to control LED (ON/OFF) from ESP32 using Serial Bluetooth
  by Daniel Carrasco -> https://www.electrosoftcloud.com/
*/
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include "BluetoothSerial.h"
#include "esp_adc_cal.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run 'make menuconfig' to and enable it
#endif
#define LED_CONECTADO 19
#define MOTOR_ON  4
#define MOTOR_OFF 5
#define Alarma 23 //Alarma y led 

/**********************************GPS********************************************/



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//On ESP32: GPIO-21(SDA), GPIO-22(SCL)
#define OLED_RESET -1 //Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C //See datasheet for Address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define RXD2 16
#define TXD2 17
HardwareSerial neogps(1);


/*********************************************************************************/


int band_quitar_alarma;//si
int band_on;//si
int band_off;//si
int band_alarma;//si
int band_asegurar; //si
int band_desasegurar;//si
int band_salida;//si revisaaaar
BluetoothSerial BT; 
TinyGPSPlus gps;


void print_speed();

void callback_function(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_START_EVT) {
    Serial.println("Inicializado ESP");
  }
  if (event == ESP_SPP_SRV_OPEN_EVT) {
   
    Serial.println("Cliente conectado");
    //digitalWrite(Alarma, LOW);
    digitalWrite(LED_CONECTADO, HIGH);
    band_asegurar = 1;
    

  }
  if (event == ESP_SPP_CLOSE_EVT && band_salida !=1) {
    band_alarma = 1;
    Serial.println("Cliente desconectado");
    digitalWrite(LED_CONECTADO, LOW);
    //digitalWrite(Alarma, HIGH);
  }
}
/*************************************FUNCIONES FUERA DE LOS MODULOS*************************************************/

  

/*****************************************************************************************************/
void setup() {

  /****************************************SETUP GPS***********************************/

  Serial.begin(115200);
  //Begin serial communication Arduino IDE (Serial Monitor)

  //Begin serial communication Neo6mGPS
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.display();
  delay(2000);

  /***************************************************************************/
  Serial.begin(9600); // Inicializando la conexión serial para debug
  BT.begin("ESP32_LED_Control_PRO"); // Nombre de tu Dispositivo Bluetooth y en modo esclavo
  Serial.println("El dispositivo Bluetooth está listo para emparejar");
  BT.register_callback(callback_function); // Registramos la función "callback_function" como función callback.
  pinMode (LED_CONECTADO, OUTPUT); // Cambia el PIN del led a OUTPUT
//  pinMode (LED_DESCONECTADO, OUTPUT);
  pinMode (MOTOR_ON, OUTPUT);
  pinMode (MOTOR_OFF, OUTPUT);
  pinMode (Alarma, OUTPUT);

}
void loop() {
  
  /****************************************LOOP GPS***********************************/


  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (neogps.available())
    {
      if (gps.encode(neogps.read()))
      {
        newData = true;
      }
    }
  }

  //If newData is true
  if(newData == true)
  {
    newData = false;
    Serial.println(gps.satellites.value());
    print_speed();
  }
  else
  {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.setTextSize(3);
    display.print("No Data");
    display.display();
  } 
  
  /*-------------------------------------------------------------------------------------------------------------------------------------------------------------*/

  if(band_alarma == 1)
    digitalWrite(Alarma, HIGH);
  
  

  /*-------------------------------------------------------------------------------------------------------------------------------------------------------------*/
if(band_asegurar == 1) {
  
    char rta = BT.read();
    if (rta == 'S' || rta == 'N' || rta == 'E'){
      if (rta == 'S') {
        digitalWrite(MOTOR_ON, HIGH);
        delay(2000);
        digitalWrite(MOTOR_ON, LOW);  
      }
      if (rta == 'N') {
        digitalWrite(MOTOR_OFF, HIGH);
        delay(2000);
        digitalWrite(MOTOR_OFF, LOW);  
      }
      
      if(rta == 'E'){
        band_salida = 1;
        exit(-1);
    }
  }
 }
}


void print_speed(){
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
       
  if (gps.location.isValid() == 1)
  {
   //String gps_speed = String(gps.speed.kmph());
    display.setTextSize(1);
    
    display.setCursor(25, 5);
    display.print("Lat: ");
    display.setCursor(50, 5);
    display.print(gps.location.lat(),6);

    display.setCursor(25, 20);
    display.print("Lng: ");
    display.setCursor(50, 20);
    display.print(gps.location.lng(),6);

    display.setCursor(25, 35);
    display.print("Speed: ");
    display.setCursor(65, 35);
    display.print(gps.speed.kmph());
    
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print("SAT:");
    display.setCursor(25, 50);
    display.print(gps.satellites.value());

    display.setTextSize(1);
    display.setCursor(70, 50);
    display.print("ALT:");
    display.setCursor(95, 50);
    display.print(gps.altitude.meters(), 0);

    display.display();
    
  }
  else
  {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.setTextSize(3);
    display.print("No Data");
    display.display();
  }  

}
