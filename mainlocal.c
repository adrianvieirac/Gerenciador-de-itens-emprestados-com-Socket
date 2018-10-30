#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_COISAS 50

//definindo se e WIN32 ou UNIX
#if defined(_WIN32) || defined(WIN32)
#define LIMPA_TERM system("cls")
#else
#define LIMPA_TERM system("clear")
#endif

typedef struct
{
	char coisa[30];
	char nome_emprestou[50];
	char nome_emprestado[50];
	int data_emp[3];
	int data_dev[3];
	int devo;
	int ativo;
} Coisa;

Coisa coisas[MAX_COISAS];

void menu();
void carregar();
void salvar();
void listar();
void atrasados();
void devolvidos();
void cadastrar_coisas();
void editar();
void remover_coisas();
void printar();

int opcoes(void)
{
	printf("COISAS EMPRESTADAS\n\n1 - Listar Itens\n2 - Itens Atrasados\n3 - Itens Devolvidos\n4 - Cadastrar Itens\n5 - Remover Itens\n6 - Editar item\n0 - Fechar Programa\n\n");
}

int main()
{
	carregar();
	menu();
	salvar();

	return 0;
}

void menu()
{
	//variables
	int op;

	//menu
	do{
		LIMPA_TERM;
		opcoes();
		scanf("%d", &op);

		switch(op)
		{
			case 1:
			LIMPA_TERM;
			printf("LISTA DE ITENS\n");
			listar();
			break;

			case 2:
			LIMPA_TERM;
			printf("Itens Atrasados\n");
			atrasados();
			break;

			case 3:
			LIMPA_TERM;
			devolvidos();
			break;

			case 4:
			LIMPA_TERM;
			printf("Cadastrar Itens\n");
			cadastrar_coisas();
			break;

			case 5:
			LIMPA_TERM;
			printf("Remover Itens\n");
			remover_coisas();
			break;

			case 6:
			LIMPA_TERM;
			printf("Editar Itens\n");
			editar();
			break;

			default:
			LIMPA_TERM;
			printf("\nInvalid Value\n");
			opcoes();

		}
	}while(op != 0);

}

void listar()
{
	int i, cont = 0;
	
	for ( i = 0; i < MAX_COISAS; ++i)
	{
		if (coisas[i].ativo == 1)
		{
			printar(i);
			cont = 1;
		}
	}

	if (cont == 0)
	{
		printf("Nenhum item Cadastrado\n");
	}
	printf("\nDigite enter para sair\n");
	setbuf(stdin,NULL);
	getchar();

}

void atrasados()
{
	int i, cont = 0;
	int data[3];
	int atrasado = 1;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	data[2] = tm.tm_year + 1900;
	data[1] = tm.tm_mon + 1;
	data[0] = tm.tm_mday;
	
	for(i = 0; i < MAX_COISAS; ++i)
	{
		if (coisas[i].ativo == 1)
		{
			if (coisas[i].data_dev[2] > data[2])
			{
				atrasado = 0;
			}
			else if(coisas[i].data_dev[2] == data[2] && coisas[i].data_dev[1] > data[1])
			{
				atrasado = 0;
			}
			else if(coisas[i].data_dev[2] == data[2] && coisas[i].data_dev[1] == data[1] && coisas[i].data_dev[0] >= data[0])
			{
				atrasado = 0;
			}

			if (coisas[i].data_dev[2] == 0 && coisas[i].data_dev[1] == 0 && coisas[i].data_dev[0] == 0)
			{
				atrasado = 0;
			}
			
			if(atrasado && coisas[i].devo == 0)
			{
				printar(i);
				cont = 1;
			}
		}
	}
	if (cont == 0)
	{
		printf("Nenhum item atrasado\n");	
	}
	printf("\nDigite enter para sair\n");
	setbuf(stdin, NULL);
	getchar();
}

void devolvidos()
{
	int i, op, id, cont = 0;
	do{
		LIMPA_TERM;
		printf("LISTA DE ITENS\n");
		listar();

		printf("ITENS DEVOLVIDOS\n");
		for ( i = 0; i < MAX_COISAS; ++i)
		{
			if (coisas[i].ativo == 1 && coisas[i].devo == 1)
			{
				printar(i);
				cont = 1;
			}
		}
		if (cont == 0)
		{
			printf("Nenhum Iten devolvido\n");
			
		}

		printf("\n1- Marcar item como devolvido\n0 - Sair\n");
		scanf("%d", &op);
		if(op == 1)
		{
			printf("\n\nDigite o ID do item para marca: ");
			scanf("%d", &id);
			coisas[id-1].devo = 1;
		}
	}while(op != 0);


}

void cadastrar_coisas()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int op, op_aux, aux;
	char coisa[30];
	char nome_emprestou[50];
	char nome_emprestado[50];
	int data_emp[3];
	int data_dev[3];
	int i;

	do{
		setbuf(stdin, NULL);
		printf("\nItem Emprestado: ");
		fgets(coisa, sizeof(coisa), stdin);

		printf("Nome da pessoa que emprestou: ");
		fgets(nome_emprestou, sizeof(nome_emprestou), stdin);

		printf("Nome da pessoa a quem foi emprestado: ");
		fgets(nome_emprestado, sizeof(nome_emprestado), stdin);

		printf("\n1 - Digitar data do emprestimo\n0 - Usar data atual para cadastra o Item\n");
		scanf("%d", &aux);
		if(aux == 1)
		{
			printf("\nDia, mes e ano que foi emprestado: ");
			scanf("%d%d%d", &data_emp[0], &data_emp[1], &data_emp[2]);
			printf("Data cadastrada!\n");
		}
		else
		{
			data_emp[0] = tm.tm_mday;
			data_emp[1] = tm.tm_mon + 1;
			data_emp[2] = tm.tm_year + 1900;
			printf("Data cadastrada!\n");
		}

		printf("\nDeseja cadastra data de devolucao?\n1 - sim\n0 - nao\n");
		scanf("%d", &op_aux);
		if (op_aux)
		{
			printf("\nDia, mes e ano da devolucao: ");
			scanf("%d%d%d", &data_dev[0], &data_dev[1], &data_dev[2]);
			printf("Data cadastrada!\n");
		}
		else
		{
			data_dev[0] = 0;
			data_dev[1] = 0;
			data_dev[2] = 0;
			printf("Data cadastrada!\n");
		}

		for (i = 0; i < MAX_COISAS; ++i)
		{
			if (coisas[i].ativo == 0)
			{
				strcpy(coisas[i].coisa, coisa);

				strcpy(coisas[i].nome_emprestou, nome_emprestou);

				strcpy(coisas[i].nome_emprestado, nome_emprestado);

				coisas[i].data_emp[0] = data_emp[0];
				coisas[i].data_emp[1] = data_emp[1];
				coisas[i].data_emp[2] = data_emp[2];
				
				coisas[i].data_dev[0] = data_dev[0];
				coisas[i].data_dev[1] = data_dev[1];
				coisas[i].data_dev[2] = data_dev[2];
				

				coisas[i].ativo = 1;

				break;

			}
		}

		printf("\n1- continuar\n0 - sair\n");
		scanf("%d", &op);
	}while(op != 0);
}

void remover_coisas()
{
	int i, op;
	int id;
	listar();
	do{
		printf("\nDigite o ID do item para remover: ");
		scanf("%d", &id);
		--id;
		coisas[id].ativo = 0;
		
		printf("\n1 - continuar removendo\n0 - sair\n");
		scanf("%d", &op);
	}while(op != 0);
}

void printar(int i)
{
	printf("\nID: %d\n", i+1);
	printf("Item: %s\n", coisas[i].coisa);
	printf("De: %s\n", coisas[i].nome_emprestou);
	printf("Para: %s\n", coisas[i].nome_emprestado);
	printf("Data de emprestimo: %d/%d/%d\n", coisas[i].data_emp[0], coisas[i].data_emp[1], coisas[i].data_emp[2]);
	printf("Data de devolucao: %d/%d/%d\n", coisas[i].data_dev[0], coisas[i].data_dev[1], coisas[i].data_dev[2]);
	printf("\n--------------------------\n");
}

void carregar()
{
	int i;
	FILE *lei;
	lei = fopen("coisas.txt", "r");
	//leitura
	for (i = 0; i < MAX_COISAS; ++i)
	{
		fscanf(lei, "%s", coisas[i].coisa);
		fscanf(lei, "%s", coisas[i].nome_emprestou);
		fscanf(lei, "%s", coisas[i].nome_emprestado);
		fscanf(lei, "%d %d %d", &coisas[i].data_emp[0], &coisas[i].data_emp[1], &coisas[i].data_emp[2]);
		fscanf(lei, "%d %d %d", &coisas[i].data_dev[0], &coisas[i].data_dev[1], &coisas[i].data_dev[2]);
		fscanf(lei, "%d", &coisas[i].devo);
		fscanf(lei, "%d", &coisas[i].ativo);
	}
	fclose(lei);
}

void salvar()
{
	int i;
	FILE *sal;
	sal = fopen("coisas.txt", "w");
	for (i = 0; i < MAX_COISAS; ++i)
	{
		fprintf(sal, "%s\n", coisas[i].coisa);
		fprintf(sal, "%s\n", coisas[i].nome_emprestou);
		fprintf(sal, "%s\n", coisas[i].nome_emprestado);
		fprintf(sal, "%d %d %d \n", coisas[i].data_emp[0], coisas[i].data_emp[1], coisas[i].data_emp[2]);
		fprintf(sal, "%d %d %d \n", coisas[i].data_dev[0], coisas[i].data_dev[1], coisas[i].data_dev[2]);
		fprintf(sal, "%d \n", coisas[i].devo);
		fprintf(sal, "%d \n", coisas[i].ativo);
	}
	fclose(sal);
}

void editar()
{
	int i, op, id;
	listar();
	
	do
	{
		printf("Digite o id do Item para editar\n");
		scanf("%d", &id);
		--id;

		setbuf(stdin, NULL);
		printf("\nItem Emprestado: ");
		fgets(coisas[id].coisa, sizeof(coisas[id].coisa), stdin);

		printf("Nome da pessoa que emprestou: ");
		fgets(coisas[id].nome_emprestou, sizeof(coisas[id].nome_emprestou), stdin);

		printf("Nome da pessoa a quem foi emprestado: ");
		fgets(coisas[id].nome_emprestado, sizeof(coisas[id].nome_emprestado), stdin);

		printf("\nDia, mes e ano que foi emprestado: ");
		scanf("%d%d%d", &coisas[id].data_emp[0], &coisas[id].data_emp[1], &coisas[id].data_emp[2]);

		printf("\nDia, mes e ano da devolucao: ");
		scanf("%d%d%d", &coisas[id].data_dev[0], &coisas[id].data_dev[1], &coisas[id].data_dev[2]);

		printf("\nItem editado com sucesso!\n");
		printf("\n1 - Para editar outro item\n0 - Para sair\n");
		scanf("%d", &op);

	}while(op != 0);

}