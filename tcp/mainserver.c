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
int aux, taux = sizeof(aux);

Coisa coisas[MAX_COISAS];

void menu();
void listar();
void atrasados();
void devolvidos();
void cadastrar_coisas();
void remover_coisas();
void printar();

int opcoes(void)
{
	aux = 0;
	send(skt, &aux, sizeof(aux), 0);
	printf("COISAS EMPRESTADAS\n\n1 - Listar Itens\n2 - Itens Atrasados\n3 - Itens Devolvidos\n4 - Cadastrar Itens\n5 - Remover Itens\n0 - Fechar Programa\n\n");
	strcpy(mensagem,"COISAS EMPRESTADAS\n\n1 - Listar Itens\n2 - Itens Atrasados\n3 - Itens Devolvidos\n4 - Cadastrar Itens\n5 - Remover Itens\n0 - Fechar Programa");
	send(skt, mensagem, strlen(mensagem), 0);
}

char mensagem[BYTE], *loc;
int pontarq, tbuf, skt, tskt, escolha;
struct sockaddr_in serv;

int main()
{
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
		//mandando tipo e recebendo opcao
		aux = 1;
		send(skt, &aux, sizeof(aux), 0);
		recv(skt, &op, sizeof(op), 0);

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

}

void listar()
{
	int i;
	
	for ( i = 0; i < MAX_COISAS; ++i)
	{
		if (coisas[i].ativo == 1)
		{
			printar(i);
			/*printf("\nID: %d\n", i+1);
			printf("Item: %s\n", coisas[i].coisa);
			printf("De: %s\n", coisas[i].nome_emprestou);
			printf("Para: %s\n", coisas[i].nome_emprestado);
			printf("Data de emprestimo: %d/%d/%d\n", coisas[i].data_emp[0], coisas[i].data_emp[1], coisas[i].data_emp[2]);
			printf("Data de devolucao: %d/%d/%d\n", coisas[i].data_dev[0], coisas[i].data_dev[1], coisas[i].data_dev[2]);
			printf("\n--------------------------\n");*/
		}
	}
	strcpy(mensagem, "Digite 0 para sair");
	send(skt, mensagem, BYTE, 0);
	aux = 1;
	send(skt, &aux, sizeof(aux), 0);
	recv(skt, &aux, sizeof(aux), 0);



}

void atrasados()
{
	int i;
	int data[3];
	int atrasado = 1;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	data[2] = tm.tm_year + 1900;
	data[1] = tm.tm_mon + 1;
	data[0] = tm.tm_mday;
	
	for(i = 0; i < MAX_COISAS; ++i)
	{
		if(data[2] <= coisas[i].data_dev[2] && data[1] <= coisas[i].data_dev[1] && data[0] <= coisas[i].data_dev[0])
		{
			atrasado = 0;
		}
		
		if(atrasado)
		{
			printf("\nID: %d\n", i+1);
			printf("Iten: %s\n", coisas[i].coisa);
			printf("De: %s\n", coisas[i].nome_emprestou);
			printf("Para: %s\n", coisas[i].nome_emprestado);
			printf("Data de emprestimo: %d/%d/%d\n", coisas[i].data_emp[0], coisas[i].data_emp[1], coisas[i].data_emp[2]);
			printf("Data de devolucao: %d/%d/%d\n", coisas[i].data_dev[0], coisas[i].data_dev[1], coisas[i].data_dev[2]);
			printf("\n--------------------------\n");
			
		}
		
	}
	setbuf(stdin, NULL);
	getchar();
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
				printf("De: %s\n", coisas[i].nome_emprestou);
				printf("Para: %s\n", coisas[i].nome_emprestado);
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

		printf("\nNome da pessoa que emprestou: ");
		fgets(nome_emprestou, sizeof(nome_emprestou), stdin);

		printf("\nNome da pessoa a quem foi emprestado: ");
		fgets(nome_emprestado, sizeof(nome_emprestado), stdin);

		printf("1 - Usar data atual para cadastra o Item\n0 - Digitar data do emprestimo\n");
		scanf("%d", &aux);
		if(aux == 1)
		{
			data_emp[0] = tm.tm_mday;
			data_emp[1] = tm.tm_mon + 1;
			data_emp[2] = tm.tm_year + 1900;
		}
		else
		{
			printf("\nDia, mes e ano que foi emprestado: ");
			scanf("%d%d%d", &data_emp[0], &data_emp[1], &data_emp[2]);
		}

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
	strcpy(mensagem, "\nID: ");
	send(skt, mensagem, BYTE, 0);
	aux = 2;
	send(skt, &aux, taux, 0);
	aux = htons(i+1);
	send(skt, &aux, taux, 0);
	printf("\nID: %d\n", i+1);
	printf("Item: %s\n", coisas[i].coisa);
	printf("De: %s\n", coisas[i].nome_emprestou);
	printf("Para: %s\n", coisas[i].nome_emprestado);
	printf("Data de emprestimo: %d/%d/%d\n", coisas[i].data_emp[0], coisas[i].data_emp[1], coisas[i].data_emp[2]);
	printf("Data de devolucao: %d/%d/%d\n", coisas[i].data_dev[0], coisas[i].data_dev[1], coisas[i].data_dev[2]);
	printf("\n--------------------------\n");


}