matriz_img=imread("C:\Users\ander\Dropbox\UNESP\Mestrado\Processamento Digital de Imagens\Listas\Lista4\IMG\lena.bmp");
linhas=size(matriz_img,'r'); //recebe o numero de linhas da imagem
colunas=size(matriz_img,'c'); //recebe o numero de colunas da imagem
printf("Numero de linhas: %d, numero de colunas: %d", linhas, colunas);

// procedimento para descobrir o valor do maior nível de cinza
W=255;

printf("Nivel maximo de cinza: %d", W);

// procedimento de negação
nova_matriz=zeros(linhas,colunas);

for i=1:linhas
    for j=1:colunas
        nova_matriz(i,j)=W-matriz_img(i,j);
    end
end

nova_img=uint8(nova_matriz);

imshow(nova_img);
