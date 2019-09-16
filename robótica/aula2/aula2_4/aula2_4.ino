//le o valor do potenciometro sem converter de 0 a 255
int potPin = 14;//define o pino analógico em que o potenciometro vai ser conectado
int ledPin =  9; //define o pino que terá um led e uma resitência ligado ao terra
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //definir comunicação.
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
  Serial.println(analogRead(potPin));
}
