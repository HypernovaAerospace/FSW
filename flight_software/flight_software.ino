#include <Adafruit_MPU6050.h>//Librería para el MPU6050
#include <Adafruit_Sensor.h>//Librería para el MPU6050
#include <Wire.h>//Librería para el MPU6050
#include <Adafruit_BME280.h>
#include <SPI.h>    // incluye libreria interfaz SPI
#include <SD.h>     // incluye libreria para tarjetas SD

//Definiciones para el programa
#define SEALEVELPRESSURE_HPA 1013.25  //Se indica la presión a nivel del mar en hPa
#define BME280_ADDRESS 0x76 //Se indica la dirección I2C del sensor
#define LED 32 //Se indica el pin para el LED indicador
#define BUZZ 33 //Se indica el pin para el Buzzer indicador
#define SSpin 5    // Slave Select en pin digital 10
#define MATCH 15 //Cerillo electrónico

//Cración de objetos
Adafruit_BME280 bme;  //Se crea el objeto BME280
Adafruit_MPU6050 mpu; //Se crea el objeto MPU
File archivo;     // objeto archivo del tipo File

//Prototipo de funciones
float mpuRead();
void mpuInit();
void bme_verify();
float bme_read();
void sdInit();
void sdWrite(float mpu, float bme, String state);
void blinkChute();

//Variables
String datos_almacenados = "";
String datos_leidos = "";
String estado = "Plataforma";
float T, P, P0, h, hreal, h1, h2, ay = 0.0;
int H, aux, archivo_num, aux1, aux2 = 0;

//**************************************************** INICIALIZACIÓN DE PERIFÉRICOS ***********************************************************
void setup() {
  //Inicialización de la terminal serie
  Serial.begin(9600);
  while(!Serial);
  
  //Inicialización de sensores
  mpuInit();
  bme_verify();
  sdInit();

  //Salidas
  pinMode(LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(MATCH, OUTPUT);
}

//**************************************************** PROGRAMA PRINCIPAL *****************************************************************************
void loop() {
  digitalWrite(MATCH, LOW);
  //*********************************ESTADO DE PLATAFORMA (ESTADO INICIAL)
  if(estado == "Plataforma"){
    //Leer MPU6050, Blink LED y BUZZER y no almacenar en la SD
    Serial.println(estado);
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZ, HIGH);
    //Si la aceleración en Y es mayor o igual a 5G (49 m/s^2). cambiar a Vuelo
    if(mpuRead() >= 10.0){
      estado = "Vuelo";
    }
    delay(100);
    digitalWrite(LED, LOW);
    digitalWrite(BUZZ, LOW);
    aux1 ++;
    if(aux1 == 150){
      aux1 = 0;
      sdWrite(bme_read(),mpuRead(),estado);
    }
    delay(100);

  } 
  //**********************************ESTADO DE VUELO
  else if(estado == "Vuelo"){
  //Leer MPU6050, BMP280, almacenar en la SD
  digitalWrite(LED, LOW);
  digitalWrite(BUZZ, LOW);
  //Si la altura decrece cambiar a Apogeo
  h1 = bme_read();
  if(h2-h1 < 0){
    //Está en decremento
    while(h2-h1 < 0){
      h1 = bme_read();
      aux ++;
      delay(50);
      h2 = bme_read();
      if(aux == 40){
        estado = "Apogeo";
        blinkChute();
        break;
      }
      sdWrite(h2,mpuRead(),estado);
      delay(50);
    }
  }
  delay(50);
  h2 = bme_read();
  sdWrite(h2,mpuRead(),estado);
  delay(50);
  }
  //***************************ESTADO DE APOGEO
  else if(estado == "Apogeo") {
  //Desplegar paracaídas, SD, Blink
  digitalWrite(MATCH, HIGH);  //Despliegue del paracaídas
  h1 = bme_read();
  ay = mpuRead();
  sdWrite(h1,ay,estado);
  digitalWrite(LED, HIGH);
  digitalWrite(BUZZ, HIGH);
  if(h1 <= 10){
    estado = "Aterrizaje";
    sdWrite(h1,ay,estado);
  }
  delay(100);
  digitalWrite(LED, LOW);
  digitalWrite(BUZZ, LOW);
  delay(100);
  }
  else if(estado == "Aterrizaje"){
  digitalWrite(LED, HIGH);
  digitalWrite(BUZZ, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  digitalWrite(BUZZ, LOW);
  delay(100);
  }
} 

//**************************************************** FUNCIONES DE PERIFÉRICOS ***********************************************************
//**************************************************** FUNCIONES PARA EL MPU ***********************************************************
float mpuRead(){  
  sensors_event_t a, g, t_mpu;
  mpu.getEvent(&a, &g, &t_mpu);
  return a.acceleration.y + 9.81;
}

void mpuInit(){
  Serial.println("MPU6050 DRIVER");
  //Inicialización del MPU6050
  if(!mpu.begin()){
    Serial.println("No se pudo iniciar el MPU6050");
    while(1){
      delay(10);
    }
  }
  Serial.print("MPU detectado");

  //Se indican parámetros iniciales para el funcionamiento del MPU
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

//**************************************************** FUNCIONES PARA EL BME ***********************************************************
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

float bme_read(){
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
  return h;
}

//**************************************************** FUNCIONES PARA LA SD ***********************************************************
void sdInit(){
	bool aux;
  //Inicialización de la tarjeta SD
  if (!SD.begin(SSpin)) {     // inicializacion de tarjeta SD
    Serial.println("Fallo en inicializacion");// si falla se muestra texto correspondiente y
    return;         // se sale del setup() para finalizar el programa
  }
  Serial.println("Tarjeta inicializada");

  aux=true;
  while(aux){
  if (SD.exists("/flight "+(String)archivo_num+".txt"))
  {
    archivo_num++;
  }else{
    SD.open("/flight " +(String)archivo_num+ ".txt", FILE_WRITE);
    Serial.println("Archivo creado: ");
    Serial.println("/flight "+(String)archivo_num+".txt");
    aux=false;
    }
  }
}

void sdWrite(float mpu, float bme, String state){
  //Escritura en el archivo
  archivo = SD.open("/flight " +(String)archivo_num+ ".txt", FILE_APPEND);
  if(!archivo){
    Serial.println("Error al abrir el archivo");
    return;
  }
  datos_leidos = String(mpu) + ", " + String(bme) + ", " + state;
  archivo.println(datos_leidos);
  archivo.close();
}

//Función para avisar el despliegue de paracarídas
void blinkChute(){
  while(1){
  digitalWrite(LED, HIGH);
  digitalWrite(BUZZ, HIGH);
  delay(10);
  digitalWrite(LED, LOW);
  digitalWrite(BUZZ, LOW);
  delay(10);
  aux2++;
  if(aux2 == 50){
    aux = 0;
    break;
  }
  }
}