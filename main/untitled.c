#include <stdio.h>
#include <string.h>

typedef struct {
	char simbolo;
	int visitada; 
	
	} celula;
	
int main () {

	int row, column;
	char input[15], input2[15];
	char t[] = ".txt"; // criei uma nova string só para o usuário não precisar colocar o .txt e abrir direto
	
	printf("Digite o nome do arquivo que deseja abrir: ");
	
	fgets(input, sizeof(input), stdin); // pegar o input do usuário e armazenar em input
	
	input[strcspn(input, "\n")] = '\0'; // substituir a quebra de linha pelo elemento de quebra de uma string.
	
	strcat(input, t); // concatenar o nome do arquivo com .txt
	printf("\n"); // adicionar uma quebra de linha adicional para ficar mais bonitinho

	FILE *arquivo = fopen(input, "r");
	  if(arquivo == NULL) {
    		printf("Falha ao acessar o arquivo\n"); // Caso o usuário coloque um arquivo inexistente
    		return 1;
  	}
	fscanf(arquivo, "%d %d", &row, &column);
	
	celula labirinto[row][column];

	for (int i = 0; i < row; i++) { 
		for (int j = 0; j < column; j++) {                     // laço para pegar os elementos do arquivo e armazenar em uma matriz
			
			fscanf(arquivo, " %c", &labirinto[i][j].simbolo);
			labirinto[i][j].visitada = 0;
			}
		}
		
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {                     // laço para imprimir a matriz já formatada na tela
			printf("%c ", labirinto[i][j].simbolo);
			}
			printf("\n");
		}
	printf("\n");
	
	printf("Digite como deseja salvar o nome do arquivo: ");
	
	fgets(input2, sizeof(input2), stdin); // pegar o input do usuário e armazenar em input
	
	input2[strcspn(input2, "\n")] = '\0'; // substituir a quebra de linha pelo elemento de quebra de uma string.
	
	strcat(input2, t); // concatenar o nome do arquivo com .txt
 
	arquivo = fopen(input2, "w"); 
	 
	 for (int i = 0; i < row; i++){ // laço for para printar os elementos do labirinto em outro arquivo e deixar salvo.
		 
		 for (int j = 0; j < column; j++) {
			 
				fprintf(arquivo, "%c ", labirinto[i][j].simbolo);
			 }
		 
		 fprintf(arquivo, "\n");
		 }
		
	 printf("\n");
	 
	 printf("SALVO COM SUCESSO!\n");
		 
		 
	 fclose(arquivo);
	 

	
	
return 0;
	
}
