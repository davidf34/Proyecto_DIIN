#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
#define LED 23
#define LED_OFF 19
int band=20;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//On ESP32: GPIO-21(SDA), GPIO-22(SCL)
#define OLED_RESET -1 //Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C //See datasheet for Address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define RXD2 16
#define TXD2 17
HardwareSerial neogps(1);
TinyGPSPlus gps;

BluetoothSerial BT; // Objeto Bluetooth
void callback_function(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
 /* if (event == ESP_SPP_START_EVT) {
    Serial.println("Inicializado SPP");
  }*/
 if (event == ESP_SPP_SRV_OPEN_EVT ) {
    Serial.println("Cliente conectado");
    BT.println("1");
    digitalWrite(LED,HIGH);
    delay(1000);
    digitalWrite(LED,LOW);
    delay(500);
    digitalWrite(LED,HIGH);
    delay(1000);
    digitalWrite(LED,LOW);
    band=0;
//    
//    while(band==0){
//      digitalWrite(LED,HIGH);
//      delay(1000);
//      digitalWrite(LED,LOW);
//      delay(1000);
//      esp_spp_cb_event_t event;
//      band2=1;
//      if(event == ESP_SPP_CLOSE_EVT){
//        band=1;
//        Serial.println("ya no Valimos VERGA");
//        delay(1000);
//        }  
//        Serial.println("Valimos BERGA");
//    }
    
  }
  else if (event == ESP_SPP_CLOSE_EVT  ) {
    Serial.println("Cliente desconectado");
    digitalWrite(LED_OFF,HIGH);
    delay(3000);
    digitalWrite(LED_OFF,LOW);
    delay(500);
    digitalWrite(LED_OFF,HIGH);
    delay(3000);
    digitalWrite(LED_OFF,LOW);
    band=1; 
  }
//    while(band==1){
//    digitalWrite(LED_OFF,HIGH);
//    delay(1000);                      //Estoy desconectado
//    digitalWrite(LED_OFF,LOW);
//    if (event == ESP_SPP_SRV_OPEN_EVT  ) {
//      band=0;
//      }
//    }
//    while(band==0){
//      digitalWrite(LED,HIGH);
//      delay(500);
//      digitalWrite(LED,LOW);
//      if (event == ESP_SPP_CLOSE_EVT  ) {
//        band=1;
//        }
//    }
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
  pinMode (LED, OUTPUT); // Cambia el PIN del led a OUTPUT
  pinMode (LED_OFF, OUTPUT);
  
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

}

void loop() {
  esp_spp_cb_event_t event;  
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
  
}

void print_speed()
{
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
