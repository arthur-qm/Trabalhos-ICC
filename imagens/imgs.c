#include <stdio.h>
#include <stdlib.h>

void erro();

float** ler_arquivo(float **mat, int *w, int *h);
char* pegar_nome();

float** aloca_mat(int w, int h);

void libera_mat(float **mat, int linhas);

void esclarecer(float **mat, int w, int h, float fator);

void invert_vert(float **mat, int w, int h);
void invert_hor(float **mat, int w, int h);

float** gira_esquerda(float **mat, int *w, int *h);
float** gira_direita(float **mat, int *w, int *h);

float** aumentar(float **mat, int *w, int *h, float x);
float** diminuir(float **mat, int *w, int *h, float x);

void escrever_arq(float **mat, int w, int h);

int arredonda(float x, int lim); /* auxiliar */

int main() {
	
	int w, h; /* width, height */
	float **matriz_img = NULL;
	int opcao_menu;
	float fator_escuro;
	int opcao_modo;
	float fator_escalar;
	float **copia_matriz_img = NULL;

	matriz_img = ler_arquivo(matriz_img, &w, &h);
	
	/* Loop para o usuario ficar escolhendo o que quer */
	while (opcao_menu != 7) {
		printf("O que voce quer fazer com a imagem?\n");
		printf("Digite um numero:\n");
		printf("[1] - Ler outro arquivo\n");
		printf("[2] - Clarear ou escurecer a imagem por um fator informado\n");
		printf("[3] - Inverter horizontalmente ou verticalmente a imagem\n");
		printf("[4] - Rotacionar 90 graus a esquerda ou direita\n");
		printf("[5] - Aumentar ou reduzir a imagem a um fator escalar x\n");
		printf("[6] - Salvar imagem processada em um arquivo de saida\n");
		printf("[7] - Sair\n");

		scanf("%d", &opcao_menu);
		
		switch (opcao_menu) {
			case 1:
				matriz_img = ler_arquivo(matriz_img, &w, &h);
				break;
			case 2:
				printf("Informe o fator pelo qual voce quer escurecer ou clarear a imagem.\n");
				printf("Se for maior que 1, vai clarear e, se for menor, vai escurecer\n");
				scanf("%f", &fator_escuro);
				
				if (fator_escuro <= 0) {
					printf("Um fator nao positivo nao faz sentido :((\n");
					continue;
				}

				esclarecer(matriz_img, w, h, fator_escuro);

				break;
			case 3:
				printf("Voce quer inverter horizontalmente(0) ou verticalmente(1)?\n");
				scanf("%d", &opcao_modo);

				if (opcao_modo == 1) {
					invert_vert(matriz_img, w, h);
				} else {
					invert_hor(matriz_img, w, h);
				}

				break;
			case 4:
				printf("Voce quer rotacional 90 graus esquerda (anti-horario) ou 90 graus direita (horario)?\n");
				printf("Digite 0 para esquerda ou 1 pra direita\n");
				scanf("%d", &opcao_modo);

				if (opcao_modo == 1) {
					copia_matriz_img = gira_direita(matriz_img, &w, &h);
				} else {
					copia_matriz_img = gira_esquerda(matriz_img, &w, &h);
				}

				if (copia_matriz_img == NULL) {
					/* ja liberei a matriz dentro da funcao */
					erro();
				}

				matriz_img = copia_matriz_img;

				break;
			case 5:
				printf("Por qual fator voce quer escalar a imagem? Menor que 1 para diminuir e maior que 1 para aumentar\n");
				scanf("%f", &fator_escalar);
				
				if (fator_escalar <= 0) {
					printf("Um fator escalar nao positivo nao faz sentido!!\n");
					return 0;
				}

				if (fator_escalar > 1) {
					copia_matriz_img = aumentar(matriz_img, &w, &h, fator_escalar);
				} else if (fator_escalar < 1) {
					copia_matriz_img = diminuir(matriz_img, &w, &h, fator_escalar);
				}

				if (copia_matriz_img == NULL) {
					libera_mat(matriz_img, w);
					erro();
				}

				matriz_img = copia_matriz_img;
				break;
			case 6:
				printf("Com que nome voce deseja salvar seu arquivo?\n");
				escrever_arq(matriz_img, w, h);
				break;
			case 7:
				break; /* nao eh break de sair do loop...*/
			default:
				printf("Por favor escreva um inteiro de 1 a 7 >.<  !!!\n");
		}
	}
	
	libera_mat(matriz_img, w);

	return 0;
}

void erro() {
	printf("Pane no sistema alguem me desconfigurou\n");
	exit(1);
}

float** ler_arquivo(float **mat, int *w, int *h) {
	
	if (mat != NULL) {
		libera_mat(mat, *w);
	}
	
	printf("Digite o nome do arquivo e pressione enter: ");

	char* nome = pegar_nome();
	
	if (nome == NULL) {
		erro();
	}

	FILE *arq_entrada = fopen(nome, "r");
	
	if (arq_entrada == NULL) {
		printf("Erro ao abrir o arquivo\n");
		free(nome);
		erro();
	}

	fscanf(arq_entrada, "%d %d ", w, h);
	mat = aloca_mat(*w, *h);

	if (mat == NULL) {
		free(nome);
		fclose(arq_entrada);
		erro();
	}

	for (int i = 0; i < *w; i++) {
		for (int j = 0; j < *h; j++) {
			fscanf(arq_entrada, "%f ", &mat[i][j]);
		}
	}

	free(nome);
	fclose(arq_entrada);
	
	printf("Arquivo lido com sucesso!\n");

	return mat;
}

char* pegar_nome() {
	char* saida = NULL;
	char* copia_saida = NULL;
	char atual = '\0';
	int tam_nome_arq = 0;

	scanf(" %c", &atual);
	while (atual != '\n') {
		tam_nome_arq++;
		saida = (char*) realloc(copia_saida, (tam_nome_arq+1) * sizeof(char));

		if (saida == NULL) {
			free(copia_saida);
			return NULL;
		}

		saida[tam_nome_arq-1] = atual;
		saida[tam_nome_arq] = '\0';

		copia_saida = saida;
		scanf("%c", &atual);
	}

	return saida;
}

void libera_mat(float **mat, int linhas) {
	for (int i = 0; i < linhas; i++) {
		free(mat[i]);
	}
	free(mat);
}

float** aloca_mat(int w, int h) {
	float** saida = (float**) malloc(w * sizeof(float*));
	if (saida == NULL) {
		return NULL;
	}

	for (int i = 0; i < w; i++) {
		saida[i] = (float*) malloc(h * sizeof(float));
		if (saida[i] == NULL) {
			for (int j = 0; j < i; j++) {
				free(saida[j]);
			}
			free(saida);
			return NULL;
		}
	}

	return saida;
}

void esclarecer(float **mat, int w, int h, float fator) {
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			mat[i][j] *= fator;
			if (mat[i][j] > 1) {
				mat[i][j] = 1;
			}
		}
	}
}

void invert_vert(float **mat, int w, int h) {
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h/2; j++) {
			float temp = mat[i][j];
			mat[i][j] = mat[i][h-j-1];
			mat[i][h-j-1] = temp;
		}
	}
}

void invert_hor(float **mat, int w, int h) {
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w/2; i++) {
			float temp = mat[i][j];
			mat[i][j] = mat[w-i-1][j];
			mat[w-i-1][j] = temp;
		}
	}
}

float** gira_esquerda(float **mat, int *w, int *h) {
	
	float **saida = aloca_mat(*h, *w);
	
	if (saida == NULL) {
		return NULL;
	}

	for (int i = 0; i < *h; i++) {
		for (int j = 0; j < *w; j++) {
			saida[i][(*w)-j-1] = mat[j][i];
		}
	}
	
	libera_mat(mat, *w);

	int temp = *w;
	*w = *h;
	*h = temp;

	return saida;
}

float** gira_direita(float **mat, int *w, int *h) {
	
	float **saida = aloca_mat(*h, *w);
	
	if (saida == NULL) {
		return NULL;
	}

	for (int i = 0; i < *h; i++) {
		for (int j = 0; j < *w; j++) {
			saida[(*h)-i-1][j] = mat[j][i];
		}
	}

	libera_mat(mat, *w);

	int temp = *w;
	*w = *h;
	*h = temp;

	return saida;
}

/* mapeio as coordenadas pela multiplicacao do fator x
 * e depois, para os pixels que nao aconteceram nada, 
 * eu digo que eles valem o cara mais proximo deles */
float** aumentar(float **mat, int *w, int *h, float x) {
	
	int nw = (*w) * x;
	int nh = (*h) * x;

	float **saida = aloca_mat(nw, nh);

	if (saida == NULL) {
		return NULL;
	}
	
	for (int i = 0; i < nw; i++) {
		for (int j = 0; j < nh; j++) {
			saida[i][j] = mat[arredonda(i/x, *w)][arredonda(j/x, *h)];
		}
	}
	
	*w = nw;
	*h = nh;

	return saida;

}

float** diminuir(float **mat, int *w, int *h, float x) {
	
	int nw = (*w) * x;
	int nh = (*h) * x;

	float **saida = aloca_mat(nw, nh);

	if (saida == NULL) {
		return NULL;
	}

	int **contador = (int**) malloc(nw * sizeof(int*));
	if (contador == NULL) {
		libera_mat(saida, nw);
		return NULL;
	}

	for (int i = 0; i < nw; i++) {
		contador[i] = (int*) calloc(nh, sizeof(int));
		if (contador[i] == NULL) {
			libera_mat(saida, nw);
			for (int j = 0; j < nw; j++) {
				free(contador[j]);
			}
			free(contador);
			return NULL;
		}
	}
	
	/* zerar */
	for (int i = 0; i < nw; i++) {
		for (int j = 0; j < nh; j++) {
			saida[i][j] = 0;
			contador[i][j] = 0; /* so pra ter certeza */
		}
	}
	
	/* saida vira media dos pixels proximos */
	for (int i = 0; i < *w; i++) {
		for (int j = 0; j < *h; j++) {
			saida[arredonda(i * x, nw)][arredonda(j * x, nh)] += mat[i][j]; 
			contador[arredonda(i*x, nw)][arredonda(j*x, nh)]++;
		}
	}

	for (int i = 0; i < nw; i++) {
		for (int j = 0; j < nh; j++) {
			if (contador[i][j] >= 1) {
				saida[i][j] /= contador[i][j];
			}
		}
	}

	*w = nw;
	*h = nh;

	return saida;
}

void escrever_arq(float **mat, int w, int h) {
	char* nome = pegar_nome();

	if (nome == NULL) {
		libera_mat(mat, w);
		erro();
	}

	FILE *saida = fopen(nome, "w");
	
	if (saida == NULL) {
		free(nome);
		libera_mat(mat, w);
		erro();
	}
	
	fprintf(saida, "%d %d\n", w, h);
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			fprintf(saida, "%.2f ", mat[i][j]);
		}
		fprintf(saida, "\n");
	}

	free(nome);
	fclose(saida);
}

/* pode ser que o resultado seja maior que o indice da matriz
 * entao tem esse lim pra controlar */

int arredonda(float x, int lim) {
	int xx = x;
	
	if (x - xx > 0.5) {
		xx++;
	}

	if (xx >= lim) {
		return lim-1;
	}

	return xx;
}
