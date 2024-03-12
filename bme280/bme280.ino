#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA 1013.25  //Se indica la presión a nivel del mar en hPa
#define BME280_ADDRESS 0x76 //Se indica la dirección I2C del sensor

Adafruit_BME280 bme;  //Se crea el objeto BME280

//Prototitpo de funciones
void bme_verify();
void bme_read();

//Variables
float T, P, P0, h = 0.0;
int H = 0;

void setup() {
  //Inicialización de la terminal serie
  Serial.begin(9600);
  while(!Serial);

  //Verificación del sensor BME280
  bme_verify();

}

void loop() {
  bme_read();

  Serial.print("Presion: ");
  Serial.print(P);
  Serial.print(" hPa ");
  
  Serial.print("Temperatura: ");
  Serial.print(T);
  Serial.print(" C ");
  
  Serial.print("Humedad: ");
  Serial.print(H);
  Serial.print(" % ");
  
  Serial.print("Altitud: ");
  Serial.print(h);
  Serial.println(" m");

  delay(5000);
}

void bme_verify(){
  //Si el sensor no es detecatado
  if (!bme.begin(BME280_ADDRESS )) {
    Serial.println("S01");
  }
  else{
    P0=bme.readPressure()/100; //Se lee la presión de referencia [hPa]
    Serial.println("Online");
    Serial.print(P0);
    Serial.println(" hPa");
    delay(1000);
  }
}

void bme_read(){
  P = bme.readPressure()/100;   //Se lee la presión [hPa]
  T = bme.readTemperature();    //Se lee la temperatura [°C]
  H = bme.readHumidity();       //Se lee la humedad relativa [%]
  h = bme.readAltitude(P0);     //Se calcula la altitud [m]
}
