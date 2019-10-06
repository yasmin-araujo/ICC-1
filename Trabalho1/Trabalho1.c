#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void tela_jogo(int fase, int num_seq)//(int fase, int num_seq, char nivel[])
{
    int seq[num_seq][fase]; //matriz das sequencias
    int resp[fase]; //vetor das respostas
    int ch = 3; //numero de chances
    bool errou = false; //flag para marcar se usuario errou
    int pontos = 0, pnt = 0;
	int i, j;
	
	//para cada uma das sequencias
    for(i = 0; i < num_seq; i++)
    {
    	//pontuação máxima que pode ganhar
        pontos += fase * 10;
        pnt = 0;

		//gera sequencia
		for(j = 0; j < fase; j++)
        {
            seq[i][j] = rand() % 10;
        }
        
        do
        {
	        //system("cls");
	        //printf("\n\n\n\tNIVEL %s\n\n", nivel);
	        printf("Sequencia #%d:\n",i+1);
	        
			//mostra a sequencia
	        for(j = 0; j < fase; j++)
	        {
	            printf("%d", seq[i][j]);
				
				//coloca espaço entre os numeros da seq
	            //if(j < (fase-1))
	                printf(" ");
	        }
	        //printf("\n\n\t");
	        
	        /*
	        if (fase == 9) //se nivel dificil então tempo = 10
	        	sleep(fase+1);
	        else
	        	sleep(fase);
	        */
				
	        //system("cls");
        
            errou = false; //inicializa errou
        	//printf("\n\n\n\tNIVEL %s\n\n", nivel);
            //printf("\nSequencia #%d",i+1);
            
            //pega respostas
            for(j = 0; j < fase; j++)
            {
                scanf(" %d", &resp[j]);
                
            	//para cada resposta compara com o valor da seq, se for diferente ativa flag errou e dinimui os pontos
                if (resp[j] != seq[i][j])
                {
                    errou = true;
                    pnt += 10;
                    if(pnt <= (fase * 10))
                    	pontos -= 10;
                }
            }
            
            if (pontos < 0) //não deixa os pontos serem negativos
                pontos = 0;

			//se flag errou foi ativada perde uma chance e mostra as estatisticas
            if(errou)
            {
                ch--;
                printf("\nIncorreto! \nTentativas disponiveis: %d \nPontuacao: %d\n", ch, pontos);
                //getchar();
                //getchar();
                //system("cls");
            }
            else //se não errou nenhum numero vai para a proxima sequencia
            {       					    
                printf("\nCorreto! \nTentativas disponiveis: %d \nPontuacao: %d\n", ch, pontos);
                //getchar();
                //getchar();
                break;
            }
            
        } while (ch > 0); //mantem na mesma sequencia enquanto usuario estiver errando e tiver chances disponiveis

		//caso o usuario perca todas as chances
        if(ch == 0)
        {
            printf("Nao ha mais tentativas disponiveis... Tente novamente!");
            return;            
        }
    }
	//caso acerte todas as sequencias com chances sobrando
	printf("Otima memoria! Parabens!!! ");
}

void tela_inicial()
{
    int num;
    char nivel;

    //system("cls");
    //printf("\n\n\n\tTRABALHO 1 - MEMORIZANDO SEQUENCIAS\n");
    
    //alimenta a seed do srand
    //printf("\n\n\n\tDigite um numero: ");
    scanf("%d", &num);
    srand(num);
    
    //printf("\n\t- F (facil)\n\t- I (intermediario)\n\t- A (avancado)\n\n");
    //printf("\tSelecione um nivel: ");
    scanf("%c", &nivel);
    scanf("%c", &nivel);

    if(nivel == 'F' || nivel == 'f')
    {
    	//chama função mandando como parametro o tamanho que cada seq terá
        tela_jogo(5, 3);//(5, 3, "FACIL"); //manda tamanho, num de seq e nome do nivel
    }
    else if(nivel == 'I' || nivel == 'i')
    {
        tela_jogo(7, 4);//(7, 4, "INTERMEDIARIO");
    }
    else if(nivel == 'A' || nivel == 'a')
    {
        tela_jogo(9, 5);//(9, 5, "DIFICIL");
    }
}

int main()
{
    tela_inicial();

    return 0;
}
