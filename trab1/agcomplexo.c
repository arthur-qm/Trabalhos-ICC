/*
 * TRABALHO DE ICC1 - ALGORITMO GENETICO
 * 
 * INTEGRANTES:
 * -- Arthur Queiroz Moura (Num USP 13671532)
 * -- Joao Pedro Moura Diniz Araujo (Num USP 13671869)
 * ******CASOS TESTE DISCUTIDOS NO .txt*****
 * ******COMPILAR COM A FLAG -lm DEVIDO AO USO DA
 *       BIBLIOTECA MATH ******
 */



/* Nesse trabalho, foi feito um algoritmo genetico para encontrar
 * numeros complexos proximos das raizes de um polinomio 
 * generico de grau 'n' com coeficientes tambem complexos
 * (NOTA: O programa apenas lida com numeros complexos cujas partes
 * real e imaginaria sao ambas inteiras).
 */

/* Primeiramente, o usuario digita o grau do polinomio e seus coeficientes
 * Depois disso, ele digita os parametros de numero de geracoes, populacao
 * maxima por geracao, chance de mutacao por bit, [...] 
 */

/*
 * A partir dai, sao gerados individuos aleatorios e o AG comeca.
 * Primeiramente, para uma dada geracao, sao computados os erros de cada
 * individuo, que medem o quao ruim ele e.
 * Para calcular o erro, basta calcular |P(a + bi)|^2 onde a+bi e o individuo
 * e P o polinomio digitado.
 * Isso faz sentido, ja que representa a norma ao quadrado de um numero complexo
 * ou seja, o quadrado da distancia desse numero ate a origem no plano de Argand-Gauss.
 * Alem disso, toda vez que esse calculo do erro eh feito, sao colocados os melhores
 * individuos ate o momento em um vetor, que se atualiza a cada nova geracao
 *
 * Depois disso, e feito um bubble sort para rankear a geracao atual. Depois do ranking,
 * acontece um cruzamento aleatorio entre os "POR_MENORES"% (o usuario digita a 
 * porcentagem de melhores individuos).
 *
 * Para cruzar dois numeros, foram utilizados os operadores bit a bit tanto na parte real
 * quanto na imaginaria. Se a parte real de um tivesse o sinal oposto da parte real do outro,
 * o sinal era escolhido aleatoriamente (mesma coisa para a parte imaginaria). E, quanto aos
 * outros bits, se fosse igual, o bit permanecia igual. E se fosse diferente, era escolhido com
 * 50% de chance. Feito o cruzamento em si, ocorrem as mutacoes, que se tornam mais dificeis a
 * medida que se distancia do bit menos significante. E isso, finalmente, gera um individuo, o
 * qual e adicionado em um vetor que guarda a proxima geracao.
 *
 * Depois do numero especificado de geracoes ocorrer, o programa mostra o conteudo do vetor
 * que, a cada geracao, guarda os melhores daquela geracao e vai se adaptando com o tempo.
 * (Repare que esse vetor foi programado para conter apenas valores distintos).
 */


#include <stdio.h> /* printf, scanf */
#include <stdlib.h> /* rand, srand */
#include <math.h> /* sqrt */
#include <time.h> /* setar seed do srand */

int main() {
	
	srand(time(NULL));

	/* Variaveis cujos valores estao apenas
	 * por referencia, pois vao ser todas
	 * escaneadas. */

	int NUMGER = 1000;
	int RANGE_PRIMGER = 15;
	int popmax = 100;
	int maiorbitmut = 4;
	float POR_MENORES = 0.2;
	float CHANCE_MULT_BIT = 0.04;	

	int menores = POR_MENORES * popmax;

	int n;
	
	printf("===============\n");
	printf("ESSE PROGRAMA UTILIZA UM ALGORITMO GENETICO PARA ");
	printf("ENCONTRAR NUMEROS COMPLEXOS DA FORMA a+bi COM a E b INTEIROS QUE SAO PROXIMOS ");
	printf("DAS RAIZES DO POLINOMIO DE COEFICIENTES INTEIROS FORNECIDO PELO USUARIO.\n");
	printf("===============\n");

	printf("QUAL O GRAU DO POLINOMIO? ");
	scanf("%d", &n);
	
	if (n <= 0) {
		printf("O VALOR DO GRAU INFORMADO PRECISA SER MAIOR QUE ZERO!!");
		printf("\nEXECUCAO ABORTADA!\n");
		return 0;
	}

	printf("\n");

	/* Dois vetores: um com a parte real de
	 * cada um dos n+1 coeficientes e outro
	 * com a imaginaria */

	int coefsr[n+1], coefsi[n+1];

	for (int i = n; i >= 0; i--) {
		if (i > 0) {
			printf("\nQUAL O COEFICIENTE DO TERMO x^%d?\n", i);
		} else {
			printf("\nQUAL O TERMO CONSTANTE?\n");	
		}
		
		printf("PARTE REAL: ");
		scanf("%d", &coefsr[i]);
		printf("PARTE IMAGINARIA: ");
		scanf("%d", &coefsi[i]);

		if (i == n && coefsr[i] == 0 && coefsi[i] == 0) {
			printf("EM UM POLINOMIO, O TERMO DE MAIOR GRAU POSSUI COEFICIENTE NAO NULO!!\n");
			printf("EXECUCAO ABORTADA!\n");
			return 0;
		}
	}

	printf("COEFICIENTES DIGITADOS COM SUCESSO!!\n");
	printf("\nO PROGRAMA RESOLVERA A SEGUINTE EQUACAO POLINOMIAL:\n");
	
	for (int i = n; i >= 0; i--) {
		if (i > 0) {
			printf("(%d+(%d)i)x^%d + ", coefsr[i], coefsi[i], i);
		} else {
			printf("(%d+(%d)i) = 0\n", coefsr[i], coefsi[i]);
		}
	}

	/* Variaveis relacionadas ao AG sao escaneadas */
	

	printf("QUAL O NUMERO DE GERACOES? ");
	scanf("%d", &NUMGER);
	
	printf("QUAL A POPULACAO MAXIMA POR GERACAO? ");
	scanf("%d", &popmax);
	
	/* a probabilidade de mutacao e maior no bit menos significante */
	printf("QUAL A CHANCE DE MUTACAO DO BIT MENOS SIGNIFICANTE(em %%)? ");
	scanf("%f", &CHANCE_MULT_BIT);
	CHANCE_MULT_BIT /= 100;
	
	printf("QUAL A PORCENTAGEM DE MELHORES A SEREM CRUZADOS ENTRE SI? ");
	scanf("%f", &POR_MENORES);
	menores = POR_MENORES / 100 * popmax;
	
	/* a primeira geracao assume valores de -range ate +range */
	printf("QUAL O RANGE DE NUMEROS DA PRIMEIRA GERACAO? ");
	scanf("%d", &RANGE_PRIMGER);
	
	printf("QUAL O MAIOR BIT QUE PODE SER MUTADO? ");
	scanf("%d", &maiorbitmut);

	printf("OLHANDO GERACOES...\n");
	
	/* Os 2 vetores pop_inicial guardarao as partes
	 * imaginaria e real da populacao cujo score vai
	 * sendo testado a cada loop. Inicialmente terao
	 * um valor aleatorio definido pelo range informado
	 * pelo usuario.  */
	int pop_inicial_re[popmax+2];
	int pop_inicial_im[popmax+2];

	/* A cada geracao, o valor de cada individuo (a +bi)
	 * e colocado no polimomio e calculado.
	 * Entao esses dois vetores guardam as partes real
	 * e imaginaria de P(a + bi) */ 
	long long int resultados_re[popmax+2];
	long long int resultados_im[popmax+2];

	/* Esses dois vetores vao ser o resultado do cruzamento
	 * entre os melhores de uma determinada populacao inicial.*/
	int pop_depois_re[popmax+2];
	int pop_depois_im[popmax+2];

	/* Esse vetor guarda a norma |P(a+bi)|^2, que representa
	 * o quao longe o individuo esta de ser uma raiz. */
	long long int erros[popmax+2];

	/* Esses dois vetores guardam os n melhores individuos
	 * (distintos) ate o momento. Eles sao mostrados no final */
	int thefittestre[n];
	int thefittestim[n];

	/* Esse fetor auxiliar guarda a norma ao quadrado do
	 * i-esimo individuo guardado no vetor de cima */
	long long int thefittestsc[n];

	/* Essa variavel auxiliar guarda o indice do ultimo
	 * individuo colocado no vetor de melhores individuos
	 * Entao, por enquanto, ela vale -1. */
	int fittest_ind = -1;

	/* Gerar primeira ger */

	for (int i = 0; i < popmax; i++) {
		/* rand() % (2*range+1) gera um inteiro aleatorio no intervalo [0, 2*range]
		 * Se eu subtrair por range, o inteiro aleatorio fica no intervalo [-range, range]*/

		pop_inicial_re[i] = (rand() % (2*RANGE_PRIMGER + 1)) - RANGE_PRIMGER;
		pop_inicial_im[i] = (rand() % (2*RANGE_PRIMGER + 1)) - RANGE_PRIMGER;
	}

	/* Loop para lidar com as NUMGER-1 geracoes seguintes*/
	for (int ger=1; ger <= NUMGER; ger++) {
		/* a cada geracao ... */
		/* ver como elas estao (calcular o erro) */
		for (int i = 0; i < popmax; i++) {
			/* Criar variaveis com nome menor para nao precisar
			 * ficar escrevendo pop_inicial_re[i] */
			long long int re = pop_inicial_re[i];
			long long int im = pop_inicial_im[i];

			/* Variaveis que vao guardar P(re + i * im) */
			long long int ans_re = 0, ans_im = 0;
			
			/* Variaveis que guardam (re + im * i)^expoente da
			 * parcela do polinomio. Quando o expoente e zero, vale 1.*/

			long long int retothei = 1, imtothei = 0;
			
			/* Sao as parcelas atuais. Ou seja, o individuo elevado ao 
			 * expoente multiplicado pelo coeficiente correspondente, */
			long long int pa_at_re, pa_at_im;
			
			/* Variavel para auxiliar operacoes */
			long long int prev_retothei;

			for (int parcela=0; parcela <= n; parcela++) {
				
				/* Calcular o valor da parcela atual (coeficiente vezes
				 * termo x^parcela) */
				/* Lembrando que (a+bi)(c+di)=ac-bd + (ad+bc)i */
				pa_at_re = coefsr[parcela] * retothei - coefsi[parcela] * imtothei;
			    pa_at_im = coefsr[parcela] * imtothei + coefsi[parcela] * retothei;

				/* Adicionar parcela ao calculo de P(re + i * im) */
				ans_re += pa_at_re;
				ans_im += pa_at_im;	
				
				/* Preparar potencia para a proxima parcela. Para tanto, 
				 * fazer com que (retothei + i * imtothei) se multiplique por (re + i * im)*/
				prev_retothei = retothei;
				retothei = retothei * re - imtothei * im;
				imtothei = prev_retothei * im + imtothei * re;
			}

			/* Guardar o P do individuo atual */
			resultados_re[i] = ans_re;
			resultados_im[i] = ans_im;

			/* Guardar o quadrado da norma do P do individuo
			 * atual. Lembrando que |a+bi|^2 = a^2 + b^2 pode
			 * representar a distancia ate o zero. */
			erros[i] = ans_re * ans_re + ans_im * ans_im;
			
			/* A parte seguinte do codigo serve para atualizar
			 * o vetor que, a cada geracao, se atualiza com os 
			 * melhores individuos */

			/* Se nao tiver ninguem */
			if (fittest_ind == -1) {
				/* Colocar o atual */
				thefittestre[0] = pop_inicial_re[i];
				thefittestim[0] = pop_inicial_im[i];
				thefittestsc[0] = erros[i];
				fittest_ind = 0;
			} else {
				/* Se ja tiver alguem no vetor */

				/* Verificar se o individuo atual nao ja
				 * esta presente. */
				int flag_ja_existe = 0;

				for (int j = 0; j <= fittest_ind; j++) {
					if (thefittestre[j] == pop_inicial_re[i] && thefittestim[j] == pop_inicial_im[i]) {
						flag_ja_existe = 1;
						break;
					}
				}

				/* Se ele ja estiver presente, o proximo
				 * individuo e processado. */
				if (flag_ja_existe) {
					continue;
				}

				if (fittest_ind < n-1) {
					/* Se o vetor nao estiver completamente cheio */

					/* Colocar o individuo na ultima posicao */					
					thefittestre[fittest_ind+1] = pop_inicial_re[i];
					thefittestim[fittest_ind+1] = pop_inicial_im[i];
					thefittestsc[fittest_ind+1] = erros[i];
					
					/* Percorrer o vetor fazendo com que ele permaneca
					 * ordenado de acordo com o erro */

					for (int j = fittest_ind; j >= 0; j--) {
						if (thefittestsc[j] > thefittestsc[j+1]) {
							/* Se encontrar individuos fora de ordem,
							 * trocar de lugar */
							int tempre = thefittestre[j];
							int tempim = thefittestim[j];
							long long int tempsc = thefittestsc[j];

							thefittestre[j] = thefittestre[j+1];
							thefittestim[j] = thefittestim[j+1];
							thefittestsc[j] = thefittestsc[j+1];

							thefittestre[j+1] = tempre;
							thefittestim[j+1] = tempim;
							thefittestsc[j+1] = tempsc;
						} else {
							/* Como o vetor e ordenado, se eu nao encontrei
							 * dois termos fora de lugar, entao o resto do vetor
							 * esta OK */
							break;
						}
					}

					/* Depois de inserir o individuo, aumentar
					 * o indice do ultimo individuo do vetor */
					fittest_ind++;
				} else {
					
					/* Se o vetor ja estiver cheio */

					if (thefittestsc[n-1] > erros[i]) {
						/* Ver se ele e melhor que o pior do vetor */

						/* Se for, troca o pior do vetor */
						thefittestre[n-1] = pop_inicial_re[i];
						thefittestim[n-1] = pop_inicial_im[i];
						thefittestsc[n-1] = erros[i];

						/* Manter o vetor ordenado, conforme ja comentado */
						for (int j = n-2; j >= 0; j--) {
							if (thefittestsc[j] > thefittestsc[j+1]) {
								int tempre = thefittestre[j];
								int tempim = thefittestim[j];
								long long int tempsc = thefittestsc[j];

								thefittestre[j] = thefittestre[j+1];
								thefittestim[j] = thefittestim[j+1];
								thefittestsc[j] = thefittestsc[j+1];

								thefittestre[j+1] = tempre;
								thefittestim[j+1] = tempim;
								thefittestsc[j+1] = tempsc;
							} else {
								break;
							}
						}
					}
				}
			}
		} /* fim do processamento dos individuos */

		/* ranquea-los com bubble sort */
		for (int i = 0; i < popmax; i++) {
			for (int j = 0; j < popmax - 1; j++) {
				
				if (erros[j] > erros[j+1]) {
					/* se o indice j for pior, trocar */
					
					long long int ansrejtemp = resultados_re[j];
					long long int ansimjtemp = resultados_im[j];
					long long int scorejtemp = erros[j];
					int rejtemp = pop_inicial_re[j];
				    int imjtemp = pop_inicial_im[j];

					resultados_re[j] = resultados_re[j+1];
					resultados_im[j] = resultados_im[j+1];
					pop_inicial_re[j] = pop_inicial_re[j+1];
					pop_inicial_im[j] = pop_inicial_im[j+1];
					erros[j] = erros[j+1];

					resultados_re[j+1] = ansrejtemp;
					resultados_im[j+1] = ansimjtemp;
					pop_inicial_re[j+1] = rejtemp;
					pop_inicial_im[j+1] = imjtemp;
					erros[j+1] = scorejtemp;
				}
			}
		}

		/* Na ultima vez que o loop roda, ele
		 * so processa o score da ultima geracao
		 * (que vai ser a geracao de numero NUMGER,
		 * ja que a primeira foi a que foi gerada
		 * aleatoriamente)*/

		if (ger == NUMGER) {
			break;
		}

		/* Mostra quantos filhos foram gerados */
		int escolhidos = 0;

		/* Gerar popmax filhos para a proxima geracao */
		while (escolhidos < popmax) {

			/* Pegar dois indices de 0 ate menores-1 para cruzar */
			/* Ou seja, estou cruzando os melhores */
			int i1 = rand() % menores;
			int i2 = rand() % menores;

			/* Se os individuos forem iguais, entao, para aumentar a
			 * variabilidade, ha uma chance de 96% de outros indices
			 * serem escolhidos */

			if (pop_inicial_im[i1] == pop_depois_im[i2] && pop_inicial_re[i1] == pop_inicial_re[i2]) {
				if (rand() % 100 > 3) {
					continue;
				}
			}

			/* pop_depois vai guardar o valor dos filhos 
			 * Inicializando como zero... */
			pop_depois_re[escolhidos] = 0;
			pop_depois_im[escolhidos] = 0;
			
			/* Pegar as partes reais e imaginarias de cada indice */
			int agora1recp = pop_inicial_re[i1];
			int agora1imcp = pop_inicial_im[i1];

			int agora2recp = pop_inicial_re[i2];
			int agora2imcp = pop_inicial_im[i2];
			
			/* dizer que o sinal do individuo e o sinal
			 * do primeiro indice */
			int sinalre = agora1recp >= 0 ? 1 : -1;
			int sinalim = agora1imcp >= 0 ? 1 : -1;

			/* Mas se os sinais forem diferentes, pegar um
			 * sinal aleatorio */
			if (agora1recp * agora2recp <= 0) {
				sinalre = rand() % 2 ? 1 : -1;
			}

			if (agora1imcp * agora2imcp <= 0) {
				sinalim = rand() % 2 ? 1 : -1;
			}

			/* Com o sinal ja escolhido, deixo todos os
			 * negativos positivos para facilitar as contas. */
			if (agora1recp < 0) {
				agora1recp *= -1;
			}
			if (agora1imcp < 0) {
				agora1imcp *= -1;
			}
			if (agora2recp < 0) {
				agora2recp *= -1;
			}
			if (agora2imcp < 0) {
				agora2imcp *= -1;
			}
				
			/* Cruzar a parte real (cruzre) */
			/* bitpos eh a posicao atual do bit */
			int cruzre = 0, bitpos = 0;
			while (agora1recp > 0 || agora2recp > 0) {
				
				/* Se tiverem o bit zero em comum, ele vai ser
				 * herdado */
				int thisbit = 0;
				
				/* Se tiverem o bit 1 em comum, ele vai ser herdado */
				if (agora1recp & agora2recp & 1) {
					thisbit = 1;
				} else if ((agora1recp & 1) | (agora2recp & 1)) {
					/* Se forem diferentes os bits, pego 0 ou 1
					 * com 50% de chance */
					thisbit = rand() % 2;
				}
				/* Adiciono o bit atual a cruzre */
				cruzre += thisbit * (1 << bitpos);

				/* Atualizo variaveis para o proximo bit */
				agora1recp >>= 1;
				agora2recp >>= 1;
				bitpos++;
			}

			/* Anaologo ao cruzre */
			int cruzim = 0;
			bitpos = 0;
			while (agora1imcp > 0 || agora2imcp > 0) {
				int thisbit = 0;
				if (agora1imcp & agora2imcp & 1) {
					thisbit = 1;
				} else if ((agora1imcp & 1) | (agora2imcp & 1)) {
					thisbit = rand() % 2;
				}
				cruzim += thisbit * (1 << bitpos);
				agora1imcp >>= 1;
				agora2imcp >>= 1;
				bitpos++;
			}
			
			/* Depois de tudo, eu digo que o filho
			 * recebe cada parte vezes o seu sinal */
			pop_depois_re[escolhidos] = cruzre * sinalre;
			pop_depois_im[escolhidos] = cruzim * sinalim;

			/* Vou para o proximo filho */
			escolhidos++;

		} /* fim dos cruzamentos */

		/* Efetuar mutacoes */

		for (int i = 0; i < popmax; i++) {
			
			int at_re = pop_depois_re[i];
			int at_im = pop_depois_im[i];
			int sinalre = 1, sinalim = 1;

			int curr_re = 0, curr_im = 0;

			/* Para a mutacao, primeiramente extraio o sinal
			 * e deixo positiva cada parte */
			if (at_re < 0) {
				at_re *= -1;
				sinalre *= -1;
			}
			if (at_im < 0) {
				at_im *= -1;
				sinalim *= -1;
			}

			int bitpos = 0;
			
			/* loop do primeiro bit ate o maior bit que posso
			 * mutar, que ja foi uma variavel determinada */

			for (int j = 0; j < maiorbitmut; j++) {

				/* Vou mutar os bits das duas partes simultaneamente */	
				int bitre = 0;
				int bitim = 0;

				/* No lado esquerdo fica um numero de 0 ate 9999
				 * no direito, fica o numero associado a chance de ocorrer
				 * mutacao, a qual decai com o aumento da significancia do bit.
				 * Entao se j=0 e por exemplo CHANCE=0.03, entao o lado direito
				 * seria 300. Ou seja, se o lado esquerdo for de 0 ate 299, 
				 * entao a mutacao ocorre. Mas 0 ate 299 corresponde a 300 numeros.
				 * E 300 dividido pelos 10000 e igual a 0.03.
				 * Ou seja, a chance do primeiro bit eh determinada por
				 * CHANCE_MULT_BIT.
				 * A partir dai, a chance vai decaindo conforme o aumento
				 * da posicao do bit. (eg se j=1, a chance ja vai ser a metade). */
				if (rand() % 10000 < 10000 * CHANCE_MULT_BIT / (j+1)) {
					/* Se a chance aleatoria ocorrer, trocar o bit */
					bitre = 1 - (at_re & 1);
				} else {
					/* Se nao, o bit permanece o mesmo */
					bitre = at_re & 1;
				}

				/* Analogo ao bitre para o bitim */
				if (rand() % 10000 < 10000 * CHANCE_MULT_BIT / (j+1)) {
					bitim = 1 - (at_im & 1);
				} else {
					bitim = at_im & 1;
				}

				/* Adicionar o bit ao valor atual */
				curr_re += bitre * (1 << bitpos);
				curr_im += bitim * (1 << bitpos);
				
				/* Atualizar variaveis para a proxima iteracao do loop */
				at_re >>= 1;
				at_im >>= 1;
				bitpos++;
			}

			/* Mutar o sinal tambem, com uma chance 10% maior (fator de 1.1) */
			if (rand() % 10000 < 10000 * CHANCE_MULT_BIT * 1.1) {
				sinalre *= -1;
			}

			if (rand() % 10000 < 10000 * CHANCE_MULT_BIT * 1.1) {
				sinalim *= -1;
			}

			/* Termino da aplicacao da mutacao */
			pop_depois_re[i] = curr_re * sinalre;
			pop_depois_im[i] = curr_im * sinalim;
		}

		/* popinicial passa a valer popdepois e o loop
		 * comeca com a proxima geracao */

		for (int i = 0; i < popmax; i++) {
			pop_inicial_im[i] = pop_depois_im[i];
			pop_inicial_re[i] = pop_depois_re[i];
		}
	}

	/* Mostra os individuos mais bem adaptados, conforme
	 * a array dos n melhores individuos salvos. Tambem
	 * mostra o erro */
	
	printf("OS %d MAIS BEM ADAPTADOS:\n", fittest_ind+1);

	for (int i = 0; i <= fittest_ind; i++) {
		printf("Erro de (%d) + (%d)i = ", thefittestre[i], thefittestim[i]);
		printf("| P( (%d) + (%d)i ) | = ", thefittestre[i], thefittestim[i]);
		printf("%.5lf\n", sqrt(thefittestsc[i]));
	}

	return 0;
}
