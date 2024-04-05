#include <Adafruit_MPU6050.h>//Librería para el MPU6050
#include <Adafruit_Sensor.h>//Librería para el MPU6050
#include <Wire.h>//Librería para el MPU6050

#define LED 32 //Se indica el pin para el LED indicador
#define BUZZ 33 //Se indica el pin para el Buzzer indicador

//Se crea el objeto mpu
Adafruit_MPU6050 mpu;

//Prototipo de funciones
float mpuRead();
void mpuInit();

void setup() {
  //Iniciliazación de la comunicación serie con la terminal
  Serial.begin(9600);
  while(!Serial);
    delay(10);

  //Inicialización del MPU
  mpuInit();

  //Salidas
  pinMode(LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, HIGH);
  digitalWrite(BUZZ, HIGH);
  Serial.print(mpuRead());
  Serial.println("y");
  delay(100);
  digitalWrite(LED, LOW);
  digitalWrite(BUZZ, LOW);
  delay(100);

}

float mpuRead(){  
  sensors_event_t a, g, t_mpu;
  mpu.getEvent(&a, &g, &t_mpu);
  return a.acceleration.y;
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

