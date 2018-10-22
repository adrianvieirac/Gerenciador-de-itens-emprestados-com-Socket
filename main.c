#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	char nome[50];
	int data_emp[3];
	int data_dev[3];
	int devo;
	int ativo;
} Coisa;

Coisa coisas[MAX_COISAS];

void menu();
void listar();
void atrasados();
void devolvidos();
void cadastrar_coisas();
void remover_coisas();

int opcoes(void)
{
	printf("COISAS EMPRESTADAS\n\n1 - Listar Itens\n2 - Itens Atrasados\n3 - Itens Devolvidos\n4 - Cadastrar Itens\n5 - Remover Itens\n0 - Fechar Programa\n\n");
}

int main(int argc, char const *argv[])
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

			default:
			LIMPA_TERM;
			printf("\nInvalid Value\n");
			opcoes();

		}
	}while(op != 0);

	return 0;
}

void menu()
{

}

void listar()
{
	int i;
	
	for ( i = 0; i < MAX_COISAS; ++i)
	{
		if (coisas[i].ativo == 1)
		{
			printf("\nID: %d\n", i+1);
			printf("Iten: %s\n", coisas[i].coisa);
			printf("Nome: %s\n", coisas[i].nome);
			printf("Data de emprestimo: %d/%d/%d\n", coisas[i].data_emp[0], coisas[i].data_emp[1], coisas[i].data_emp[2]);
			printf("Data de devolucao: %d/%d/%d\n", coisas[i].data_dev[0], coisas[i].data_dev[1], coisas[i].data_dev[2]);
			printf("\n--------------------------\n");
		}
	}
	setbuf(stdin,NULL);
	getchar();

}

void atrasados()
{

}

void devolvidos()
{
	int i, op, id;
	do{
		LIMPA_TERM;
		printf("LISTA DE ITENS\n");
		listar();
		printf("ITENS DEVOLVIDOS\n");
		for ( i = 0; i < MAX_COISAS; ++i)
		{
			if (coisas[i].ativo == 1 && coisas[i].devo == 1)
			{
				printf("\nID: %d\n", i+1);
				printf("Iten: %s\n", coisas[i].coisa);
				printf("Nome: %s\n", coisas[i].nome);
				printf("Data de emprestimo: %d/%d/%d\n", coisas[i].data_emp[0], coisas[i].data_emp[1], coisas[i].data_emp[2]);
				printf("Data de devolucao: %d/%d/%d\n", coisas[i].data_dev[0], coisas[i].data_dev[1], coisas[i].data_dev[2]);
				printf("\n--------------------------\n");
			}
		}

		printf("\n1- Marcar item como devolvido\n0 - Sair\n");
		scanf("%d", &op);
		if(op)
		{
			printf("\n\nDigite o ID do item para marca: ");
			scanf("%d", &id);
			coisas[id-1].devo = 1;
		}
	}while(op != 0);


}

void cadastrar_coisas()
{
	int op, op_aux;
	char coisa[30];
	char nome[50];
	int data_emp[3];
	int data_dev[3];
	int i;

	do{
		setbuf(stdin, NULL);
		printf("\nItem Emprestado: ");
		fgets(coisa, sizeof(coisa), stdin);

		printf("\nNome da pessoa a quem foi emprestado: ");
		fgets(nome, sizeof(nome), stdin);

		printf("\nDia, mes e ano que foi emprestado: ");
		scanf("%d%d%d", &data_emp[0], &data_emp[1], &data_emp[2]);

		printf("\nDeseja cadastra data de devolucao?\n1 - sim\n0 - nao\n");
		scanf("%d", &op_aux);
		if (op_aux)
		{
			printf("\nDia, mes e ano da devolucao: ");
			scanf("%d%d%d", &data_dev[0], &data_dev[1], &data_dev[2]);
		}
		else
		{
			data_dev[0] = 0;
			data_dev[1] = 0;
			data_dev[2] = 0;
		}

		for (i = 0; i < MAX_COISAS; ++i)
		{
			if (coisas[i].ativo == 0)
			{
				strcpy(coisas[i].coisa, coisa);

				strcpy(coisas[i].nome, nome);

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

}
