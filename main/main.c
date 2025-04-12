#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ROW 20                    // Define a proporção máxima do labirinto que diz no doc p/ evitar problema de alocação de memória
#define MAX_COL 20

int prob(int n) {                     // Função de probabilidade criada para verificar se o personagem ganhou ou perdeu
    if (n > 10) n = 10;			      // Tem como entrada um número inteiro que será utilizado para representar a porcentagem inicial de vitória
    else if (n < 0) n = 0;

    int universo = rand() % 10;

    for (int i = 0; i < n; i++) {
        if (universo == i) return 1;
    }

    return 0;
}

typedef struct {                      // Estrutura utilizada para representar o labirinto
    char simbolo; 
    int visitada;
} celula;

int direcoes[4][2] = {         
    {-1, 0}, // Cima
    {1, 0},  // Baixo
    {0, -1}, // Esquerda
    {0, 1}   // Direita
};

void posicaoI(celula labirinto[MAX_ROW][MAX_COL], int row, int column, int *x, int *y) { // Função criada para verificar a posição inicial do personagem
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

int ehValido(int x, int y, int row, int column) {  // Função para verificar se as posições vizinhas são válidas
    return (x >= 0 && x < row && y >= 0 && y < column);
}

int algrtm(celula labirinto[MAX_ROW][MAX_COL], int row, int column, int startX, int startY) { // Algorítimo de Busca em Largura 
    int filaX[MAX_ROW * MAX_COL], filaY[MAX_ROW * MAX_COL];
    int frente = 0, tras = 0;

    int distancia[MAX_ROW][MAX_COL];                     // Matrizes auxiliáres da distância e nós de orígens
    int orgX[MAX_ROW][MAX_COL], orgY[MAX_ROW][MAX_COL];

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            distancia[i][j] = -1;
            orgX[i][j] = -1;
            orgY[i][j] = -1;
        }
    }

    filaX[tras] = startX;          // Introduz a fila com a posição inicial
    filaY[tras] = startY;
    distancia[startX][startY] = 0;
    tras++;

    int chance = 5;                // Definindo uma variável que será usada na função de probabilidade. Fixamos o número inicial de 5, que representa 50%
								   // assim como foi requisitado nas orientações
    while (frente < tras) {	
        int atualX = filaX[frente];
        int atualY = filaY[frente];
        frente++;

        if (labirinto[atualX][atualY].simbolo == '@') {  // Constroi o caminho correto
            int x = atualX, y = atualY;

            while (x != -1 && y != -1) {
                if (labirinto[x][y].simbolo != '$' && labirinto[x][y].simbolo != '@') {
                    if (labirinto[x][y].simbolo == '%') {
                        if (prob(chance)) {
                            labirinto[x][y].simbolo = '!';  // Após ganhar o combate, substitui aquela posição por '!'
                            chance++; // Aumenta mais 10% a chance de ganhar a cada vez que ele derrota um inimigo
                        } else {
                            labirinto[x][y].simbolo = '+'; // Após perder o combate, substitui aquela posição por '+'
                            printf("\nO personagem foi derrotado em combate!\n\n");	   
                            for (int i = 0; i < row; i++) {                            // Laço for para exibir o labirinto formatado assim que ele perde o combate para um inimigo
                                for (int j = 0; j < column; j++) {
                                    printf("%c ", labirinto[i][j].simbolo);
                                }
                                printf("\n");
                            }
                            return 0;
                        }
                    } else {
                        labirinto[x][y].simbolo = '*';
                    }
                }

                int tempX = orgX[x][y];
                int tempY = orgY[x][y];
                x = tempX;
                y = tempY;
            }

            printf("\nO personagem venceu todos os inimigos e chegou ao destino!\n\n");

            for (int i = 0; i < row; i++) {
                for (int j = 0; j < column; j++) {                // Laço for para trocar o símbolo '$' para 'v' assim que ele encontrar o caminho correto
                    if (labirinto[i][j].simbolo == '$') {
                        labirinto[i][j].simbolo = 'v';
                    }
                }
            }

            for (int i = 0; i < row; i++) {
                for (int j = 0; j < column; j++) {               // Laço for para exibir o labirinto resolvido na tela, com todos os inimigos derrotados
                    printf("%c ", labirinto[i][j].simbolo);      // e o caminho da vitória encontrado
                }
                printf("\n");
            }

            return 1;
        }

        for (int i = 0; i < 4; i++) {            // Laço para verificar as posições vizinhas            
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

int caminhoaleatorio(celula labirinto[MAX_ROW][MAX_COL], int row, int column, int x, int y) { // Função para fazer com que o personagem caminhe de forma aleatória
    int passos = 0;
    int chance = 5;

    while (passos < 20) { // Tenta andar algumas vezes antes de parar
        int direcao = rand() % 4; // Escolhe uma direção aleatória através da função rand()
        int novoX = x + direcoes[direcao][0];
        int novoY = y + direcoes[direcao][1];

        if (!ehValido(novoX, novoY, row, column) ||    // Verifica se o passo é válido;
            labirinto[novoX][novoY].simbolo == '#' ||
            labirinto[novoX][novoY].simbolo == '*' ||
            labirinto[novoX][novoY].simbolo == '@' ||
            labirinto[novoX][novoY].simbolo == '!') {
            passos++;
        } else if (labirinto[novoX][novoY].simbolo == '%') { // Caso encontre um inimigo, verifica se ele ganha ou perde o combate
            if (prob(chance)) {
                labirinto[novoX][novoY].simbolo = '!'; // Após ganhar o combate, substitui aquela posição por '!'
                x = novoX;
                y = novoY;
                chance++; // Aumenta mais 10% a chance de ganhar a cada vez que ele derrota um inimigo
                passos = 0;
            } else {
                printf("O personagem foi derrotado em combate!\n");
                labirinto[novoX][novoY].simbolo = '+'; // Após perder o combate, substitui aquela posição por '+'
                for (int i = 0; i < row; i++) {        // Laço for para imprimir as posições percorridas do labirinto assim que ele perder um combate. 
                    for (int j = 0; j < column; j++) {
                        printf("%c ", labirinto[i][j].simbolo);  
                    }
                    printf("\n");
                }
                return 0;
            }
        } else if (labirinto[novoX][novoY].simbolo == '$') {
            printf("O personagem encontrou o caminho!\n");
            labirinto[novoX][novoY].simbolo = 'v';   // Caso encontre o caminho, substitui aquela posição por 'v'
            for (int i = 0; i < row; i++) {
                for (int j = 0; j < column; j++) {
                    printf("%c ", labirinto[i][j].simbolo);      // Laço for para exibir o caminho percorrido após encontrar a vitória. 
                }
                printf("\n");
            }
            return 1;
        } else {
            x = novoX;
            y = novoY;
            passos = 0;
            labirinto[x][y].simbolo = '*'; // Marca a posição percorrida
        }
    }

    labirinto[x][y].simbolo = '?';  // Caso o personagem se perca, substitui aquela posição por '?'
    printf("O personagem se perdeu!\n\n");

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {               // Laço for para exibir o caminho percorrido pelo labirinto.
            printf("%c ", labirinto[i][j].simbolo);
        }
        printf("\n");
    }

    return 0;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int row, column, modo, modo2;
    int ci, cj, count = 0, verificar; // Ci e Cj são variáveis criadas para obter as posições iniciais [i] [j] do personagem, ou seja, onde está o símbolo '@'

    if (argc < 2) {                                            // Verificar se o usuário passou o nome do arquivo de entrada
        printf("Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }

    FILE *arquivo = fopen(argv[1], "r");                 // Realizar a leitura do arquivo de entrada
    if (!arquivo) {
        printf("Erro ao abrir o arquivo %s\n", argv[1]);
        return 1;
    }

    fscanf(arquivo, "%d %d", &row, &column);
    celula labirinto[MAX_ROW][MAX_COL];                  // Declaração do labirinto principal

    printf("Labirinto carregado!\n\n");

    for (int i = 0; i < row; i++) {                      // Laço para pegar os elementos de entrada e armazená-los em uma matriz, criando o labirinto
        for (int j = 0; j < column; j++) { 
            fscanf(arquivo, " %c", &labirinto[i][j].simbolo);
            labirinto[i][j].visitada = 0;
        }
    }

    celula labirinto2[MAX_ROW][MAX_COL];                 // Declarando um outro labirinto para ser utilizado como cópia e assim resetar durante cada execução do while
    for (int i = 0; i < row; i++)					     // A fim de que, quando chamado novamente as funções de resolução do labirinto, o labirinto utilizado possa ser o mesmo do arquivo
        for (int j = 0; j < column; j++)			     // de entrada (labirinto sem estar resolvido)
            labirinto2[i][j] = labirinto[i][j];

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {               // Exibir o labirinto (não resolvido) na tela.
            printf("%c ", labirinto[i][j].simbolo);
        }
        printf("\n");
    }
    printf("\n");

    char input2[15];       // Declaração de uma string que armazenará o nome do arquivo que o usuário deseja salvar
    char t[] = ".txt";

    printf("Escreva como deseja que o arquivo seja salvo: ");
    fgets(input2, sizeof(input2), stdin);
    input2[strcspn(input2, "\n")] = '\0';
    strcat(input2, t);     // Concatenando o nome do arquivo que o usuário passou com o '.txt'

    printf("Escolha um dos seguintes modos:\n1 -> Resolver com uma tentativa\n2 -> Resolver ate obter sucesso\n3 -> Sair do programa\n");
    scanf("%d", &modo);
    getchar();

    int startX, startY;  
    posicaoI(labirinto, row, column, &startX, &startY);

    if (modo == 1) { // Modo de resolver apenas uma vez
        printf("Escolha uma das alternativas:\n1 -> Tentar resolver de forma inteligente\n2 -> Tentar resolver de forma aleatoria\n");
        scanf("%d", &modo2);
        if (modo2 == 1) {
            algrtm(labirinto, row, column, startX, startY); // Chamando a função algrtm para resolver o labirinto de forma inteligente
        } else if (modo2 == 2) {
            for (int i = 0; i < row; i++)       // Laço for para pegar a posição inicial do personagem
                for (int j = 0; j < column; j++)
                    if (labirinto[i][j].simbolo == '@') {
                        ci = i;
                        cj = j;
                    }
            caminhoaleatorio(labirinto, row, column, ci, cj); // Chamando a função caminhoaleatorio para resolver o labirinto de forma aleatória
        }
    } else if (modo == 2) { // Modo de tentar resolver até conseguir
        printf("Escolha uma das alternativas:\n1 -> Tentar resolver de forma inteligente\n2 -> Tentar resolver de forma aleatoria\n");
        scanf("%d", &modo2);
        if (modo2 == 1) {
            while (1) { // While para tentar resolver o labirinto até conseguir
                for (int i = 0; i < row; i++)
                    for (int j = 0; j < column; j++)
                        labirinto[i][j] = labirinto2[i][j];  // Resetando o labirinto a cada execução do while

                verificar = algrtm(labirinto, row, column, startX, startY); // Chamando a função algrtm para tentar resolver o labirinto de forma inteligente
                count++; // Contando a quantidade de execuções (tentativas)
                if (verificar == 1) break; // Se conseguir completar, para o looping
            }
            printf("O labirinto foi resolvido apos %d tentativa(s)\n", count);
        } else if (modo2 == 2) {
            for (int i = 0; i < row; i++) // Laço for para pegar a posição inicial do personagem
                for (int j = 0; j < column; j++)
                    if (labirinto[i][j].simbolo == '@') {
                        ci = i;
                        cj = j;
                    }

            while (1) {
                for (int i = 0; i < row; i++)
                    for (int j = 0; j < column; j++)
                        labirinto[i][j] = labirinto2[i][j];

                verificar = caminhoaleatorio(labirinto, row, column, ci, cj); // Chamando a função caminho aleatório para tentar resolver o labirinto de forma aleatória
                count++;
                if (verificar == 1) break; // Se conseguir completar, para o looping
                if (count > 100) { // Se houver mais que 100 tentativas, o looping para e o o programa exibe uma mensagem de erro.
                    printf("Maximo de tentativas realizadas. Nao foi possivel concluir o labirinto.\n");
                    return 0;
                }
            }
            printf("O labirinto foi resolvido apos %d tentativa(s)\n", count);
        }
    } else {
        printf("Programa encerrado...\n");
        return 0;
    }

    arquivo = fopen(input2, "w"); // Cria o arquivo que o usuário nomeou.

    for (int i = 0; i < row; i++) {          // Imprime o labirinto resolvido
        for (int j = 0; j < column; j++) { 
            fprintf(arquivo, "%c ", labirinto[i][j].simbolo);
        }
        fprintf(arquivo, "\n");
    }

    printf("\nSALVO COM SUCESSO!\n");
    fclose(arquivo);
    return 0;
}
