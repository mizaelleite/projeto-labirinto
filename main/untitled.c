#include <stdio.h>
#include <string.h>


typedef struct {
    char simbolo;
    int visitada; 
} celula;


int main(int argc, char *argv[]) {
    // Verifica se o usuário passou o nome do arquivo de entrada
    if (argc < 2) {
        printf("Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }


    // Abre o arquivo de entrada para leitura
    FILE *arquivo = fopen(argv[1], "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo %s\n", argv[1]);
        return 1;
    }


    // Lê as dimensões do labirinto
    int row, column;
    fscanf(arquivo, "%d %d", &row, &column);
    
    // Declara a matriz do labirinto
    celula labirinto[row][column];


    // Lê os dados do arquivo e armazena na matriz
    for (int i = 0; i < row; i++) { 
        for (int j = 0; j < column; j++) {
            fscanf(arquivo, " %c", &labirinto[i][j].simbolo);
            labirinto[i][j].visitada = 0;
        }
    }
    
    // Fecha o arquivo de entrada após a leitura
    fclose(arquivo);


    // Exibe o labirinto carregado na tela
    printf("\nLabirinto carregado:\n");
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            printf("%c ", labirinto[i][j].simbolo);
        }
        printf("\n");
    }
    printf("\n");


    // Pede ao usuário o nome do arquivo de saída
    char input2[15];
    char t[] = ".txt"; 


    printf("Digite o nome do arquivo para salvar: ");
    fgets(input2, sizeof(input2), stdin);


    // Remove a quebra de linha do nome digitado
    input2[strcspn(input2, "\n")] = '\0';
    
    // Adiciona a extensão .txt automaticamente
    strcat(input2, t);


    // Abre o arquivo para escrita
    arquivo = fopen(input2, "w"); 
    if (!arquivo) {
        printf("Erro ao criar o arquivo de saída\n");
        return 1;
    }


    // Escreve o labirinto no arquivo de saída
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            fprintf(arquivo, "%c ", labirinto[i][j].simbolo);
        }
        fprintf(arquivo, "\n");
    }


    // Fecha o arquivo de saída
    fclose(arquivo);
    
    printf("\nSALVO COM SUCESSO em '%s'!\n", input2);
    
    return 0;
}
