#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ROW 20
#define MAX_COL 20
// Define a proporção máxima do labirinto que diz no doc p/ evitar problema de alocação de memória

// modo 1 = o personagem ganha
// modo 2 = o personagem se perde

int prob (int n) {// Função de teste de probabilidade para o sistema de combate

    if (n > 10) {
        n = 10;
        }
    else if (n < 0) {
        n = 0;
        }

    int ale = rand() % 10;

    for (int i = 0; i < n; i++) {

        if (ale == i) {
            return 1;
            }
        }    
    return 0;    
}

typedef struct {
  char simbolo;
  int visitada; 

  } celula;

int direcoes[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Cima, Baixo, Esquerda, Direita

void posicaoI(celula labirinto[MAX_ROW][MAX_COL], int row, int column, int *x, int *y) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            if (labirinto[i][j].simbolo == '$') {
                *x = i;
                *y = j;
                return;
            }
        }
    }
}
// Verifica posição inicial

int ehValido(int x, int y, int row, int column) {
    return (x >= 0 && x < row && y >= 0 && y < column);
}
// Verifica se o vizinho é válido


int algrtm(celula labirinto[MAX_ROW][MAX_COL], int row, int column, int startX, int startY) { // Algoritmo de busca em largura
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

    int in = 5; // definindo uma variável que será usada no sistema de combate

    while (frente < tras) {
        int atualX = filaX[frente];
        int atualY = filaY[frente];
        frente++;

        // Constroe o caminho correto
   
        if (labirinto[atualX][atualY].simbolo == '@') {
            int x = atualX, y = atualY;
            while (x != -1 && y != -1) {
                if (labirinto[x][y].simbolo != '$') {
                    if(labirinto[x][y].simbolo != '@') {
                        if(labirinto[x][y].simbolo == '%') {
                            if (prob(in)) {
                                labirinto[x][y].simbolo = '!';
                                in++;
                                }
                            else {
                                labirinto[x][y].simbolo = '+';
                                printf("\n");
                                printf("O personagem foi derrotado em combate!\n\n");
                                for (int i = 0; i < row; i++) {
									for (int j = 0; j < column; j++) {
										printf("%c ", labirinto[i][j].simbolo);  // laço for para exibir o caminho na tela
									}
									printf("\n");
								}
                                return 0;
                                }
                        } else {
                            labirinto[x][y].simbolo = '*'; // Marca o caminho
                        }
                    } 
                }
                int tempX = orgX[x][y];
                int tempY = orgY[x][y];
                x = tempX;
                y = tempY;
            }
            printf("\n");
            printf("O personagem venceu todos os inimigos e chegou ao destino!\n\n");
            for (int i = 0; i < row; i++) {
				for (int j = 0; j < column; j++) {
					if (labirinto[i][j].simbolo == '$') {       // laço for para trocar o símbo da vitória quando ele chega ao destino
						labirinto[i][j].simbolo = 'v';
						}
					}
				}
			for (int i = 0; i < row; i++) {
                for (int j = 0; j < column; j++) {
                    printf("%c ", labirinto[i][j].simbolo);        // laço for para exibir o caminho na tela
                }
                printf("\n");
            }
	
            return 1;
        }

        // Verifica as posições vizinhas
        for (int i = 0; i < 4; i++) {
            int novoX = atualX + direcoes[i][0];
            int novoY = atualY + direcoes[i][1];

            if (ehValido(novoX, novoY, row, column)) {
                if (distancia[novoX][novoY] == -1 && labirinto[novoX][novoY].simbolo != '#') {
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
return 0;
}

int caminhoaleatorio(celula labirinto[MAX_ROW][MAX_COL], int row, int column, int x, int y) {
    int passos = 0;
    int in = 5;

    while (passos < 20) { // Tenta andar algumas vezes antes de parar
		
        int direcao = rand() % 4; // escolhe uma direção
        int novoX = x + direcoes[direcao][0];
        int novoY = y + direcoes[direcao][1];

        if (!(ehValido(novoX, novoY, row, column)) || (labirinto[novoX][novoY].simbolo == '#') || (labirinto[novoX][novoY].simbolo == '*') || (labirinto[novoX][novoY].simbolo == '@' ) || (labirinto[novoX][novoY].simbolo == '!' )) {
            passos++; }  // verifica se o passo é válido
            
        else if (labirinto[novoX][novoY].simbolo == '%') {
			if (prob(in)) { // verificando se ele ganha o combate
				labirinto[novoX][novoY].simbolo = '!';
				x = novoX;
                y = novoY; 
                passos = 0;
                labirinto[x][y].simbolo = '!'; 
				}
			else {
				printf("O personagem foi derrotado em combate!\n");
				labirinto[novoX][novoY].simbolo = '+'; // caso ele perca o combate
				for (int i = 0; i < row; i++) {
				for (int j = 0; j < column; j++) {
					printf("%c ", labirinto[i][j].simbolo); // imprime o labirinto na tela
				}
				printf("\n");
			}
				return 0;
				}
			
			}
		else if (labirinto[novoX][novoY].simbolo == '$') {
			printf("O personagem encontrou o caminho!\n"); // caso ele ache o caminho, irá marcar como vitória
			labirinto[novoX][novoY].simbolo = 'v';
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < column; j++) {
					printf("%c ", labirinto[i][j].simbolo); // imprime o labirinto na tela
				}
				printf("\n");
			}
			return 1;
			}
        else {
            x = novoX;
            y = novoY; 
            passos = 0;
            labirinto[x][y].simbolo = '*'; 
        }
	}
	
 
    labirinto[x][y].simbolo = '?'; // Simboliza que o personagem se perdeu
    
    printf("O personagem se perdeu!\n\n");
	
	for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            printf("%c ", labirinto[i][j].simbolo); // imprime o labirinto na tela
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}




int main(int argc, char *argv[]) {
    srand(time(NULL));
    int row, column, modo, modo2;
    int ci,cj, count = 0, ver;
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
  
  printf("Labirinto carregado!\n\n");

  for (int i = 0; i < row; i++) { 
    for (int j = 0; j < column; j++) {                     // Laço para pegar os elementos do arquivo e armazenar em uma matriz

      fscanf(arquivo, " %c", &labirinto[i][j].simbolo);
      labirinto[i][j].visitada = 0;
      }
    }
    
  celula labirintoOriginal[MAX_ROW][MAX_COL];             // Cópia do labirinto para usar dentro do while
    for (int i = 0; i < row; i++) { 
    for (int j = 0; j < column; j++) {
        labirintoOriginal[i][j] = labirinto[i][j];
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
	  
	printf("Escreva como deseja que o arquivo seja salvo: ");
    fgets(input2, sizeof(input2), stdin);


    // Remove a quebra de linha do nome digitado
    input2[strcspn(input2, "\n")] = '\0';

    // Adiciona a extensão .txt automaticamente
    strcat(input2, t);
    
    printf("Escolha um dos seguintes modos:\n1 -> Resolver com uma tentativa\n2 -> Resolver ate obter sucesso\n3 -> Sair do programa\n");// Pede ao usuário qual o modo do labirinto
    scanf("%d", &modo);
    getchar();
  
  int startX, startY;
  posicaoI(labirinto, row, column, &startX, &startY);

  if(modo == 1){
	   printf("Escolha uma das alternativas:\n1 -> Tentar resolver de forma inteligente\n2 -> Tentar resolver de forma aleatoria\n");
	   scanf("%d", &modo2);
	   if (modo2 == 1) {
		   algrtm(labirinto, row, column, startX, startY);
		   }
	   else if (modo2 == 2) {
		   for (int i = 0; i < row ; i++) {
			   for (int j = 0; j < column; j++) {
				   if (labirinto[i][j].simbolo == '@') {
					  ci = i;
					  cj = j;
				   }
			   }
		   }
		   
		caminhoaleatorio(labirinto,row,column,ci,cj);
   }
}

   else if (modo == 2) {
	   printf("Escolha uma das alternativas:\n1 -> Tentar resolver de forma inteligente\n2 -> Tentar resolver de forma aleatoria\n");
	   scanf("%d", &modo2);
	   if (modo2 == 1) {
		   while (1) {
			   for (int i = 0; i < row; i++) {
                for (int j = 0; j < column; j++) {               // laço for para criar uma cópia toda vez a cada execução do labirinto
                    labirinto[i][j] = labirintoOriginal[i][j];
                }
            }
			   ver = algrtm(labirinto,row, column, startX, startY);
			   count++;
			   if (ver == 1) {
				   break;
				   }
			   }
		   printf("O labirinto foi resolvido apos %d tentativa(s)\n", count);
	   }
	    else if (modo2 == 2) {
		   for (int i = 0; i < row ; i++) {
			   for (int j = 0; j < column; j++) {
				   if (labirinto[i][j].simbolo == '@') {
					  ci = i;
					  cj = j;
				   }
			   }
		   }
		   
		while (1) {
			for (int i = 0; i < row; i++) {
                for (int j = 0; j < column; j++) {
                    labirinto[i][j] = labirintoOriginal[i][j];
                }
            }
			ver = caminhoaleatorio(labirinto, row, column, ci, cj);
			count++;
			if (ver == 1) {
				break;
				}
			if (count > 100) {
				printf("Maximo de tentativas realizadas. Nao foi possivel concluir o labirinto.");
				break;
				}
			}
		printf("O labirinto foi resolvido apos %d tentativa(s)\n", count);
   }
}

    else {
		printf("Programa encerrado...");
		return 0;
	}

  
  arquivo = fopen(input2, "w"); 

    
  
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
