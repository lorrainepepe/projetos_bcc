//Ao soltar o botão led muda o estado. O led não pode ficar louco se o botão ficar pressionado.
//Lógica tradicional
int verifica_botao = 0;
int input = 3;
int output = 13;
int estado = 0;
int troca = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(output, OUTPUT);
  pinMode(input, INPUT);
  digitalWrite(input, LOW); // 0
  digitalWrite(output, LOW); // 0
}

int botao(){
   while(digitalRead(input) == HIGH){ // somente ao soltar o botão é que devemos trocar o estado do led, isto é, quando sai do while.
    troca = 1;  
  }
  if(troca == 1){
    if(estado == 1) estado = 0;
    else estado = 1;
  }
  troca = 0;
  return estado;
}

void loop() {
   int b = botao();
    digitalWrite(output,b); // Note que com a negação temos o que precisamos no enunciado.
  
}
