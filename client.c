#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>


#define SERVER_IP "127.0.0.1"
#define BYTE 1024
#define PORTA 8585
#define TITULO "\n    ############### BeM VinDo ###############\n\n"

void imprimirAguarde(void);

/************************
*          MAIN         *
************************/
int main ()
{

    char mensagem[BYTE], *loc;
    int tbuf, skt, escolha;
    struct sockaddr_in serv;
    system("clear");

    /**INICIALIZA ESTRUTURA SOCKETS*/
    skt = socket(AF_INET, SOCK_STREAM, 0);
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(SERVER_IP);
    serv.sin_port = htons (PORTA);
    memset (&(serv.sin_zero), 0x00, sizeof (serv.sin_zero));

    /**INICIA COMUNICAÇÃO COM SERVIDOR*/
    while(connect (skt, (struct sockaddr *)&serv, sizeof (struct sockaddr)) != 0){
        imprimirAguarde();      ///AGUARDA SERVIDOR SE COMUNICAR
    }
    printf(">> A Conexao com o Servidor %s foi estabelecida na porta %d \n\n",SERVER_IP,PORTA);
    printf(">> Envie /x pra sair \n\n");

    int out;
    int in;
    int inteiro;

    while(1)
    {
        //escolha para receber mensagem inteiro ou de texto
        recv(skt, &in, 4, 0);

        if (in == 1)//inteiro
        {
            recv(skt, &in, 4, 0);
            printf("%d", in);
        }
        else if (in == 0)//texto
        {
            recv(skt, mensagem, BYTE, 0);
            printf("%s", mensagem);
        }
        else if (in == 3)
        {
            system("clear");
        }

        //vendo que o programa espera receber int ou char
        recv(skt, &in, 4, 0);

        if (in == 1)//inteiro
        {
            scanf("%d", &out);
            send(skt, &out, 4, 0);
        }
        else if (in == 0)//texto
        {
            setbuf(stdin, NULL);
            fgets(mensagem, BYTE, stdin);
            send(skt, mensagem, BYTE, 0);
        }

    }
    /**RECEBE MENSAGEM DO SERVIDOR*
    tbuf = recv (skt, mensagem, BYTE, 0);
    mensagem[tbuf] = 0x00;
    printf (">: %s\n",mensagem);

    /**ENVIA MENSAGEM PARA O SERVIDOR*
    strcpy(mensagem, "Cliente diz: olá!!!");
    send(skt, mensagem, strlen(mensagem), 0 );


    /**LOOP DE COMUNICAÇÃO ENTRE CLIENTE E SERVIDOR*
    do{
        ///envia
        printf("> ");
        fgets(mensagem, sizeof(mensagem), stdin);
        send(skt, mensagem, strlen(mensagem), 0);

        ///recebe
        tbuf = recv (skt, mensagem, BYTE, 0);
        mensagem[tbuf] = 0x00;
        printf (">: Servidor diz: %s\n",mensagem);

    }while(strcmp(mensagem,"/x")!= 0);    ///COMUNICAÇÃO SE ENCERRA QUANDO USUARIO DIGITAR /X
    */


    /**FINALIZA CONEXÃO*/
    close(skt);
    printf (">>A conexao com o servidor foi finalizada!!!\n\n");
    exit(0);
}



/**************************************************************
*   FUNÇÃO RESPOSÁVEL POR IMPRIMIR MENSAGER NA TELA           *
*   ENQUANTO AGUARDA ALGUM SERVIDOR ESTABELECER COMUNICAÇÃO   *
***************************************************************/
void imprimirAguarde(){
    int i=0;
    char dot[12] = "";
    for(i=0; i<4;i++){
        system("clear");
        printf(TITULO);
        printf("\n\nProcurando servidor.");
        printf("\nAguarde %s\n\n", dot);
        strcat(dot,".");
        sleep(1);
    }
    strcpy(dot, "");
}