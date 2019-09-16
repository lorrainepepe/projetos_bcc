
//Enquanto o botão (pino 2) estiver pressionado, o led deve ficar ligado.
//Lógica tradicional
int verifica_botao = 0;
int input = 3;
int output = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(output, OUTPUT);
  pinMode(input, INPUT);
  digitalWrite(input, LOW); // 0
  digitalWrite(output, LOW); // 0
}

void loop() {
  verifica_botao = digitalRead(input);
  digitalWrite(output,verifica_botao); // Note que com a negação temos o led desligado
}
