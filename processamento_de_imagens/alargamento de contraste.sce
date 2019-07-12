matriz_img=imread("C:\Users\alunos\Desktop\lena.bmp");
linhas=size(matriz_img,'r'); //recebe o numero de linhas da imagem
colunas=size(matriz_img,'c'); //recebe o numero de colunas da imagem
printf("Numero de linhas: %d, numero de colunas: %d", linhas, colunas);

// procedimento para descobrir o valor do maior nível de cinza
W=255;
L1 = 100;
L2 = 200;
k1 = 0.6;
k2 = 1.2;
k3 = 0.95;

//printf("Nivel maximo de cinza: %d", W);

// procedimento de negação
nova_matriz=zeros(linhas,colunas);

for i=1:linhas
    for j=1:colunas
        if(matriz_img(i,j) < L1) 
            if (k1*matriz_img(i,j) > 255) nova_matriz(i,j) = 255;
            else nova_matriz(i,j) = k1*matriz_img(i,j);
            end
        elseif(matriz_img(i,j) >= L1 && matriz_img(i,j) < L2) 
            if (k2*matriz_img(i,j) > 255) nova_matriz(i,j) = 255;
            else nova_matriz(i,j) = k2*matriz_img(i,j);
            end
        else
            if (k3*matriz_img(i,j) > 255) nova_matriz(i,j) = 255;
            else nova_matriz(i,j) = k3*matriz_img(i,j);
            end
        end
    end
end

nova_img=uint8(nova_matriz);

imshow(nova_img);
