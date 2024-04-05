#define LED 4 //Indicador visual
#define BUZZ 2  //Indicador auditivo
#define MATCH 15 //Cerillo electrónico

void setup() {
  Serial.begin(9600);
  while(!Serial);

  //Salidas
  pinMode(LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(MATCH, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);
  digitalWrite(BUZZ, LOW);
  digitalWrite(MATCH, LOW);

  delay(2000);
  digitalWrite(LED, LOW);
  digitalWrite(BUZZ, HIGH);
  digitalWrite(MATCH, HIGH);
  delay(3000);


}
