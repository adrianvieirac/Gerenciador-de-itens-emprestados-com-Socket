#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_COISAS 50

//tcp
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

//definindo se e WIN32 ou UNIX
#if defined(_WIN32) || defined(WIN32)
#define LIMPA_TERM system("cls")
#else
#define LIMPA_TERM system("clear")
#endif

#define PORTA 8585
#define BYTE 1024
#define SENDI aux = 1; send(skt, &aux, 4, 0); send(skt, &aux2, 4, 0)
#define SENDI2 aux = 2; send(skt, &aux, 4, 0)
#define SENDI3 aux = 3; send(skt, &aux, 4, 0)
#define SENDM aux = 0; send(skt, &aux, 4, 0); send(skt, mensagem, 1024, 0)
#define RECVI aux = 1; send(skt, &aux, 4, 0); recv(skt, &aux, 4, 0)
#define RECVM aux = 0; send(skt, &aux, 4, 0); recv(skt, mensagem, 1024, 0)

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

char mensagem[BYTE], *loc;
int pontarq, tbuf, skt, tskt, escolha;
struct sockaddr_in serv;
int aux, aux2 = sizeof(aux);

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
	strcpy(mensagem,"COISAS EMPRESTADAS\n\n1 - Listar Itens\n2 - Itens Atrasados\n3 - Itens Devolvidos\n4 - Cadastrar Itens\n5 - Remover Itens\n6 - Editar item\n0 - Fechar Programa\n\n");
	SENDM;
}

char mensagem[BYTE], *loc;
int pontarq, tbuf, skt, tskt, escolha;
struct sockaddr_in serv;

int main()
{
	carregar();
	system("clear");
	/**INICIALIZA ESTRUTURA SOCKETS*/
	skt = socket(AF_INET, SOCK_STREAM, 0);
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port = htons (PORTA);
	memset(&(serv.sin_zero),0x00,sizeof(serv.sin_zero));
	tskt = sizeof(struct sockaddr_in);

	printf("\n    ############### Server ###############\n\n");
	/**SOCKET INICIALIZA LISTENER PARA OUVIR PORTA*/
	bind(skt,(struct sockaddr *)&serv,sizeof(struct sockaddr));
	listen(skt,1);
	printf(">> Servidor esta escutando na porta %d\n\n",PORTA);

	/**RECEBE NOVAS CONEXÕES*/
	//O problema acho que está aqui, preciso receber mais uma conexão para o sistema continuar.
	skt = accept(skt,(struct sockaddr *)&serv,&tskt);
	printf(">> A Conexao com o endereco %s foi estabelecida\n\n",inet_ntoa(serv.sin_addr));


	/**ENVIA MENSAGEM PARA CLIENTE	
	strcpy(mensagem,"Servidor diz: olá!!!");
	send(skt,mensagem,strlen(mensagem), 0);
	/**RECEBE MENSAGEM DE CLIENTE
	tbuf = recv(skt, mensagem,BYTE, 0);
	mensagem[tbuf]=0x00;
	printf(">: %s\n",mensagem);*/


	menu();

	close(skt);
	printf(">> A Conexao com o host %s foi encerrada!!!\n\n",inet_ntoa(serv.sin_addr));

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
		SENDI3; SENDI2;
		opcoes();
		//recebendo opcao
		RECVI;
		op = aux;

		switch(op)
		{
			case 1:
			LIMPA_TERM;
			SENDI3; SENDI2;
			printf("LISTA DE ITENS\n");
			strcpy(mensagem, "LISTA DE ITENS\n"); SENDM; SENDI2;
			listar();
			break;

			case 2:
			LIMPA_TERM;
			SENDI3; SENDI2;
			printf("ITENS ATRASADOS\n");
			strcpy(mensagem, "ITENS ATRASADOS\n"); SENDM; SENDI2;
			atrasados();
			break;

			case 3:
			LIMPA_TERM;
			SENDI3; SENDI2;
			//printf("ITENS DEVOLVIDOS\n");
			//strcpy(mensagem, "ITENS DEVOLVIDOS\n"); SENDM; SENDI2;
			devolvidos();
			break;

			case 4:
			LIMPA_TERM;
			SENDI3; SENDI2;
			printf("CADASTRAR ITENS\n");
			strcpy(mensagem, "CADASTRAR ITENS\n"); SENDM; SENDI2;
			cadastrar_coisas();
			break;

			case 5:
			LIMPA_TERM;
			SENDI3; SENDI2;
			printf("REMOVER ITENS\n");
			strcpy(mensagem, "REMOVER ITENS\n"); SENDM; SENDI2;
			remover_coisas();
			break;

			case 6:
			LIMPA_TERM;
			printf("EDITAR ITENS\n");
			strcpy(mensagem, "EDITAR ITENS\n"); SENDM; SENDI2;
			editar();
			break;

			default:
			LIMPA_TERM;
			SENDI3; SENDI2;
			printf("VALOR INVALIDO\n");
			strcpy(mensagem, "VALOR INVALIDO\n"); SENDM; SENDI2;
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
		strcpy(mensagem, "Nenhum item Cadastrado\n"); SENDM; SENDI2;
	}
	printf("\nDigite algum numero para sair\n");
	strcpy(mensagem, "\nDigite algum numero para sair\n"); SENDM;
	RECVI;



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
		strcpy(mensagem, "Nenhum item atrasado\n"); SENDM; SENDI2;
	}

	printf("\nDigite algum numero para sair\n");
	strcpy(mensagem, "\nDigite algum numero para sair\n"); SENDM;
	RECVI;

}

void devolvidos()
{
	int i, op, id, cont = 0;
	do{
		LIMPA_TERM;
		SENDI3; SENDI2;
		printf("LISTA DE ITENS\n");
		strcpy(mensagem, "\nLISTA DE ITENS\n"); SENDM; SENDI2;
		listar();

		printf("\nITENS DEVOLVIDOS\n");
		strcpy(mensagem, "\nITENS DEVOLVIDOS\n"); SENDM; SENDI2;
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
			strcpy(mensagem, "Nenhum Iten devolvido\n"); SENDM; SENDI2;
		}

		printf("\n1- Marcar item como devolvido\n0 - Sair\n");
		strcpy(mensagem, "\n1- Marcar item como devolvido\n0 - Sair\n"); SENDM;
		RECVI; op = aux;
		printf("%d\n", op);
		if(op == 1)
		{
			printf("\n\nDigite o ID do item para marca: ");
			strcpy(mensagem, "\n\nDigite o ID do item para marca: "); SENDM;
			RECVI; id = aux;
			printf("%d\n", id);
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
		printf("\nItem Emprestado: ");
		strcpy(mensagem, "\nItem Emprestado: "); SENDM;
		RECVM; strcpy(coisa, mensagem);
		printf("%s", coisa);

		printf("Nome da pessoa que emprestou: ");
		strcpy(mensagem, "Nome da pessoa que emprestou: "); SENDM;
		RECVM; strcpy(nome_emprestou, mensagem);
		printf("%s", nome_emprestou);

		printf("Nome da pessoa a quem foi emprestado: ");
		strcpy(mensagem, "Nome da pessoa a quem foi emprestado: "); SENDM;
		RECVM; strcpy(nome_emprestado, mensagem);
		printf("%s", nome_emprestado);

		printf("\n1 - Digitar data do emprestimo\n0 - Usar data atual para cadastra o Item\n");
		strcpy(mensagem, "\n1 - Digitar data do emprestimo\n0 - Usar data atual para cadastra o Item\n"); SENDM;
		RECVI; //scanf("%d", &aux);
		if(aux == 1)
		{
			printf("\nDia, mes e ano que foi emprestado: ");
			strcpy(mensagem, "\nDia, mes e ano que foi emprestado: "); SENDM;
			RECVI; data_emp[0] = aux;
			SENDI2;
			RECVI; data_emp[1] = aux;
			SENDI2;
			RECVI; data_emp[2] = aux;
			printf("%d %d %d\n", data_emp[0], data_emp[1], data_emp[2]);
			//scanf("%d%d%d", &data_emp[0], &data_emp[1], &data_emp[2]);
			printf("Data cadastrada!\n");
			strcpy(mensagem, "Data cadastrada!\n"); SENDM; SENDI2;
		}
		else
		{
			printf("Data cadastrada!\n");
			strcpy(mensagem, "Data cadastrada!\n"); SENDM; SENDI2;
			data_emp[0] = tm.tm_mday;
			data_emp[1] = tm.tm_mon + 1;
			data_emp[2] = tm.tm_year + 1900;
		}

		printf("\nDeseja cadastra data de devolucao?\n1 - sim\n0 - nao\n");
		strcpy(mensagem, "\nDeseja cadastra data de devolucao?\n1 - sim\n0 - nao\n"); SENDM;
		RECVI; op_aux = aux; //scanf("%d", &op_aux);
		if (op_aux)
		{
			printf("\nDia, mes e ano da devolucao: ");
			strcpy(mensagem, "\nDia, mes e ano da devolucao: "); SENDM;
			RECVI; data_dev[0] = aux;
			SENDI2;
			RECVI; data_dev[1] = aux;
			SENDI2;
			RECVI; data_dev[2] = aux;
			printf("%d %d %d\n", data_dev[0], data_dev[1], data_dev[2]);
			//scanf("%d%d%d", &data_dev[0], &data_dev[1], &data_dev[2]);
			printf("Data cadastrada!\n");
			strcpy(mensagem, "Data cadastrada!\n"); SENDM; SENDI2;
		}
		else
		{
			data_dev[0] = 0;
			data_dev[1] = 0;
			data_dev[2] = 0;
			printf("Data cadastrada!\n");
			strcpy(mensagem, "Data cadastrada!\n"); SENDM; SENDI2;
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
		strcpy(mensagem, "\n1- continuar\n0 - sair\n"); SENDM;
		RECVI; op = aux; //scanf("%d", &op);
	}while(op != 0);
}

void remover_coisas()
{
	int i, op;
	int id;
	listar();
	do{
		printf("\nDigite o ID do item para remover: ");
		strcpy(mensagem, "\nDigite o ID do item para remover: "); SENDM;
		RECVI; id = aux - 1;//scanf("%d", &id);
		printf("%d\n", id);
		coisas[id].ativo = 0;
		
		printf("\n1 - continuar removendo\n0 - sair\n");
		strcpy(mensagem, "\n1 - continuar removendo\n0 - sair\n"); SENDM;
		RECVI; op = aux; //scanf("%d", &op);
		printf("%d\n", op);
	}while(op != 0);
}

void printar(int i)
{
	//enviar ID
	strcpy(mensagem, "\nID: "); SENDM; SENDI2;
	aux2 = i+1; SENDI; SENDI2;

	//enviar Item
	strcpy(mensagem, "\nItem: "); SENDM; SENDI2;
	strcpy(mensagem, coisas[i].coisa); SENDM; SENDI2;
	strcpy(mensagem, "\n"); SENDM; SENDI2;

	//enviar De
	strcpy(mensagem, "De: "); SENDM; SENDI2;
	strcpy(mensagem, coisas[i].nome_emprestou); SENDM; SENDI2;
	strcpy(mensagem, "\n"); SENDM; SENDI2;

	//enviar Para
	strcpy(mensagem, "Para: "); SENDM; SENDI2;
	strcpy(mensagem, coisas[i].nome_emprestado); SENDM; SENDI2;
	strcpy(mensagem, "\n"); SENDM; SENDI2;

	//enviar data de emprestimo
	strcpy(mensagem, "Data de emprestimo: "); SENDM; SENDI2;
	aux2 = coisas[i].data_emp[0]; SENDI;  SENDI2;
	strcpy(mensagem, "/"); SENDM; SENDI2;
	aux2 = coisas[i].data_emp[1];  SENDI; SENDI2;
	strcpy(mensagem, "/"); SENDM; SENDI2;
	aux2 = coisas[i].data_emp[2]; SENDI; SENDI2;
	strcpy(mensagem, "\n"); SENDM; SENDI2;

	//enviar data de devolucao
	strcpy(mensagem, "Data de devolucao: "); SENDM; SENDI2;
	aux2 = coisas[i].data_dev[0]; SENDI;  SENDI2;
	strcpy(mensagem, "/"); SENDM; SENDI2;
	aux2 = coisas[i].data_dev[1];  SENDI; SENDI2;
	strcpy(mensagem, "/"); SENDM; SENDI2;
	aux2 = coisas[i].data_dev[2]; SENDI; SENDI2;
	strcpy(mensagem, "\n"); SENDM; SENDI2;

	//enviar divisao
	strcpy(mensagem, "\n--------------------------\n"); SENDM; SENDI2;

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
		printf("Digite o id do Item para editar: ");
		strcpy(mensagem, "Digite o id do Item para editar: "); SENDM;
		RECVI; id = aux - 1;

		printf("\nItem Emprestado: ");
		strcpy(mensagem, "\nItem Emprestado: "); SENDM;
		RECVM; strcpy(coisas[id].coisa, mensagem);
		printf("%s", coisas[id].coisa);

		printf("Nome da pessoa que emprestou: ");
		strcpy(mensagem, "Nome da pessoa que emprestou: "); SENDM;
		RECVM; strcpy(coisas[id].nome_emprestou, mensagem);
		printf("%s", coisas[id].nome_emprestou);

		printf("Nome da pessoa a quem foi emprestado: ");
		strcpy(mensagem, "Nome da pessoa a quem foi emprestado: "); SENDM;
		RECVM; strcpy(coisas[id].nome_emprestado, mensagem);
		printf("%s", coisas[id].nome_emprestado);

		printf("\nDia, mes e ano que foi emprestado: ");
		strcpy(mensagem, "\nDia, mes e ano que foi emprestado: "); SENDM;
		RECVI; coisas[id].data_emp[0] = aux;
		SENDI2;
		RECVI; coisas[id].data_emp[1] = aux;
		SENDI2;
		RECVI; coisas[id].data_emp[2] = aux;
		printf("%d %d %d\n", coisas[id].data_emp[0], coisas[id].data_emp[1], coisas[id].data_emp[2]);

		printf("\nDia, mes e ano da devolucao: ");
		strcpy(mensagem, "\nDia, mes e ano da devolucao: "); SENDM;
		RECVI; coisas[id].data_dev[0] = aux;
		SENDI2;
		RECVI; coisas[id].data_dev[1] = aux;
		SENDI2;
		RECVI; coisas[id].data_dev[2] = aux;
		printf("%d %d %d\n", coisas[id].data_dev[0], coisas[id].data_dev[1], coisas[id].data_dev[2]);

		printf("\nItem editado com sucesso!\n");
		strcpy(mensagem, "\nItem editado com sucesso!\n"); SENDM; SENDI2;
		printf("\n1 - Para editar outro item\n0 - Para sair\n");
		strcpy(mensagem, "\n1 - Para editar outro item\n0 - Para sair\n"); SENDM;
		RECVI; op = aux;

	}while(op != 0);

}