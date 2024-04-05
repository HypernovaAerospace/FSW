#include <SPI.h>    // incluye libreria interfaz SPI
#include <SD.h>     // incluye libreria para tarjetas SD

#define SSpin 5    // Slave Select en pin digital 10
#define LED 32 //Se indica el pin para el LED indicador
#define BUZZ 33 //Se indica el pin para el Buzzer indicador

float i,j = 0;
String datos_almacenados = "";
String datos_leidos = "";

File archivo;     // objeto archivo del tipo File
int archivo_num;

void sdInit();
void sdWrite(float mpu, float bme, String state);

void setup() {
	
  Serial.begin(9600);       // inicializa monitor serie a 9600 bps
  while(!Serial);

  sdInit();

  pinMode(LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);
}

void loop() {
  
  digitalWrite(LED, HIGH);
  digitalWrite(BUZZ, HIGH);
  i++;
  j++;
  String estado = "Plataforma";
  sdWrite(i,j,estado);
  delay(100);
  digitalWrite(LED, LOW);
  digitalWrite(BUZZ, LOW);
  delay(100);
}

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