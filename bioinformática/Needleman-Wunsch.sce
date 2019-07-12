/* Trabalho de Bioinformática - 1ºsemestre/2019
   Implementação do algoritmo Needleman-Wunsch
   Linguagem: Scilab.
   
   Integrantes: Lorraine M. Pepe e Everton Camacho
                
*/
function [nome_seq_1, seq_1, tam_seq_1, nome_seq_2, seq_2, tam_seq_2] = ler_fasta()
    clc // limpa a tela
    arquivo_fasta = uigetfile("*.txt", pwd(), "Escolha o arquivo fasta");

    abre_fasta = mopen(arquivo_fasta,'r');
    if (abre_fasta == -1)
        error ("Erro ao abrir o arquivo!");
    end
    seq_1 = "";

    nome_seq_1 = mfscanf(1, abre_fasta, "%[^\n]")

    c = mfscanf(1, abre_fasta, "%c");
    tam_seq_1 = 0
    while c(1) <> '>' do
    	if (c(1) == 'A') | (c(1) == 'C') | (c(1) == 'T') | (c(1) == 'G') then
    		seq_1 = seq_1 + c(1);
    		tam_seq_1 = tam_seq_1 +1;
    		c = mfscanf(1, abre_fasta, "%c");
	   else //Se ler algo que não seja uma base(A,C,T,G) avança para o próximo caractere
            c = mfscanf(1, abre_fasta, "%c");
       end
    end

    seq_2 = "";

    nome_seq_2 = ">" + mfscanf(1, abre_fasta, "%[^\n]");
    c = mfscanf(1, abre_fasta, "%c"); //ler o \n depois do nome
    
    tam_seq_2 = 0;
    while ~meof(abre_fasta) do
    	if (c(1) == 'A') | (c(1) == 'C') | (c(1) == 'T') | (c(1) == 'G') then
    		seq_2 = seq_2 + c(1);
    		tam_seq_2 = tam_seq_2 +1;
    		c = mfscanf(1, abre_fasta, "%c");
	   else //Se ler algo que não seja uma base(A,C,T,G) avança para o próximo caractere
            c = mfscanf(1, abre_fasta, "%c");
       end
    end
    mclose(abre_fasta);
endfunction

clc
[nome_seq_1, seq_1, tam_seq_1, nome_seq_2, seq_2, tam_seq_2]= ler_fasta();

function [res_seq_1, res_seq_2, score, trace_back, soma_pontos, tempo_gasto] = needleman_wunsh();
    //Insere o - nas sequências
    seq_1 = '-' + seq_1;
    seq_2 = '-' + seq_2;
    
    //Inicializa a matriz toda com zeros
    n = tam_seq_2 + 1; //insere a linha do gap (-)
    m = tam_seq_1 + 1; //insere a coluna do gap (-)
    score = zeros(n,m);
    trace_back = zeros(n,m);
    
    match = 5;
    mismatch = -3;
    gap = -4;
    
    //Needleman-Wunsch
    
    tic(); //para medir o tempo gasto
    //preenche a primeira linha
    k = 1;
    for i = 2:n
        score(i,1) = gap * k;
        k = k + 1;
    end
    
    //preenche a primeira coluna
    k = 1;
    for j = 2:m
        score(1,j) = gap * k;
        k = k + 1;
    end
    
    //prepara a matriz trace_back
    for i = 2:n
        trace_back(i,1) = 3;
    end
    
    for i = 2:m
        trace_back(1,i) = 2;
    end
    
    seq_1 = mstr2sci(seq_1); //converte a string para um vetor de caracteres
    seq_2 = mstr2sci(seq_2); //converte a string para um vetor de caracteres
    
    //preenche a matriz de acordo com o algoritmo Needleman-Wunsch
    for i = 2:n
        for j = 2:m
            //percorre as duas sequências e verifica match, mismatch ou gap
            if (seq_1(j) == seq_2(i)) then //ocorreu match
                caso_1 = score(i-1,j-1) + match;
            else //ocorreu mismatch
                caso_1 = score(i-1,j-1) + mismatch;
            end
            
            caso_2 = score(i,j-1) + gap;        
            caso_3 = score(i-1,j) + gap;
            
            //verifica o máximo entre os três casos e coloca na matriz de pontuação 
            [score(i,j),max_index] = max(caso_1, caso_2, caso_3);
            
            //mapeia cada ponto para saber sua origem (1 se voltou pela diagonal, 2 se voltou pela esquerda, 3 se voltou por cima).
            trace_back(i,j) = max_index;
        end
    end
    
    nova_sequencia_1 = "";
    nova_sequencia_2 = "";
    
    i = n;
    j = m;
    
    while(i >= 1 && j >= 1)
        if(trace_back(i,j) == 1) then //volta pela diagonal
           nova_sequencia_1 = nova_sequencia_1 + seq_1(j);
           nova_sequencia_2 = nova_sequencia_2 + seq_2(i); 
           i = i-1;
           j = j-1;
        elseif(trace_back(i,j) == 2) then //volta pela esquerda
            nova_sequencia_1 = nova_sequencia_1 + seq_1(j);
            nova_sequencia_2 = nova_sequencia_2 + "-";
            j = j-1; 
        elseif(trace_back(i,j) == 3) then //volta por cima
            nova_sequencia_1 = nova_sequencia_1 + "-";
            nova_sequencia_2 = nova_sequencia_2 + seq_2(i);
            i = i-1; 
        end
        if(trace_back(i,j) == 0) then //acaba no trace_back(1,1)
            break;
        end
    end
    
    //coloca as novas sequências na ordem certa
    res_seq_1 = strrev(nova_sequencia_1);
    res_seq_2 = strrev(nova_sequencia_2);
    
    i = length(res_seq_2);
    j = length(res_seq_1);
  
    res_seq_2 = mstr2sci(res_seq_2); 
    res_seq_1 = mstr2sci(res_seq_1);
    
    soma_pontos = 0;
    //calcula o score
    while(i >= 1 && j >= 1)
       if(res_seq_1(j) == res_seq_2(i)) then
           soma_pontos = soma_pontos + match;
       elseif(res_seq_1(j) == "-" || res_seq_2(i) == "-") then 
           soma_pontos = soma_pontos + gap;
       else
           soma_pontos = soma_pontos + mismatch;
       end
       i = i - 1;
       j = j - 1;
    end
    
    tempo_gasto = toc();
endfunction

clc 
[res_seq_1, res_seq_2, score, trace_back, soma_pontos, tempo_gasto] = needleman_wunsh();

//exibe os resultados
disp(score);
disp(trace_back);

printf("\n:Alinhamento");

disp(res_seq_1);
disp(res_seq_2);

printf("\nPontos: %d\n", soma_pontos);
printf("\nTempo gasto: %f s\n", tempo_gasto);
