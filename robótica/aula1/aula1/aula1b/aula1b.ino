void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(9,OUTPUT);
  digitalWrite(9,LOW);
}

void loop() {
  //vai do apagado até o aceso porém lentamente
  //parte 2 - estudar Pulse Width Modulation (PNM) - usa-se a biblioteca analogWrite
  for(int i = 0; i <=255; i++){
    delay(5);
    analogWrite(9,i);
  }
}
