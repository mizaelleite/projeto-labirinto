#include <stdio.h>
#include <string.h>

#define MAX_ROW 20
#define MAX_COL 20
// Define a proporção máxima do labirinto que diz no doc p/ evitar problema de alocação de memória

typedef struct {
  char simbolo;
  int visitada; 

  } celula;

int direcoes[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Cima, Baixo, Esquerda, Direita

void posicaoI(celula labirinto[MAX_ROW][MAX_COL], int row, int column, int *x, int *y) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            if (labirinto[i][j].simbolo == '@') {
                *x = i;
                *y = j;
                return;
            }
        }
    }
}
// verificar posição inicial

int ehValido(int x, int y, int row, int column) {
    return (x >= 0 && x < row && y >= 0 && y < column);
}
// verificar se o vizinho é válido

void algrtm(celula labirinto[MAX_ROW][MAX_COL], int row, int column, int startX, int startY) { // Algoritmo de busca em largura
    int filaX[MAX_ROW * MAX_COL], filaY[MAX_ROW * MAX_COL];
    int frente = 0, tras = 0;

    // Matrizes auxiliares da distância e nós de origens
    int distancia[MAX_ROW][MAX_COL];
    int orgX[MAX_ROW][MAX_COL], orgY[MAX_ROW][MAX_COL];

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            distancia[i][j] = -1;
            orgX[i][j] = -1;
            orgY[i][j] = -1;
        }
    }

    // Introduz a fila com a posição inicial
    filaX[tras] = startX;
    filaY[tras] = startY;
    distancia[startX][startY] = 0;
    tras++;

    while (frente < tras) {
        int atualX = filaX[frente];
        int atualY = filaY[frente];
        frente++;

        // Verifica se completou o caminho correto
        if (labirinto[atualX][atualY].simbolo == '$') {
            printf("Vitória!\n");

            // Reconstruição do caminho
            int x = atualX, y = atualY;
            while (x != -1 && y != -1) {
                if (labirinto[x][y].simbolo != '@') {
                    if(labirinto[x][y].simbolo != '$') {
                      labirinto[x][y].simbolo = '*'; // Marca o caminho
                    } else {
                      labirinto[x][y].simbolo = 'v'; // Simboliza a vitória
                    }
                }
                int tempX = orgX[x][y];
                int tempY = orgY[x][y];
                x = tempX;
                y = tempY;
            }
            for (int i = 0; i < row; i++) {
                for (int j = 0; j < column; j++) {
                    printf("%c ", labirinto[i][j].simbolo);
                }
                printf("\n");
            }
            printf("\n");
            return;
        }
      
        // Verifica as posições vizinhas
        for (int i = 0; i < 4; i++) {
            int novoX = atualX + direcoes[i][0];
            int novoY = atualY + direcoes[i][1];

            if (ehValido(novoX, novoY, row, column)) {
                if (distancia[novoX][novoY] == -1 && labirinto[novoX][novoY].simbolo != '#' && labirinto[novoX][novoY].simbolo != '%') {
                    distancia[novoX][novoY] = distancia[atualX][atualY] + 1;
                    orgX[novoX][novoY] = atualX;
                    orgY[novoX][novoY] = atualY;
                    filaX[tras] = novoX;
                    filaY[tras] = novoY;
                    tras++;
                }
            }
        }
    }
// Precisamos adicionar as derrotas (caso ele se perca e/ou perde o combate)
    printf("Derrota!\n");
}

int main(int argc, char *argv[]) {
    int row, column;
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
  fscanf(arquivo, "%d %d", &row, &column);
  celula labirinto[MAX_ROW][MAX_COL];

  for (int i = 0; i < row; i++) { 
    for (int j = 0; j < column; j++) {                     // Laço para pegar os elementos do arquivo e armazenar em uma matriz

      fscanf(arquivo, " %c", &labirinto[i][j].simbolo);
      labirinto[i][j].visitada = 0;
      }
    }

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < column; j++) {                     // Laço para imprimir a matriz já formatada na tela
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

  int startX, startY;
  posicaoI(labirinto, row, column, &startX, &startY);
  
  arquivo = fopen(input2, "w"); 

  algrtm(labirinto, row, column, startX, startY);
  
   for (int i = 0; i < row; i++){ // Laço for para printar os elementos do labirinto em outro arquivo e deixar salvo.

     for (int j = 0; j < column; j++) {

        fprintf(arquivo, "%c ", labirinto[i][j].simbolo); // Imprime o labirinto resolvido
       }

     fprintf(arquivo, "\n");
     }

   printf("\n");

   printf("SALVO COM SUCESSO!\n");


   fclose(arquivo);




return 0;

}
