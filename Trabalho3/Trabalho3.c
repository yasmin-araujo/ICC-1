#include <stdio.h>
#include <stdlib.h>

typedef struct pnts{
    int x;
    int y;
    int cr; //criterio
} pontos;

//Função para calcular o módulo de um número
float modulo(float x)
{
    if( x < 0 )
        x = -x;
    return x;
}

//Função para liberar memória alocada dinamicamente
void libera(int **mat, int m)
{
    for(int i = 0; i < m; i++)
    {
        free(mat[i]);
    }
    free(mat);
}

//Função para calcular a média
float calcula_media(float med, int len)
{
    float res = 0.0;
    res = med / (float)len;
    return res;
}

//Função para demarcar regiões
void regiao(int x, int y, int cr, int **mat, int **aux, int r, int n, int m, float *media, int *len, int prim)
{
    if(aux[x][y] == 0) //não foi visitado
    {
        float med = calcula_media(*media, *len);
        float dif = modulo((float)mat[x][y] - med);

        if(dif <= cr)//confere se cumpre com o criterio
        {
            aux[x][y] = r; //marca 
            if(prim == 1) //se não for o primeiro item
            {
                //recalcula media
                *len += 1;
                *media += mat[x][y];
                //med = calcula_media(*media, *len);
            }
        }
        else
            return;

        //Verifica os lados
        if(x > 0)
            regiao(x-1, y, cr, mat, aux, r, n, m, media, len, 1); //olha para o de cima
        if (y < n - 1)
            regiao(x, y+1, cr, mat, aux, r, n, m, media, len, 1); //olha para o da direita
        if (x < m - 1)
            regiao(x+1, y, cr, mat, aux, r, n, m, media, len, 1); //olha para o de baixo
        if (y > 0)
            regiao(x, y-1, cr, mat, aux, r, n, m, media, len, 1); //olha para o da esqeurda
    }    
}

int main() 
{
    int k; //qtd de cliques
    char arq[15]; //nome do arquivo
    pontos *pnts; //pontos clicados pelo usuario

    char tipo_arq[5];
    int n, m; //tamanho da imagem // largura x altura
    int lixo; //variavel a ser ignorada
    int **mat, **aux, **borda; //matriz de pixels
    float media = 0; //soma dos numeros conquistados
    int len = 1; //numero parcial de regioes conquistadas
    int var;

    //Leitura de dados
    scanf("%s", arq);
    scanf("%d", &k);

    //Leitura de Arquivo
    FILE *fp;

    if((fp = fopen(arq, "r")) == NULL)
    {
        printf("Erro na abertura de arquivo!");
        exit(1);
    }

    //Inicialização da matriz com os valores de cada chamada (clique)
    pnts = (pontos*) malloc (k * sizeof(pontos));

    for (int i = 0; i < k; i++)
    {
        scanf("%d %d %d\n", &pnts[i].x, &pnts[i].y, &pnts[i].cr);
    }

    fscanf(fp, " %s\n", tipo_arq);
    fscanf(fp, "%d %d\n", &n, &m);
    fscanf(fp, "%d", &lixo);

    //Inicialização das matrizes com os valores da imagem, separação de região e bordas
    mat = (int **) malloc (m * sizeof(int*));
    aux = (int **) malloc (m * sizeof(int*));
    borda = (int **) malloc (m * sizeof(int*));

    for(int i = 0; i < m; i++)
    {
        mat[i] = (int*) malloc (n * sizeof(int));
        aux[i] = (int*) calloc (n, sizeof(int));
        borda[i] = (int*) calloc (n, sizeof(int));
    }

    //Armazenamento dos valores do arquivo
    for(int i = 0; i < m; i ++)
    {
        for(int j = 0; j < n; j++)
        {
            fscanf(fp, " %d\n", &mat[i][j]);
        }
    }

    fclose(fp);

    //Demarcação das Regiões
    for(int i = 0; i < k; i++) //para cada clique
    {
        len = 1;
        media = (float) mat[pnts[i].x][pnts[i].y];
        regiao(pnts[i].x, pnts[i].y, pnts[i].cr, mat, aux, i+1, n, m, &media, &len, 0);
    }
    
    //Demarcação das Bordas
    for(int i = 0; i < m; i ++)
    {
        for(int j = 0; j < n; j++)
        {
            var = aux[i][j];

            if(var > 0)
            {
                //Criação de variaveis para facilitar visualização
                int cima = 0;
                if(i > 0)
                    cima = aux[i-1][j];
                int dir = 0;
                if(j < n-1)
                    dir = aux[i][j+1];
                int baixo = 0;
                if(i < m-1)
                    baixo = aux[i+1][j];
                int esq = 0;
                if(j > 0)
                    esq = aux[i][j-1];

                //Verificação das bordas
                if(i > 0 && var != cima)//cima // diferente dele mesmo
                {
                    //Quando os dois valores são diferentes ambos são marcados na matriz borda como 1
                    borda[i][j] = 1;
                    borda[i-1][j] = 1;
                }
                if(j < n-1 && var != dir)//direita
                {
                    borda[i][j] = 1;
                    borda[i][j+1] = 1;
                }
                if(i < m-1 && var != baixo)//embaixo
                {
                    borda[i][j] = 1;
                    borda[i+1][j] = 1;
                }
                if(j > 0 && var != esq)//esquerda
                {
                    borda[i][j] = 1;
                    borda[i][j-1] = 1;
                }
            }
        }
    }

    //Impressão dos resultados na tela
    for(int i = 0; i < m; i ++)
    {
        for(int j = 0; j < n; j++)
        {
            //Imprime apenas os valores demarcados anteriormente como 1 na matriz de borda
            if(borda[i][j] == 1)
                printf("(%d, %d)\n", i, j);
        }
    }

    //Liberação da memoria alocada dinamicamente
    free(pnts);
    libera(mat, m);
    libera(aux, m);
    libera(borda, m);

    return 0;
}

// _____________@_______@ @_____
// _____________@@__@_@@@_____
// _____________@__@@_____@_____
// ____________@@_@__@_____@_____
// ___________@@@_____@@___@@@@@_____
// __________@@@@______@@_@____@@_____
// _________@@@@_______@@______@_@_____
// _________@@@@_______@_______@_____
// _________@@@@@_____@_______@_____
// __________@@@@@____@______@_____
// ___________@@@@@@@______@_____
// __@@@_________@@@@@_@_____
// @@@@@@@________@@_____
// _@@@@@@@_______@_____
// __@@@@@@_______@@_____
// ___@@_____@_____@_____
// ____@______@____@_____@_@@_____
// _______@@@@_@__@@_@_@@@@@_____
// _____@@@@@@_@_@@__@@@@@@@_____
// ____@@@@@@@__@@______@@@@@_____
// ____@@@@@_____@_________@@@_____
// ____@@_________@__________@_____
// _____@_________@_____
// _______________@_____