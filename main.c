#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	int op;
	while(1)
	{
		system("clear");
		printf("1 - Para usar o programa localmente\n2 - Para usar com socket\n0 - Para sair\n");
		scanf("%d", &op);
		if (op == 1)
		{
			system("./mainlocal.out");
		}
		else if(op == 2)
		{
			system("./mainserver.out");
		}
		else if(op == 0)
		{
			printf("Saindo\n");
			exit(0);
		}
		else
		{
			printf("Nenhuma das opcoes foi escolhida!\nTente novamente!!\n");
		}
	}
	return 0;
}