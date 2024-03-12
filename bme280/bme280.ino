#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA 1013.25  //Se indica la presión a nivel del mar en hPa
#define BME280_ADDRESS 0x76 //Se indica la dirección I2C del sensor

Adafruit_BME280 bme;  //Se crea el objeto BME280

//Prototitpo de funciones
void bme_verify();

void setup() {
  //Inicialización de la terminal serie
  Serial.begin(9600);
  while(!Serial);

  //Verificación del sensor BME280
  bme_verify();

}

void loop() {
  // put your main code here, to run repeatedly:

}

void bme_verify(){
  //Si el sensor no es detecatado
  if (!bme.begin(BME280_ADDRESS )) {
    Serial.println("S01");
  }
  Serial.println("Online");
  Serial.println()
  delay(1000ms)
}
