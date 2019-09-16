// O maior valor que se pode ter é o valor do potenciometro. Porém, a intensidade da luz deve ser de acordo com o ldr, nao ultrapassando o potenciometro.
//Quando tiver luz, acender pouco, qdo nao tiver, acender muito.

#define potPin 15
#define ldrPin 14
#define ledPin 9

int valPot = 0;
int intensidade = 0;
int valLdr = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  
}

void loop() {
  delay(100);
  valPot = analogRead(potPin);
  valPot = map(valPot,0,1023,0,255);
  
  valLdr = analogRead(ldrPin);
  intensidade = map(valLdr,10,500,valPot,0);
  analogWrite(ledPin, intensidade);
  
  // put your main code here, to run repeatedly:
  Serial.print(analogRead(ldrPin));
  Serial.print("\t");
  Serial.println(analogRead(ledPin));
  
  
  
}
