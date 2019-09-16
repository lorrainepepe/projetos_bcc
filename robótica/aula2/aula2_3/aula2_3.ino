//quando soltar o botão, acende o led suavemente, quando solta de novo desligar suavemente
//Ao soltar o botão led muda o estado. O led não pode ficar louco se o botão ficar pressionado.

//Lógica tradicional
int verifica_botao = 0;
int input = 3;
int output = 9;
int estado = 0;
int troca = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(output, OUTPUT);
  pinMode(input, INPUT);
  digitalWrite(input, LOW); // 0
  digitalWrite(output, LOW); // 0
 }

void botao(){
   while(digitalRead(input) == HIGH){ // somente ao soltar o botão é que devemos trocar o estado do led, isto é, quando sai do while.
    troca = 1;  
  }
  if(troca == 1){
    if(estado == 1 || estado == 3) estado = 0;
    else if(estado == 0 || estado == 2) estado = 1;
  }
  troca = 0;
  
}

void loop() {
   botao();
   if(estado == 1){
     for(int i = 0; i <=255; i++){ //acende lentamente
        delay(5);
        analogWrite(output,i); // i de intensidade do led.. led deve estar no pino pwm - pino 9.
    }
    estado = estado + 2;  // o estado 3 diz que o led já foi aceso completamente.
   } else if(estado == 0){
     for(int i = 255; i >=0; i--){ //apaga lentamente
        delay(5);
        analogWrite(output,i); // i de intensidade do led.. led deve estar no pino pwm - pino 9.
    }
    estado = estado + 2;  // estado 2 diz que o led já foi apagado completamente
   }
   
}
