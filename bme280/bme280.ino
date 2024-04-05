#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA 1013.25  //Se indica la presión a nivel del mar en hPa
#define BME280_ADDRESS 0x76 //Se indica la dirección I2C del sensor
#define LED 32 //Se indica el pin para el LED indicador
#define BUZZ 33 //Se indica el pin para el Buzzer indicador

Adafruit_BME280 bme;  //Se crea el objeto BME280

//Prototitpo de funciones
void bme_verify();
void bme_read();

//Variables
float T, P, P0, h, hreal = 0.0;
int H = 0;

void setup() {
  //Inicialización de la terminal serie
  Serial.begin(9600);
  while(!Serial);

  //Salidas
  pinMode(LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  //Verificación del sensor BME280
  bme_verify();

}

void loop() {
  bme_read();
  digitalWrite(LED, HIGH);
  digitalWrite(BUZZ, LOW);

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
  Serial.print(" m, ");
  Serial.print(hreal);
  Serial.println(" m");

  delay(100);
  digitalWrite(LED, LOW);
  digitalWrite(BUZZ, HIGH);
  delay(100);
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
  int aux=0;
  float decimales = 0.0;
  
  P = bme.readPressure()/100;   //Se lee la presión [hPa]
  T = bme.readTemperature();    //Se lee la temperatura [°C]
  H = bme.readHumidity();       //Se lee la humedad relativa [%]
  hreal = bme.readAltitude(P0);     //Se calcula la altitud [m]
  aux = int(hreal);
  decimales = hreal - aux;
  if(decimales <= 0.8){
    h = float(aux);
  }
  else{
    h = hreal;
  }
}
