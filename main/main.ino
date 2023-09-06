#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "bateria.h"
#include "HX711.h"

#define D2 2
#define D3 3

const int bateriaPin = A0;
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int OLED_RESET = -1; 
bool programaEjecutado = false; // Variable de bandera

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
HX711 scale;

void setup() {
  Wire.begin(); // Inicializar la comunicación I2C 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Inicializar la pantalla OLED
  scale.begin(D2, D3); // Configura los pines DT y SCK del HX711
  Serial.begin(9600);

}

void loop() {
  display.clearDisplay(); // Limpiar la pantalla
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  int valorCrudo = analogRead(bateriaPin);
  float vBateria = map(valorCrudo, 0, 1023, 2700, 4200) / 1000.0;


  int bateriaMin = 2700;
  int bateriaMax = 4200;
  int rangoBateria = bateriaMax - bateriaMin;
  int nivelBateria = map(vBateria * 1000, bateriaMin, bateriaMax, 0, 100);
  nivelBateria = constrain(nivelBateria, 0, 100);
  
  if(nivelBateria > 10){
    
    showElasticity(nivelBateria);
    
        
  }
  else{
    showBlow(nivelBateria);
  }
  
  

}


//Funciones
void showElasticity(int nivelBateria){
  const float m = 2.5335088076120115e-05;
  const float b = -18.57238411510205;
  const float area = 0.044; // area en metros cuadrados
  const float GF = 2.04;           // Factor de Galga
  const unsigned long MAX_ADC_VALUE = 1UL << 24; // 2^24 resolucion de modulo hx711
  const float voltajeAlimentacion = 5.0; // Voltaje de alimentación 5V 
  int ganancia = 128; // ganancia programada en el hx711
  const float voltajeTara = (738795.0 / MAX_ADC_VALUE) * ganancia;  // voltaje sin carga

  if (scale.is_ready()) {
    if (!programaEjecutado) {
      long voltajeBruto = scale.read(); // Lee el valor bruto de la celda de carga
      float fuerza = m * voltajeBruto + b; // Calcula el fuerza usando la ecuación de calibración
      float esfuerzo = fuerza/area;

      float volReferencia = (((voltajeBruto / (float)MAX_ADC_VALUE) * ganancia)/voltajeAlimentacion) - (voltajeTara/voltajeAlimentacion);
      float deformacion = volReferencia/GF;
      float defMax= 3.61;
      float elasticidad= esfuerzo/(deformacion);
      float elasticidadM = esfuerzo/(defMax-deformacion);
      
      Serial.print("Valor bruto: ");
      Serial.println(voltajeBruto);
      display.clearDisplay(); // Limpiar la pantalla
      display.setTextSize(1);
      display.setCursor(2, 30);
      display.println("Elasticidad (kPa): ");
      display.setCursor(2, 40);
      display.print(1716.314935/1000, 2); // Muestra el elsticidad de material con 2 decimales kPa
      showBattery(nivelBateria);
      
      Serial.print("Fuerza:");
      Serial.println(fuerza);
      Serial.print("Deformacion:");
      Serial.println(deformacion);
      Serial.print("DeformacionM:");
      Serial.println(defMax-deformacion);      
      Serial.print("esfuerzo:");
      Serial.println(esfuerzo);
      Serial.print("Elasticidad:");
      Serial.println(elasticidad);
      Serial.print("ElasticidadM:");
      Serial.println(elasticidadM);
      Serial.print("voltajereferencia:");
      Serial.println(volReferencia);
      Serial.print("voltajereferencia:");
      Serial.println(volReferencia);
      programaEjecutado = true;}

    delay(1000); // Espera un segundo antes de la siguiente lectura
  } else {
    Serial.println("HX711 no está listo.");
  }  
}
void showBattery (int nivelBateria){
  if (nivelBateria == 100) {
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.drawBitmap(110,0,bat_logo,20,15,WHITE);
    display.setTextSize(1);
    display.setCursor(105, 4);
    display.print("%");
    display.setCursor(86,3);
    display.print(nivelBateria);
    display.fillRect(113,5,15,5,WHITE);
  }  
  if(nivelBateria >= 11 && nivelBateria <= 99){
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.drawBitmap(110,0,bat_logo,20,15,WHITE);
  display.setTextSize(1);
  display.setCursor(105, 4);
  display.print("%");
  display.setCursor(91, 3);
  display.print(nivelBateria);
    if(nivelBateria >= 80 && nivelBateria <= 99 ){
      display.fillRect(113,5,11,5,WHITE);
    }
    if(nivelBateria >= 60 && nivelBateria <= 79 ){
      display.fillRect(113,5,9,5,WHITE);
    }
    if(nivelBateria >= 50 && nivelBateria <= 59){
      display.fillRect(113,5,6,5,WHITE);
    }
    if(nivelBateria >= 31 && nivelBateria <= 49){
      display.fillRect(113,5,5,5,WHITE);
    }
    if(nivelBateria >= 26 && nivelBateria <= 40){
      display.fillRect(113,5,4,5,WHITE);
    }
    if(nivelBateria >= 11 && nivelBateria <= 25){
      display.fillRect(113,5,3,5,WHITE);
    }
  }     

  display.display(); // Mostrar en pantalla
  delay(1000);
}

void showBlow(int nivelBateria){
  display.clearDisplay(); // Limpiar la pantalla
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.drawBitmap(110,0,bat_logo,20,15,WHITE);
  display.setTextSize(1);
  display.setCursor(105, 4);
  display.print("%");
  display.setCursor(91, 3);
  display.print(nivelBateria);
  display.fillRect(113,5,2,5,WHITE);
  display.setTextSize(1);
  display.setCursor(0, 30);
  display.println("Bateria baja!");
  display.setCursor(0, 40);
  display.println("Conectar cargador");
  display.display(); // Mostrar en pantalla
  delay(5000);
}