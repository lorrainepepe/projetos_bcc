//le o valor do potenciometro convertendo o analógico em digital (de 0 a 1023 para 0 a 255)
#define ldrPin 14//define o pino analógico em que o potenciometro vai ser conectado
int val = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //definir comunicação na porta serial, com taxa de transmissão de 9600 bits por segundo.
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
  val = analogRead(ldrPin);
  Serial.println(val);
}
