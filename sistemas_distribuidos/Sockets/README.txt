Escrever um programa para o c�lculo de transfer�ncia de calor em uma chapa met�lica, usando uma estrutura do tipo peer-computing com sockets em C.  


O programa deve ser disparado em uma m�quina (o servidor) e, em seguida, ser disparado em outras m�quinas, que informar�o o servidor de seus endere�os. 

O servidor estabelecer� ent�o um ranking entre os "clientes", colocando-se como primeiro da lista e devolvendo aos demais os endere�os de seus vizinhos.  

A partir disso os "clientes" e servidor devem fazer a computa��o das temperaturas at� a diferen�a entre os valores de uma itera��o e outra ficar abaixo de 0.01  

Deve ser considerada uma chapa de 400x400 pontos, com bordas a 0 C e pontos de calor dados na tabela abaixo: 
 
linha	coluna	temp
75	75	-10
75	175	25
75	275	0
190	75	20
190	175	-20
190	275	10
305	75	10
305	175	30
305	275	40
 
 

O programa deve ser executado com 1, 3 e 7 clientes, medindo o tempo gasto em cada execu��o (repetir a medida pelo menos 10 vezes).