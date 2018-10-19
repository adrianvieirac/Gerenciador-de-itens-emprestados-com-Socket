#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ALUNOS 50

typedef struct
{
	char nome[50];
	float notas[4];
	int ativo;
} Aluno;

Aluno alunos[MAX_ALUNOS];

void menu();
void cadastrar();
void remover();
void reprovados();
void pesquisar();
void listar();

int main()
{
	menu();
	return 0;
}

void menu()
{
	int op;

	while(1)
	{
		system("clear");
		printf("CADASTRO DE ALUNOS\n\n");
		printf("1 - Cadastrar aluno\n2 - Remover aluno\n");
		printf("3 - Alunos Reprovados\n4 - Pesquisar aluno\n");
		printf("5 - Listar alunos\n0 - Fechar programa\n");
		scanf("%d", &op);

		switch(op)
		{			
			case 0:
			exit(0);
			break;
			
			case 1:
			system("clear");
			printf("1 - Cadastrar aluno\n");
			cadastrar();
			break;

			case 2:
			system("clear");
			printf("2 - Remover aluno\n");
			remover();
			break;

			case 3:
			system("clear");
			printf("3 - Alunos Reprovados\n");
			reprovados();
			break;

			case 4:
			system("clear");
			printf("4 - Pesquisar aluno\n");
			pesquisar();
			break;

			case 5:
			system("clear");
			printf("5 - Listar alunos\n");
			listar();
			break;

			default:
			system("clear");
			printf("valor invalido!\n\n");

		}
	}
}

void cadastrar()
{
	int i;
	char nome[50];
	float notas[4];
	int op;
	do
	{
		setbuf(stdin, NULL);
		printf("\nNome: ");
		fgets(nome, sizeof(nome), stdin);

		printf("\n1 Bimestre: ");
		scanf("%f", &notas[0]);

		printf("\n2 Bimestre: ");
		scanf("%f", &notas[1]);

		printf("\n3 Bimestre: ");
		scanf("%f", &notas[2]);

		printf("\n4 Bimestre: ");
		scanf("%f", &notas[3]);

		for (i = 0; i < MAX_ALUNOS; ++i)
		{
			if (alunos[i].ativo == 0)
			{
				alunos[i].notas[0] = notas[0];
				alunos[i].notas[1] = notas[1];
				alunos[i].notas[2] = notas[2];
				alunos[i].notas[3] = notas[3];

				strcpy(alunos[i].nome, nome);
				alunos[i].ativo = 1;

				break;

			}
		}

		printf("\n1- continuar\n0 - sair\n");
		scanf("%d", &op);
	}while(op != 0);
}

void remover()
{
	int i, op;
	int id;
	listar();
	do{
		printf("\nDigite o ID do aluno para remover: ");
		scanf("%d", &id);
		--id;
		alunos[id].ativo = 0;
		
		printf("\n1 - continuar removendo\n0 - sair\n");
		scanf("%d", &op);
	}while(op != 0);
}

void reprovados()
{
	int i;
	float media;
	for (i = 0; i < MAX_ALUNOS; ++i)
	{
		if(alunos[i].ativo == 1)
		{
			media = (alunos[i].notas[0] + alunos[i].notas[1] + alunos[i].notas[2] + alunos[i].notas[3]) / 4;
			if (media<7)
			{
				printf("\nID: %d\n", i+1);
				printf("Nome: %s\n", alunos[i].nome);
				printf("1 Bimestre: %.2f\n", alunos[i].notas[0]);
				printf("2 Bimestre: %.2f\n", alunos[i].notas[1]);
				printf("3 Bimestre: %.2f\n", alunos[i].notas[2]);
				printf("4 Bimestre: %.2f\n", alunos[i].notas[3]);
				printf("Media: %.2f\n", media );
				printf("\n--------------------------\n");
			}
		}
	}
	setbuf(stdin, NULL);
	getchar();
}

void pesquisar()
{
	int op, i, achou;
	char nome[50];
	do{	
		printf("\nDigite o nome do aluno\n");
		setbuf(stdin,NULL);
		fgets(nome, sizeof(nome), stdin);
		achou = 0;
		for (i = 0; i < MAX_ALUNOS; ++i)
		{
			if (strstr(alunos[i].nome, nome))
			{

				printf("\nID: %d\n", i+1);
				printf("Nome: %s\n", alunos[i].nome);
				printf("1 Bimestre: %.2f\n", alunos[i].notas[0]);
				printf("2 Bimestre: %.2f\n", alunos[i].notas[1]);
				printf("3 Bimestre: %.2f\n", alunos[i].notas[2]);
				printf("4 Bimestre: %.2f\n", alunos[i].notas[3]);
				printf("\n--------------------------\n");
				achou = 1;
			}
		}
		if (achou == 0)
		{
			printf("\nAluno nao encontrado!!\n");
		}
		printf("\n1 - Procurar outro aluno\n0 - sair\n");
		scanf("%d", &op);
	}while(op != 0);


}

void listar()
{
	int i;
	int op;
	printf("\nLISTA DE ALUNOS\n");
	
	for ( i = 0; i < MAX_ALUNOS; ++i)
	{
		if (alunos[i].ativo == 1)
		{
			printf("ID: %d\n", i+1);
			printf("Nome: %s\n", alunos[i].nome);
			printf("1 Bimestre: %.2f\n", alunos[i].notas[0]);
			printf("2 Bimestre: %.2f\n", alunos[i].notas[1]);
			printf("3 Bimestre: %.2f\n", alunos[i].notas[2]);
			printf("4 Bimestre: %.2f\n", alunos[i].notas[3]);
			printf("\n--------------------------\n");
		}
	}
	setbuf(stdin,NULL);
	getchar();
}
