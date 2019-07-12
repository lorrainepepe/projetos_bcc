matriz_img=imread("C:\Users\alunos\Desktop\lena.bmp");
linhas=size(matriz_img,'r'); //recebe o numero de linhas da imagem
colunas=size(matriz_img,'c'); //recebe o numero de colunas da imagem
printf("Numero de linhas: %d, numero de colunas: %d", linhas, colunas);

// procedimento para descobrir o valor do maior nível de cinza
W=255;
L = input("\n\nDigita um valor aí bonitão: ");

//printf("Nivel maximo de cinza: %d", W);

// procedimento de negação
nova_matriz=zeros(linhas,colunas);

for i=1:linhas
    for j=1:colunas
        if(matriz_img(i,j) > L) nova_matriz(i,j) = 255;
        else nova_matriz(i, j) = 0;
        end
    end
end

nova_img=uint8(nova_matriz);

imshow(nova_img);
