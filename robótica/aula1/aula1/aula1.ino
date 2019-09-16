void setup() {
  // put your setup code here, to run once;
  Serial.begin(9600);
  pinMode(9,OUTPUT);
  digitalWrite(9,LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  digitalWrite(9,LOW);
  delay(500);
  digitalWrite(9,HIGH);
  //O led começa desligado, e desliga e liga (nessa ordem), dee meio em meio segundo. 

  
  
}
