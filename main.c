#include <stdio.h>
#include <stdlib.h>

//definindo se e WIN32 ou UNIX
#if defined(_WIN32) || defined(WIN32)
#define LIMPA_TERM system("cls")
#else
#define LIMPA_TERM system("clear")
#endif

int opcoes(void)
{
	printf("\n1 - Emprestadas\n2 - Atrasados\n3 - Devolvidos\n0 - Fechar Programa\n\n");
}

int main(int argc, char const *argv[])
{
	//variables
	int escolha;

	//menu
	LIMPA_TERM;
	opcoes();
	while(1)
	{
		scanf("%d", &escolha);

		switch(escolha)
		{
			
			case 0:
			exit(0);
			break;
			
			case 1:
			LIMPA_TERM;
			printf("\nEmprestadas\n");
			opcoes();
			break;

			case 2:
			LIMPA_TERM;
			printf("\nAtrasados\n");
			opcoes();
			break;

			case 3:
			LIMPA_TERM;
			printf("\nDevolvidos\n");
			opcoes();
			break;

			default:
			LIMPA_TERM;
			printf("\nInvalid Value\n");
			opcoes();

		}
	}

	return 0;
}