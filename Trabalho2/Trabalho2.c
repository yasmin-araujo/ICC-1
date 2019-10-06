#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dados
{
    char nome[20];
    char **palavra;
    int *cont;
    int num_palavras; //indice da ultima palavra registrada + 1
    char **frase;
    int num_frase; //num de sentenças do mesmo produto
};

struct perguntas
{
    char palavras[5][20];
};

//libera memória alocada dinamicamente
void libera(struct dados* bow, int num)
{
    for(int i = 0; i < num; i++)
    {
        for(int j = 0; j < bow[i].num_palavras; j++)
        {
            free(bow[i].palavra[j]);
        }
        for(int j = 0; j < bow[i].num_frase; j++)
        {
            free(bow[i].frase[j]);
        }
        free(bow[i].palavra);
        free(bow[i].cont);
        free(bow[i].frase);
    }
}

//confere se palavra já foi adicionada no BagOfWords
int confere_palavra(char aux[], struct dados bow, int n)
{
    for (int i = 0; i < n; i++)
    {
        if(strcmp(aux, bow.palavra[i]) == 0)
            return 1;
    }
    return 0;
}

//Incrementa contagem de determinada palavra no BOW
void add_contagem(char aux[], struct dados bow, int n)
{
    for (int i = 0; i < n; i++)
    {
        if(strcmp(aux, bow.palavra[i]) == 0)
        {
            bow.cont[i]++;
            return;
        }
    }
    printf("\nErro ao incrementar contagem\n");
    return;
}

//Confere se já existe um BOW para determinado produto
int confere_aparelho(char nome[], struct dados* bow, int n)
{
    for(int i = 0; i < n; i++)
    {
        if(strcmp(nome, bow[i].nome) == 0)
            return i;
    }
    return -1;
}

//Função para separar palavras da frase e incrementar contagem
struct dados separa_conta(struct dados bow, int novo)
{
    char aux[400] = "";
    int cont_aux = 0;
    int existe = 0;
    int num = novo;
    
    //armazena sentença
    char frase[400];
    scanf(" %[^Ø]Ø", frase);
    bow.frase[bow.num_frase - 1] = (char*) malloc((strlen(frase) + 1) * sizeof(char));
    strcpy(bow.frase[bow.num_frase - 1], frase);

    //separa as palavras
    for(int i = 0; i < strlen(frase); i++)
    {
        if(frase[i] == ' ')
        {
            aux[cont_aux] = '\0';
            existe = confere_palavra(aux, bow, num);
            //se a palavra não existir adiciona ela no BOW
            if(existe == 0)
            {
                bow.palavra[num] = (char*) malloc ((strlen(aux) + 1) * sizeof(char));
                strcpy(bow.palavra[num], aux);
                bow.cont[num]++;
                num++;
            }
            else //caso ela exista apenas incrementa a contagem
            {
                add_contagem(aux, bow, num);
            }
            cont_aux = 0;
        }
        else
        {
            aux[cont_aux] = frase[i];
            cont_aux++;
        }
    }
    bow.num_palavras = num;
    return bow;
}

//Atualização da BOG existente
void atualiza_bow(int indice, struct dados* bow)
{
    //aumenta espaço alocado para as frases
    int tam = bow[indice].num_palavras;
    bow[indice].num_frase++;
    bow[indice].frase = realloc(bow[indice].frase, bow[indice].num_frase * sizeof(char*));

    //atualiza com a versão nova
    bow[indice] = separa_conta(bow[indice], tam); //manda o bow do aparelho e o indice+1 da ultima palavra adicionada
}

//Confere se a polaridade da frase deve ou não ser invertida
int inverte(char frase[20][20], int i)
{
    char inverte[3][7]= {"nao", "nunca", "jamais"};
    int flag = 0;

    //para cada uma das 3 palavras anteriores na frase
    for(int t = 1; t < 4; t++)
    {
        if(i - t >= 0)
        {
            if(strcmp(frase[i - t], inverte[0]) == 0)
            {
                flag = 1;
                break;
            }
            else if(strcmp(frase[i - t], inverte[1]) == 0)
            {
                flag = 1;
                break;
            }
            else if(strcmp(frase[i - t], inverte[2]) == 0)
            {
                flag = 1;
                break;
            }
        }
    }
    return flag;
}

//Cálculo correspondente às perguntas de porcentagem positiva ou negativa
void porcentagem(int polar, char nome[20], struct dados* bow, int num)//polaridade pedida, nome do produto, BOW, número de bows existentes
{
    char bom[][15] = {"bom", "maravilhoso", "otimo", "sensacional", "excelente", "adorei", "gostei", "amei", "eficiente", "boa", "maravilhosa", "otima"};
    char ruim[][10] = {"detestei", "odiei", "ruim", "pessimo", "terrivel", "raiva", "odio", "pessima", "lento", "lenta", "fragil", "desisti"};

    int spos = 0;
    int sneg = 0;
    int stotal = 0;
    float res = 0.0;
    int indice = 0;

    //pega o indice do BOW do produto pedido
    for(int i = 0; i < num; i++)
    {
        if(strcmp(bow[i].nome, nome) == 0)
        {
            indice = i;
            break;
        }
    }

    for(int i = 0; i < bow[indice].num_frase; i++)//para cada frase do aparelho
    {
        int j = 0;//controle de caracter da frase
        char fraux[20][20]; //frase auxiliar que separa por palavra
        int t = 0;//controle de palavra
        int a = 0;//controle de caracter
        int num = 0;
        int flag = 0;

        //separa a frase em palavras
        while(bow[indice].frase[i][j] != '\0')
        {
            if(bow[indice].frase[i][j] == ' ')
            {
                fraux[t][a] = '\0';
                t++;
                a = 0;
                num++;
            }
            else
            {
                fraux[t][a] = bow[indice].frase[i][j];
                a++;
            }
            j++;
        }

        //compara as palavras
        for (int w = 0; w < t; w++) //para cada palavra da frase
        {
            for(int y = 0; y < 12; y++) //para cada palavra pré-definida
            {
                if(strcmp(bom[y], fraux[w]) == 0)
                {
                    if(inverte(fraux, w) == 1) //confere se deve inverter polaridade
                        sneg++;
                    else
                        spos++;
                    flag = 1;
                    break;
                }
                if(strcmp(ruim[y], fraux[w]) == 0)
                {
                    if(inverte(fraux, w) == 1)
                        spos++;
                    else
                        sneg++;
                    flag = 1;
                    break;
                }

            }

            if(flag == 1)
                break;
        }
    }

    stotal = spos + sneg;

    if(polar == 1)//positivos
    {
        res = ((float)spos/(float)stotal)*100;
    }
    else//negativos
    {
        res = ((float)sneg/(float)stotal)*100;
    }

    printf("\n%.1f%%", res);
}

//Cálculo correspondente às perguntas sobre a quantidade de determinada palavra
void quantidade(char nome[20], char palavra[20], struct dados* bow, int num) //nome do produto, palavra pedida, bow, número de bows existentes
{
    int indice = 0;
    int res = 0;

    //pega o indice do BOW do produto pedido
    for(int i = 0; i < num; i++)
    {
        if(strcmp(bow[i].nome, nome) == 0)
        {
            indice = i;
            break;
        }
    }

    //pega quantidade de ocorrencias
    for(int i = 0; i < bow[indice].num_palavras; i++)
    {
        if(strcmp(palavra, bow[indice].palavra[i]) == 0)
        {
            res = bow[indice].cont[i];
            break;
        }
    }

    printf("\n%d", res);
}

//Criador de BOWs novos
struct dados bowmaker(char nome[])
{
    struct dados bow;
    bow.palavra = (char**) malloc (100 * sizeof(char*));
    bow.cont = (int*) calloc (100, sizeof(int));
    bow.num_frase = 1;
    bow.frase = (char**) malloc(bow.num_frase * sizeof(char*));
  
    //define o nome do produto do bow
    strcpy(bow.nome, nome);
    
    //separação das palavras e contagem
    bow = separa_conta(bow, 0);

    return bow;
}

int main() 
{
    int s, p; //número de sentenças e número de perguntas
    scanf("%d %d", &s, &p);
    
    struct dados bow[s];
    struct perguntas perg[p];
    
    char nome[20];
    int existe = 0;
    int num_bow = 0;
    char frase[400];
    
    //Cria/atualiza Bow
    for(int i = 0; i < s; i++)
    {
        scanf("\n\r%[^;];", nome);
        existe = confere_aparelho(nome, bow, num_bow);
        if(existe < 0) //se não existe cria um novo bow
        {
            bow[num_bow] = bowmaker(nome);
            num_bow++;
        }
        else //senão apenas atualiza existente
        {
            atualiza_bow(existe, bow);
        }
    }

    //leitura das perguntas
    for(int i = 0; i < p; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            scanf("%s", perg[i].palavras[j]);
        }

        //armazena nome do produto em questão na pergunta
        char nome[20];
        strcpy(nome, perg[i].palavras[3]); 
        strcat(nome, " ");
        strcat(nome, perg[i].palavras[4]);
        strcat(nome, " ");
        int aux = strlen(nome);

        //confere qual o tipo de pergunta e chama sua respectiva função
        if(strcmp(perg[i].palavras[0], "quantos") == 0)
        {
            int pol = 0;
            if(strcmp(perg[i].palavras[1], "positivos") == 0)
                pol = 1;
            else if(strcmp(perg[i].palavras[1], "negativos") == 0)
                pol = 0;
            porcentagem(pol, nome, bow, num_bow);
        }
        else if(strcmp(perg[i].palavras[0], "palavra") == 0)
        {
            quantidade(nome, perg[i].palavras[1], bow, num_bow);
        }
    }

    //libera memoria alocada dinamicamente
    libera(bow, num_bow);

    return 0;
}