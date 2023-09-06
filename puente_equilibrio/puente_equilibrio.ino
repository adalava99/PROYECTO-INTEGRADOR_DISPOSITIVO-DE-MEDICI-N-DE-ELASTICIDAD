//Comprobación de estabilidad de puente

const int pinVa = A0;  // Puerto analógico para la señal Va
const int pinVb = A1;  // Puerto analógico para la señal Vb

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Leer voltajes Va y Vb
  int vaRaw = analogRead(pinVa);
  int vbRaw = analogRead(pinVb);

  // Convertir lecturas a voltajes
  float vaVoltage = map(vaRaw, 0, 1023, 0, 5000) / 1000.0;
  float vbVoltage = map(vbRaw, 0, 1023, 0, 5000) / 1000.0;

  // Calcular la diferencia entre Va y Vb
  float difference = vaVoltage - vbVoltage;

  // Mostrar resultados por comunicación serial
  Serial.print("Va: ");
  Serial.print(vaVoltage, 4);
  Serial.print(" V, Vb: ");
  Serial.print(vbVoltage, 4);
  Serial.print(" V, Diferencia: ");
  Serial.print(difference, 4);
  Serial.println(" V");

  delay(1000);  // Esperar un segundo antes de la próxima lectura
}
