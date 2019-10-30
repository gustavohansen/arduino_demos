#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define PIN_LED 9
#define PIN_SENSOR_LUZ A0
#define PIN_SENSOR_HUMEADAD_TIERRA A1
#define VALOR_SECO 1023
#define VALOR_HUMEDO 0

// SSD1306 display I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int display_inicializado = 0;

void setup()
{
    Serial.begin (9600);
    
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_SENSOR_LUZ, INPUT);
    pinMode(PIN_SENSOR_HUMEADAD_TIERRA, INPUT);

    if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) // Dirección 0x3D para resolución 128x64
    {
        display_inicializado = 1;
    }
    else
    { 
        Serial.println(F("SSD1306 iniciación fallida"));
    }
}

void loop(){
  
  int luz = obtener_luz();

  int humedad = obtener_humedad_tierra();
    
  escribir_en_el_display(luz, humedad);
}

/*
 * Pendiente
Pasar la resistencia a lux
https://www.electronics-notes.com/articles/electronic_components/resistors/light-dependent-resistor-ldr.php
*/
int obtener_luz(){
  
  int valor = analogRead(PIN_SENSOR_LUZ);
  
  if (valor > 20)
  {
    digitalWrite(PIN_LED, LOW);
  }
  else{
    digitalWrite(PIN_LED, HIGH);
  }

  return valor;
}

/*
Obtenemos el valor del sensor
Lo transformamos a porcentaje con la funcion map
https://greensense.github.io/Blog/2017/02/17/Arduino-Soil-Moisture-Sensor-Calibration/
https://www.arduino.cc/reference/en/language/functions/math/map/
*/
int obtener_humedad_tierra(){
  
  int valor = analogRead(PIN_SENSOR_HUMEADAD_TIERRA);

  return map(valor, VALOR_SECO, VALOR_HUMEDO, 0, 100);
}

void escribir_en_el_display(int luz, int humedad)
{
    if (display_inicializado)
    {
        display.clearDisplay();

        display.setTextSize(2);
        display.setTextColor(WHITE);
        
        display.setCursor(0, 0);
        display.println("Luz: ");
        display.println(luz);
        display.println("Humedad:");
        display.println(humedad);
        display.print("%");
        display.display();
    }
    else 
    {
      Serial.println("Luz: ");
      Serial.print(luz);

      Serial.println("Humedad: ");
      Serial.print(humedad);
    }
    
    delay(500);
}
