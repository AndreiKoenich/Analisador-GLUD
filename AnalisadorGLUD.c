/*

Autor: Andrei Pochmann Koenich

O programa a seguir recebe uma gramatica linear unitaria a direita (GLUD) em um arquivo texto de entrada,
e uma lista de palavras em outro arquivo texto de entrada, e indica quais das palavras recebidas podem
ser geradas pela GLUD do primeiro arquivo. Por fim, o programa indica se a GLUD recebida gera uma linguagem
regular finita, infinita ou vazia. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>

#define DIMENSAONOME 100
#define DIMENSAOVARIAVEIS 100
#define DIMENSAOTERMINAIS 100
#define DIMENSAOLINHA 1000
#define MAXTRANSICAO_AFND 3
#define MAXTRANSICAO_AFD 100
#define TAMANHOMINIMOPRODUCAO 4
#define TAMANHOMAXIMOPRODUCAO 8
#define CARACTEREVAZIO "*"
#define CODIGOREJEITA -1
#define TAMANHOMAXIMOVARIAVEIS 12
#define CODIGOLINGUAGEM_VAZIA 1
#define CODIGOLINGUAGEM_FINITA 2
#define CODIGOLINGUAGEM_INFINITA 3
#define CODIGOACEITOUVAZIA 2
#define ESC 27

typedef struct transicoes_afnd /* Estrutura contendo cada nodo da lista de transicoes referente a cada estado do AFND. */
{
    char transicao[MAXTRANSICAO_AFND]; /* String contendo o nome da transicao. */
    struct transicoes_afnd *prox; /* Ponteiro para o proximo da lista. */

} TRANSICOES_AFND;

typedef struct transicoes_afd /* Estrutura contendo cada nodo da lista de transicoes referente a cada estado do AFD. */
{
    char transicao[MAXTRANSICAO_AFD]; /* String contendo o nome da transicao. */
    struct transicoes_afd *prox; /* Ponteiro para o proximo da lista. */

} TRANSICOES_AFD;

typedef struct /* Estrutura com informacoes de cada estado do AFND. */
{
    char estado; /* Nome do estado do AFND. */
    TRANSICOES_AFND *ptTransicoes; /* Ponteiro para o inicio da lista contendo as transicoes de cada estado. */
    char finais[DIMENSAOTERMINAIS]; /* String contendo os caracteres que levam para um estado de transicao final. */

} AUTOMATO_AFND;

typedef struct /* Estrutura com informacoes de cada estado do AFD. */
{
    char nome_estado[DIMENSAONOME]; /* Nome do estado do AFD. */
    TRANSICOES_AFD *ptTransicoes; /* Ponteiro para o inicio da lista contendo as transicoes de cada estado. */
    char finais[DIMENSAOTERMINAIS]; /* String contendo os caracteres que levam para um estado de transicao final. */

} AUTOMATO_AFD;

typedef struct /* Estrutura contendo informacoes referentes a GLUD recebida no arquivo de entrada. */
{
    char variaveis[DIMENSAOVARIAVEIS]; /* String contendo todos os nomes das variaveis da GLUD. */
    char terminais[DIMENSAOTERMINAIS]; /* String contendo todos os nomes dos terminais da GLUD. */
    char nome[DIMENSAONOME]; /* String contendo o nome da GLUD. */
    char inicial; /* Nome da variavel inicial da GLUD. */

} INFOGLUD;

void* inicializa_lista() /* Inicializa uma lista simplesmente encadeada. */
{
    return NULL; /* Retorna ponteiro nulo, inicializando a lista. */
}

void erro_entrada() /* Controle dos casos em que ocorre algum erro de sintaxe ou semantica no arquivo de entrada. */
{
    system("cls");
    printf("Erro de sintaxe ou semantica no arquivo de entrada. Fim do programa.\n");
    getch();
    exit(1); /* Encerra a execucao do programa, retornando um. */
}

void bubblesort(char v[]) /* Realiza a ordenacao dos caracteres de uma string em ordem crescente do ASCII. */
{
    int i, j;
    int dimensao = strlen(v);
    char aux;

    for (i = 0; i < dimensao; i++)
        for (j = 0; j < dimensao-i-1; j++)
        {
            if (v[j] > v[j+1])
            {
                aux = v[j];
                v[j] = v[j+1];
                v[j+1] = aux;
            }
        }
}

int contem_vetor(char vetor[], char caractere) /* Verifica se um determinado caractere esta presente em uma string. */
{
    for (int i = 0; vetor[i] != '\0'; i++) /* Percorre a string para encontrar o caractere. */
        if (vetor[i] == caractere)
            return 1;  /* Retorna 1, indicando que o caractere foi encontrado. */

    return 0; /* Retorna 1, indicando que o caractere nao foi encontrado. */
}

int contem_lista (TRANSICOES_AFND *ptLista, char palavra[]) /* Verifica se uma determinada transicao esta presente em uma lista de transicoes. */
{
    TRANSICOES_AFND *ptaux;

    for (ptaux = ptLista; ptaux != NULL; ptaux = ptaux->prox) /* Iteracao para percorrer a lista do inicio ao fim. */
        if (strcmp(ptaux->transicao,palavra) == 0)
            return 1; /* Retorna um, indicando que a transicao esta presente na lista. */

    return 0; /* Retorna zero, indicando que a transicao NAO esta presente na lista. */
}

void tira_repeticoes (char palavra[]) /* Ordena os caracteres de uma palavra, e remove repeticoes. */
{
    char saida[strlen(palavra)+1];
    bubblesort(palavra); /* Realiza a ordenacao dos caracteres de uma string em ordem crescente do ASCII. */

    saida[0] = palavra[0];

    int indice_saida = 0;

    for (int i = 0; palavra[i] != '\0'; i++) /* Iteracao para remover repeticoes. */
    {
        if (saida[indice_saida] != palavra[i])
        {
            indice_saida++;
            saida[indice_saida] = palavra[i];
        }
    }

    saida[indice_saida+1] = '\0';
    strcpy(palavra,saida); /* Atualiza a string com a palavra ordenada e sem repeticoes. */
}

TRANSICOES_AFND* insereFim_AFND (TRANSICOES_AFND *ptTransicoes, char transicao[]) /* Funcao para inserir uma nova transicao referente ao AFND no fim da lista encadeada. */
{
    TRANSICOES_AFND *novo;
    TRANSICOES_AFND *ptaux = ptTransicoes;

    novo = (TRANSICOES_AFND*) malloc (sizeof(TRANSICOES_AFND)); /* Aloca espaco de memoria para o novo nodo. */
    strcpy(novo->transicao,transicao);
    novo->prox = NULL;

    if (ptaux != NULL) /* Casos em que a lista nao esta vazia. */
    {
        for (ptaux = ptTransicoes; ptaux->prox != NULL; ptaux = ptaux->prox); /* Percorre a lista ate o final para inserir o novo nodo. */

        ptaux->prox = novo;
    }

    else /* Casos em que o novo elemento sera o primeiro e unico da lista. */
        ptTransicoes = novo;

    return ptTransicoes; /* Retorna o ponteiro para o inicio da lista, contendo todas as transicoes. */
}

TRANSICOES_AFD* insereFim_AFD (TRANSICOES_AFD *ptTransicoes, char transicao[]) /* Funcao para inserir uma nova transicao referente ao AFD no fim da lista encadeada. */
{
    TRANSICOES_AFD *novo;
    TRANSICOES_AFD *ptaux = ptTransicoes;

    novo = (TRANSICOES_AFD*) malloc (sizeof(TRANSICOES_AFD)); /* Aloca espaco de memoria para o novo nodo. */
    strcpy(novo->transicao,transicao);
    novo->prox = NULL;

    if (ptaux != NULL) /* Casos em que a lista nao esta vazia. */
    {
        for (ptaux = ptTransicoes; ptaux->prox != NULL; ptaux = ptaux->prox); /* Percorre a lista ate o final para inserir o novo nodo. */

        ptaux->prox = novo;
    }

    else /* Casos em que o novo elemento sera o primeiro e unico da lista. */
        ptTransicoes = novo;

    return ptTransicoes; /* Retorna o ponteiro para o inicio da lista. */
}

void destroi_listaAFND(TRANSICOES_AFND *ptLista) /* Funcao para desalocar o espaco de memoria ocupado por uma lista de transicoes de um estado do AFND. */
{
    TRANSICOES_AFND *ptaux;

    while (ptLista != NULL) /* Iteracao para percorrer a lista e liberar espacos de memoria. */
    {
        ptaux = ptLista;
        ptLista = ptLista->prox;
        free(ptaux);
    }

    free (ptLista);
}

void destroi_listaAFD(TRANSICOES_AFD *ptLista) /* Funcao para desalocar o espaco de memoria ocupado por uma lista de transicoes de um estado do AFD. */
{
    TRANSICOES_AFD *ptaux;

    while (ptLista != NULL) /* Iteracao para percorrer a lista e liberar espacos de memoria. */
    {
        ptaux = ptLista;
        ptLista = ptLista->prox;
        free(ptaux);
    }

    free (ptLista);
}

int controla_caractere(char caractere) /* Verifica se o caractere lido esta de acordo com a sintaxe correta do arquivo de entrada. */
{
    if (caractere == '\n' || caractere == '\t' || caractere == ' ')
        return 1;

    else if (caractere == '{' || caractere == '(')
        return 1;

    else if (caractere == '}' || caractere == ')')
        return 1;

    else
        return 0;
}

void controla_terminais(char linha[], INFOGLUD *informacoes, int *indice_leitura) /* Obtem os terminais, e verifica se os terminais inseridos no arquivo de entrada estao corretos. */
{
    int indice_terminais = 0; /* Indice para percorrer o vetor contendo os terminais. */
    int prox_virgula = -1; /* Variavel para indicar se o proximo caractere sera uma virgula. */

    while (linha[*indice_leitura] != '}') /* Iteracao para obter os terminais. */
    {
        if (prox_virgula == 1 && linha[*indice_leitura] != ',')
            erro_entrada();

        if (prox_virgula == -1)
        {
            if (controla_caractere(linha[*indice_leitura])) /* Verifica se o caractere lido esta de acordo com a sintaxe correta do arquivo de entrada. */
                erro_entrada();

            else if (linha[*indice_leitura] == ',' || linha[*indice_leitura] == '\0')
                erro_entrada();

            else
            {
                informacoes->terminais[indice_terminais] = linha[*indice_leitura]; /* Preenche o vetor contendo todos os terminais da GLUD. */
                indice_terminais++; /* Incrementa o indice do vetor dos terminais, para incluir os proximos terminais. */
            }
        }

        prox_virgula *= -1; /* Atualiza a variavel que indica se o proximo caractere sera uma virgula. */
        (*indice_leitura)++;
    }

    informacoes->terminais[indice_terminais] = '\0'; /* Atribui terminador nulo no vetor dos terminais, para delimitar o fim do vetor. */
    bubblesort(informacoes->terminais); /* Ordena os terminais que foram inseridos na string. */
}

void controla_variaveis(char linha[], INFOGLUD *informacoes, int *indice_leitura) /* Obtem as variaveis, e verifica se as variaveis inseridas no arquivo de entrada estao corretas. */
{
    int indice_variaveis = 0; /* Indice para percorrer o vetor contendo a variaveis. */
    int prox_virgula = -1; /* Variavel para indicar se o proximo caractere sera uma virgula. */

    while (linha[*indice_leitura] != '}') /* Iteracao para obter as variaveis. */
    {
        if (prox_virgula == 1 && linha[*indice_leitura] != ',')
            erro_entrada();

        if (prox_virgula == -1)
        {
            if (controla_caractere(linha[*indice_leitura])) /* Verifica se o caractere lido esta de acordo com a sintaxe correta do arquivo de entrada. */
                erro_entrada();

            else if (linha[*indice_leitura] == ',' || linha[*indice_leitura] == '\0')
                erro_entrada();

            else
            {
                informacoes->variaveis[indice_variaveis] = linha[*indice_leitura]; /* Preenche o vetor contendo todas variaveis da GLUD. */
                indice_variaveis++; /* Incrementa o indice do vetor das variaveis, para incluir as proximas variaveis. */
            }
        }

        prox_virgula *= -1; /* Atualiza a variavel que indica se o proximo caractere sera uma virgula. */
        (*indice_leitura)++;
    }

    informacoes->variaveis[indice_variaveis] = '\0'; /* Atribui terminador nulo no vetor das variaveis, para delimitar o fim do vetor. */
    bubblesort(informacoes->variaveis); /* Ordena as variaveis que foram inseridos na string. */

    if (strlen(informacoes->variaveis) > TAMANHOMAXIMOVARIAVEIS) /* Verifica se o usuario inseriu mais variaveis do que o permitido. */
        erro_entrada();
}

void controla_nomeproducao(char linha[], INFOGLUD *informacoes, int *indice_leitura) /* Obtem o nome da gramatica. */
{
    int indice_nome = 0;

    while (linha[*indice_leitura] != ',') /* Iteracao para obter o nome da gramatica. */
    {
        if (controla_caractere(linha[*indice_leitura])) /* Verifica sintaxe correta no arquivo de entrada. */
            erro_entrada(); /* Acusa erro de sintaxe. */

        informacoes->nome[indice_nome] = linha[*indice_leitura];
        indice_nome++; /* Incrementa o indice do nome da gramatica, para incluir os proximos caracteres. */
        (*indice_leitura)++;
    }

    informacoes->nome[indice_nome] = '\0'; /* Atribui terminador nulo na string do nome da gramatica, para delimitar o fim do vetor. */
}

int testa_repetido(char palavra[]) /* Funcao para verificar se ha caracteres repetidos em uma string. */
{
    for (int i = 0; palavra[i] != '\0'; i++)
        for (int j = 0; palavra[j] != '\0'; j++)
            if ((palavra[i] == palavra[j]) && i != j)
                return 1;

    return 0;
}

int compara_repeticoes(char palavra1[], char palavra2[]) /* Funcao para verificar se ha caracteres que aparecem nas variaveis e nos terminais ao mesmo tempo (erro). */
{
    for (int i = 0; palavra1[i] != '\0'; i++)
        for (int j = 0; palavra2[j] != '\0'; j++)
            if ((palavra1[i] == palavra2[j]))
                return 1;

    return 0;
}

void controla_primeiralinha(char linha[], INFOGLUD *informacoes) /* Funcao para obter as informacoes da primeira linha. */
{
    char aux_nome[DIMENSAONOME];
    char *aux_var, *aux_ter;
    int indice_leitura = 0;

    int achou_igual = 0; /* Flag para indicar se foi encontrada a primeira chave antes do sinal de igual. */

    for (int i = 0; linha[i] != '\0'; i++) /* Verifica se o sinal de igual esta presente antes da primeira chave. */
    {
        if (linha[i] == '=')
            achou_igual = 1;

        if (linha[i] == '{' && achou_igual != 1)
            erro_entrada();
    }

    if (achou_igual != 1)
        erro_entrada();

    while (linha[indice_leitura] != '=') /* Iteracao para obter o nome da gramatica. */
    {
        if (linha[indice_leitura] == ' ' || linha[indice_leitura] == '\t')
            erro_entrada();

        aux_nome[indice_leitura] = linha[indice_leitura];

        indice_leitura++; /* Atualiza o indice de leitura da linha do arquivo. */
    }

    if (indice_leitura == 0) /* Controle dos casos em que nenhum nome foi inserido. */
        erro_entrada();

    aux_nome[indice_leitura] = '\0'; /* Atribui terminador nulo na string do nome da gramatica, para delimitar o fim do vetor. */
    indice_leitura++; /* Atualiza o indice de leitura da linha do arquivo. */

    if (linha[indice_leitura] != '(' || linha[indice_leitura+1] != '{')
        erro_entrada();

    indice_leitura += 2; /* Atualiza o indice de leitura da primeira linha. */

    int prox_virgula = -1; /* Variavel para indicar se o proximo caractere sera uma virgula. */
    int indice_variaveis = 0;

    controla_variaveis(linha,informacoes,&indice_leitura); /* Obtem as variaveis, com controle de entradas invalidas. */

    indice_leitura++;

    /* A partir desse ponto, realiza-se controle caractere por caractere, verificando se ha erro de sintaxe. */

    if (linha[indice_leitura] != ',')
        erro_entrada();

    indice_leitura++;

    if (linha[indice_leitura] != '{')
        erro_entrada();

    indice_leitura++;
    controla_terminais(linha,informacoes,&indice_leitura); /* Obtem os terminais. */
    indice_leitura++;

    if (linha[indice_leitura] != ',')
        erro_entrada();

    indice_leitura++;
    controla_nomeproducao(linha,informacoes,&indice_leitura); /* Obtem o nome das regras de producao. */
    indice_leitura++;

    if (controla_caractere(linha[indice_leitura]))
        erro_entrada();

    else if (linha[indice_leitura] == ',' || linha[indice_leitura] == '\0')
        erro_entrada();

    informacoes->inicial = linha[indice_leitura]; /* Obtem a variavel inicial. */
    indice_leitura++;

    if (linha[indice_leitura] != ')')
        erro_entrada();

    indice_leitura++;

    if (linha[indice_leitura] != '\n')
        erro_entrada();
}

void controla_segundalinha(char linha[],INFOGLUD informacoes) /* Iteracao para obter a segunda linha, contendo o nome das producoes. */
{
    for (int i = 0; linha[i] != '\n' && linha[i] != '\0'; i++)
        if (linha[i] != informacoes.nome[i])
            erro_entrada(); /* Acusa erro de semantica. */
}

void controla_producoes(char linha[],AUTOMATO_AFND afnd[],INFOGLUD informacoes) /* Funcao para obter e controlar as regras de producao da GLUD. */
{
    int total_estados = strlen(informacoes.variaveis); /* Calcula o numero total de estados do AFND. */
    int indice_afnd = 0;
    int i = 0;

    char transicao[MAXTRANSICAO_AFND];

    if(strlen(linha) < TAMANHOMINIMOPRODUCAO) /* Verifica se o tamanho da linha de producao esta adequado (tamanho minimo). */
        erro_entrada();

    if(strlen(linha) > TAMANHOMAXIMOPRODUCAO) /* Verifica se o tamanho da linha de producao esta adequado (tamanho maximo). */
        erro_entrada();

    char variavel = linha[0];

    if (!contem_vetor(informacoes.variaveis,variavel)) /* Verifica se a variavel da producao esta presente no conjunto de variaveis. */
        erro_entrada();

    if(linha[1] != ' ' || linha[2] != '-' || linha[3] != '>')
        erro_entrada();

    if(linha[4] != ' ' && linha[4] != '\n' && strlen(linha) == 4)
    {
        while(informacoes.variaveis[i] != variavel)
            i++;

        strcat(afnd[i].finais,CARACTEREVAZIO);
        tira_repeticoes(afnd[i].finais);
        return;
    }

    if(linha[4] != ' ' && linha[4] != '\n' && strlen(linha) != 4)
        erro_entrada();

    else if (linha[4] != '\n') /* Casos em que a producao lida nao e vazia. */
    {
        if (!contem_vetor(informacoes.terminais,linha[5])) /* Verifica se o terminal lido de fato esta contido no conjunto de terminais. */
            erro_entrada(); /* Acusa erro de semantica. */

        if (linha[6] == '\n' || linha[6] == '\0')
        {
            while(informacoes.variaveis[i] != variavel)
                i++;

            transicao[0] = linha[5];
            transicao[1] = '\0';

            strcat(afnd[i].finais,transicao);
            tira_repeticoes(afnd[i].finais);
            return;
        }

        transicao[0] = linha[5];
        transicao[1] = '\0';

        if (linha[6] != '\n' && linha[6] != '\0')
        {
            if (!contem_vetor(informacoes.variaveis,linha[6]))
                erro_entrada();

            else if (linha[7] != '\n' && linha[7] != '\0')
                erro_entrada();

            transicao[1] = linha[6];
            transicao[2] = '\0';
        }
    }

    else
    {
        while(informacoes.variaveis[i] != variavel) /* Encontra o indice da nova variavel. */
            i++;

        strcat(afnd[i].finais,CARACTEREVAZIO); /* Atualiza o vetor de terminais que levam a um estado final para o estado em questao. */
        tira_repeticoes(afnd[i].finais); /* Remove repeticoes do vetor de terminais que levam a um estado final. */
        return;
    }

    i = 0;

    while(informacoes.variaveis[i] != variavel) /* Encontra o indice da nova variavel. */
        i++;

    afnd[i].ptTransicoes = insereFim_AFND(afnd[i].ptTransicoes,transicao); /* Insere a nova producao na estrutura. */
}

void controla_automato(INFOGLUD informacoes) /* Verifica se ha alguma inconsistencia nos dados coletados sobre o automato. */
{
    if (testa_repetido(informacoes.terminais)) /* Verifica se ha terminais repetidos no conjunto de terminais. */
        erro_entrada();

    else if (testa_repetido(informacoes.variaveis)) /* Verifica se ha variaveis repetidas no conjunto de variaveis. */
        erro_entrada();

    else if (compara_repeticoes(informacoes.terminais, informacoes.variaveis)) /* Verifica se o conjunto dos terminais e das variaveis possuem um mesmo caractere. */
        erro_entrada();

    if(!contem_vetor(informacoes.variaveis,informacoes.inicial)) /* Verifica se a variavel inicial de fato esta no conjunto das variaveis. */
        erro_entrada();
}

void converte_binario(int entrada, int bits, int saida[]) /* Converte um numero inteiro decimal de entrada em um numero binario de saida, embutido em um vetor. */
{
    unsigned int mascara = 1U << (bits-1);

    int aux[bits], indice_aux = 0;

    for (int i = 0; i < bits; i++)
    {
        aux[i] = (entrada & mascara) ? 1 : 0;
        entrada <<= 1;
    }

    for (int i = bits-1; i >= 0; i--) /* Inverte os bits, para melhor a ordenacao das variaveis da GLUD. */
    {
        saida[indice_aux] = aux[i];
        indice_aux++;
    }
}

void inicializa_afd(AUTOMATO_AFD afd[],AUTOMATO_AFND afnd[], char variaveis[]) /* Inicia a conversao do AFND em AFD. */
{
    int total_variaveis = strlen(variaveis);
    int total_estadosAFD = pow(2,total_variaveis)-1; /* Calcula a quantidade de estados do AFD. */
    int aux[total_variaveis];

    for (int i = 0; i < total_variaveis; i++)
    {
        aux[i] = 0;
        afd[i].finais[0] = '\0';
    }

    int indice_nome_estado = 0;
    int indice_afd = 0;

    for (int i = 1; i <= total_estadosAFD; i++)
    {
        indice_nome_estado = 0;
        converte_binario(i,total_variaveis,aux); /* Realiza a conversao do indice em binario, para ter uma referencia dos estados do AFND que irao compor o novo estado do AFD. */

        for (int j = 0; j < total_variaveis; j++)
        {
            if(aux[j] == 1)
            {
                afd[indice_afd].nome_estado[indice_nome_estado] = variaveis[j]; /* Realiza a criacao do novo estado do AFD, agrupando certos estados do AFND. */
                indice_nome_estado++;
            }
        }

        afd[indice_afd].ptTransicoes = inicializa_lista(); /* Inicializa a lista de transicoes para cada estado do novo AFD. */
        afd[indice_afd].nome_estado[indice_nome_estado] = '\0';
        bubblesort(afd[indice_afd].nome_estado); /* Realiza a ordenacao do nome do estado. */
        indice_afd++;
    }
}

void obtemnome_estado(char novo_estado[], TRANSICOES_AFND *ptTransicoes, char terminal) /* Funcao para obter o nome do novo estado do AFD, a partir do AFND. */
{
    TRANSICOES_AFND *ptaux;
    int indice_aux = 0;

    if (ptTransicoes == NULL) /* Controle dos casos em que um estado nao possui transicoes para outro estado. */
        return;

    for (ptaux = ptTransicoes; ptaux != NULL; ptaux = ptaux->prox) /* Percorre a lista de transicoes para obter o nome do novo estado. */
        if (ptaux->transicao[0] == terminal && ptaux->transicao[1] != '\0')
        {
            novo_estado[indice_aux] = ptaux->transicao[1];
            indice_aux++;
        }

    novo_estado[indice_aux] = '\0';
}

void preenche_afd(AUTOMATO_AFD afd[],AUTOMATO_AFND afnd[],INFOGLUD informacoes) /* Preenche o AFD com as informacoes necessarias, obtidas a partir do AFND. */
{
    int total_variaveis = strlen(informacoes.variaveis);
    int total_terminais = strlen(informacoes.terminais);
    int total_estadosAFD = pow(2,total_variaveis)-1; /* Calcula a quantidade total de estados do novo AFD. */

    char novo_estado[DIMENSAOVARIAVEIS];
    memset(novo_estado, 0, sizeof(novo_estado)); /* Inicializa todo o array contendo o nome do novo estado. */

    char nova_producao[DIMENSAOVARIAVEIS];
    memset(nova_producao, 0, sizeof(nova_producao)); /* Inicializa todo o array de caracteres contendo a producao. */

    for (int i = 0; i < total_estadosAFD; i++)
        memset(afd[i].finais, 0, sizeof(afd[i].finais)); /* Reinicia todo o array de caracteres da linha antes da proxima iteracao. */

    for (int i = 0; i < total_variaveis; i++)
    {
        for (int j = 0; j < total_terminais; j++)
        {
            obtemnome_estado(novo_estado,afnd[i].ptTransicoes, informacoes.terminais[j]); /* Determina o nome do novo estado do novo AFD. */

            for (int k = 0; k < total_estadosAFD; k++)
            {
                if (afd[k].nome_estado[0] == afnd[i].estado && afd[k].nome_estado[1] == '\0')
                {
                    if (novo_estado[0] != '\0')
                    {
                        nova_producao[0] = informacoes.terminais[j];
                        strcat(nova_producao,novo_estado);
                        afd[k].ptTransicoes = insereFim_AFD(afd[k].ptTransicoes,nova_producao); /* Atualiza as transicoes do novo estado do AFD sendo gerado. */
                        break;
                    }
                }
            }

            memset(novo_estado, 0, sizeof(novo_estado)); /* Reinicia todo o array contendo o nome do novo estado, antes da proxima iteracao. */
            memset(nova_producao, 0, sizeof(nova_producao)); /* Reinicia todo o array de caracteres contendo a producao, antes da proxima iteracao. */
        }
    }
}

void ajusta_producao(char nova_producao[],INFOGLUD informacoes) /* Ajusta o nome da producao, colocando o terminal na primeira posicao da string. */
{
    if (contem_vetor(informacoes.terminais,nova_producao[0])) /* Verifica se o primeiro caractere da string ja e um terminal. */
        return;

    char terminal;

    for (int i = 1; nova_producao[i] != '\0'; i++) /* Percorre o nome da producao ate achar o terminal. */
        if (contem_vetor(informacoes.terminais,nova_producao[i]))
        {
            terminal = nova_producao[i]; /* Salva o terminal da producao. */
            break;
        }

    char aux[DIMENSAOVARIAVEIS];
    aux[0] = terminal; /* Coloca o terminal no inicio da regra de producao. */
    int indice_aux = 1;

    for (int i = 0; nova_producao[i] != '\0'; i++) /* Copia o conteudo da producao para a nova string, com o terminal no inicio. */
    {
        if(nova_producao[i] != terminal)
        {
            aux[indice_aux] = nova_producao[i];
            indice_aux++;
        }
    }

    aux[indice_aux] = '\0';
    strcpy(nova_producao,aux); /* Atualiza a string antiga. */
}

void atualiza_afd_aux(AUTOMATO_AFD afd[],INFOGLUD informacoes, char terminal) /* Atualiza as regras de producao de cada estado do novo AFD, para um terminal recebido como parametro. */
{
    int total_variaveis = strlen(informacoes.variaveis);
    int total_estadosAFD = pow(2,total_variaveis)-1;

    char estado_composto[DIMENSAOVARIAVEIS];
    memset(estado_composto, 0, sizeof(estado_composto)); /* Reinicia todo o array de caracteres da linha antes da proxima iteracao. */

    TRANSICOES_AFD *ptaux;

    char nova_producao[DIMENSAOVARIAVEIS];
    memset(nova_producao, 0, sizeof(nova_producao)); /* Inicializa todo o array de caracteres. */

    for (int i = 0; i < total_estadosAFD; i++) /* Percorre cada um dos novos estados do novo AFD. */
    {
        if (strlen(afd[i].nome_estado) > 1) /* Verifica se o estado do novo AFD e um estado composto por estados do antigo AFND. */
        {
            strcpy(estado_composto,afd[i].nome_estado); /* Salva o nome do novo estado do AFD em string auxiliar. */

            for (int j = 0; j < strlen(estado_composto); j++) /* Percorre cada caractere do nome do estado referente ao AFD. */
            {
                for (int k = 0; k < total_estadosAFD; k++) /* Percorre cada um dos novos estados do novo AFD. */
                {
                    if (strlen(afd[k].nome_estado) == 1 && estado_composto[j] == afd[k].nome_estado[0])
                    {
                        for (ptaux = afd[k].ptTransicoes; ptaux != NULL; ptaux = ptaux->prox)
                        {
                            if (ptaux->transicao[0] == terminal)
                            {
                                strcat(nova_producao,ptaux->transicao); /* Atualiza o nome da producao referente ao estado do novo AFD. */
                                tira_repeticoes(nova_producao); /* Ordena os caracteres da producao, e remove repeticoes. */
                            }
                        }

                    }
                }
            }

            memset(estado_composto, 0, sizeof(estado_composto)); /* Reinicia todo o array de caracteres da linha antes da proxima iteracao. */
            ajusta_producao(nova_producao,informacoes); /* Ajusta a producao, colocando o terminal no inicio do seu nome, se necessario. */

            if(nova_producao[0] != '\0')
                afd[i].ptTransicoes = insereFim_AFD(afd[i].ptTransicoes,nova_producao); /* Insere a nova producao na lista de producoes do estados do novo AFD. */

            memset(nova_producao, 0, sizeof(nova_producao)); /* Reinicia todo o array de caracteres da linha antes da proxima iteracao. */
        }
    }
}

void atualiza_afd(AUTOMATO_AFD afd[],INFOGLUD informacoes) /* Atualiza as regras de producao de cada estado do novo AFD, para cada um dos terminais. */
{
    int total_terminais = strlen(informacoes.terminais);

    for(int i = 0; i < total_terminais; i++) /* Atualiza as regras de producao, mudando os terminais passados como parametro. */
        atualiza_afd_aux(afd,informacoes,informacoes.terminais[i]);
}

void ajusta_producoes_afd(AUTOMATO_AFD afd[], INFOGLUD informacoes) /* Ultimo controle de producoes do AFD, para ordenar o nome dos novos estados (compostos). */
{
     TRANSICOES_AFD *ptaux;

    int total_variaveis = strlen(informacoes.variaveis);
    int total_estadosAFD = pow(2,total_variaveis)-1;

    for (int i = 0; i < total_estadosAFD; i++) /* Percorre todo o AFD do inicio ao fim. */
        for(ptaux = afd[i].ptTransicoes; ptaux != NULL; ptaux = ptaux->prox) /* Percorre a lista de transicoes de um estado, do inicio ao fim. */
            tira_repeticoes((ptaux->transicao)+1); /* Ordena segundo a ordem lexicografica, a partir do segundo caractere. */
}

void atualiza_finais_afd(AUTOMATO_AFD afd[], AUTOMATO_AFND afnd[],INFOGLUD informacoes) /* Determina os caracteres que levam a um estado final, para cada estado do novo AFD gerado. */
{
    int total_variaveis = strlen(informacoes.variaveis);
    int total_estadosAFD = pow(2,total_variaveis)-1;

    char aux[DIMENSAONOME]; /* String auxiliar que contera o nome de cada estado. */
    memset(aux, 0, sizeof(aux)); /* Inicializa a string auxiliar que contera o nome de cada estado. */

    for (int i = 0; i < total_variaveis; i++)
    {
        aux[0] = afnd[i].estado; /* Obtem o nome do estado sendo analisado. */
        aux[1] = '\0';

        for (int j = 0; j < total_estadosAFD; j++)
        {
            if (strstr(afd[j].nome_estado,aux))
            {
                strcat(afd[j].finais,afnd[i].finais); /* Atualiza a string contendo o nomes dos caracteres que levam a um estado final. */
                tira_repeticoes(afd[j].finais); /* Remove repeticoes da string contendo o nomes dos caracteres que levam a um estado final, se necessario. */
            }
        }

        memset(aux, 0, sizeof(aux)); /* Reinicia todo o array de caracteres da linha antes da proxima iteracao. */
    }

    ajusta_producoes_afd(afd,informacoes); /* Ultimo controle de producoes do AFD, para ordenar o nome dos novos estados (compostos). */
}

int retorna_indice_nomeAFND(AUTOMATO_AFND afnd[], char nome, int dimensao) /* Recebe um nome de um estado, e retorna o indice do estado do AFND no vetor. */
{
    for (int i = 0; i < dimensao; i++) /* Iteracao para obter o valor do indice. */
        if  (afnd[i].estado == nome)
            return i; /* Retorna o indice correspondente ao estado, no vetor AFND. */

    return -1; /* Retorna -1, se o nome do estado nao foi encontrado. */
}

int retorna_indice_nomeAFD(AUTOMATO_AFD afd[], char nome[], int dimensao) /* Recebe um nome de um estado, e retorna o indice do estado do AFD no vetor. */
{
    for (int i = 0; i < dimensao; i++) /* Iteracao para obter o valor do indice. */
        if (strcmp(afd[i].nome_estado,nome) == 0)
            return i; /* Retorna o indice correspondente ao estado, no vetor AFD. */

    return -1; /* Retorna -1, se o nome do estado nao foi encontrado. */
}

int retorna_indice_terminal(char letra, char terminais[]) /* Recebe um caractere terminal, e retorna o indice do terminal no vetor. */
{
    int dimensao = strlen(terminais);

    for (int i = 0; i < dimensao; i++) /* Iteracao para obter o valor do indice. */
        if (letra == terminais[i])
            return i;

    return -1; /* Retorna -1, se o nome nao foi encontrado. */
}

void preenche_final(AUTOMATO_AFD afd[],int **afd_final, INFOGLUD informacoes) /* Preenche o vetor de inteiros que representa o novo AFD. */
{
    int total_variaveis = strlen(informacoes.variaveis);
    int total_estadosAFD = pow(2,total_variaveis);
    int total_terminais = strlen(informacoes.terminais);

    TRANSICOES_AFD *ptaux;

    for (int i = 0; i < total_estadosAFD; i++) /* Inicializa todo o vetor de inteiro referente ao novo AFD. */
        for (int j = 0; j < total_terminais; j++)
            afd_final[i][j] = CODIGOREJEITA;

    int indice_linha = 0;
    int indice_coluna = 0;

    for (int i = 0; i < total_estadosAFD-1; i++)
    {
        for (ptaux = afd[i].ptTransicoes; ptaux != NULL; ptaux = ptaux->prox) /* Obtem os indices das informacoes para preencher o vetor de inteiros do AFD. */
        {
            indice_linha = retorna_indice_nomeAFD(afd,(ptaux->transicao+1),total_estadosAFD-1); /* Obtem o indice referente ao nome do estado. */
            indice_coluna = retorna_indice_terminal(ptaux->transicao[0],informacoes.terminais); /* Obtem o indice referente ao nome do terminal. */

            afd_final[i][indice_coluna] = indice_linha; /* Preenche o vetor de inteiros do AFD com as informacoes obtidas na iteracao. */
        }
    }
}

int testa_palavra(AUTOMATO_AFD afd[], INFOGLUD informacoes, char palavra[]) /* Determina se o automato AFD aceita ou rejeita uma palavra. */
{
    int total_variaveis = strlen(informacoes.variaveis);
    int total_estadosAFD = pow(2,total_variaveis)-1;
    int total_terminais = strlen(informacoes.terminais);

    int estado = 0;
    char proximo[DIMENSAONOME];
    memset(proximo,0,sizeof(proximo));
    proximo[0] = '\0';

    TRANSICOES_AFD *ptaux;

    for (int i = 0; i < total_estadosAFD; i++)
        if(strlen(afd[i].nome_estado) == 1 && afd[i].nome_estado[0] == informacoes.inicial)
        {
            estado = i; /* Determina o estado inicial. */
            break;
        }

    if (palavra[0] == '\0' && strstr(afd[estado].finais,CARACTEREVAZIO)) /* Controle de aceitacao da palavra vazia. */
        return CODIGOACEITOUVAZIA; /* Retorna 2, indicando que a palavra vazia foi aceita. */

    char caractere;
    int achou = 0; /* Flag para indicar se foi encontrada uma transicao para o caractere da vez. */
    int i = 0;

    for (i = 0; palavra[i] != '\0'; i++)
    {
        caractere = palavra[i];
        memset(proximo,0,sizeof(proximo));
        proximo[0] = '\0';

        if (palavra[i+1] == '\0' && contem_vetor(afd[estado].finais,caractere)) /* Controle de aceitacao de casos como A -> a */
            return 1;

        for (ptaux = afd[estado].ptTransicoes; ptaux != NULL; ptaux = ptaux->prox)
            if (ptaux->transicao[0] == caractere)
            {
                achou = 1; /* Indica que foi encontrada uma transicao para o caractere da vez. */
                strcpy(proximo,(ptaux->transicao+1)); /* Atualiza o nome do proximo estado do automato a ser analisado. */
                break;
            }

        if (achou != 1)
            return 0;

        for (int j = 0; j < total_estadosAFD; j++)
            if(strcmp(afd[j].nome_estado,proximo) == 0)
            {
                estado = j; /* Encontra o indice no vetor AFD, do proximo estado a ser analisado. */
                break;
            }

        achou = 0; /* Reinicia a flag para a proxima iteracao. */
    }

    if(strstr(afd[estado].finais,CARACTEREVAZIO)) /* Verifica se a analise parou em um estado final, para aceitar a palavra. */
        return 1;

    else /* Rejeita a palavra, pois a analise parou em um estado nao-final. */
        return 0;
}

void recebe_palavras(AUTOMATO_AFD afd[], INFOGLUD informacoes) /* Recebe o arquivo contendo as palavras, e determina se elas pertencem ou nao a linguagem. */
{
    FILE *arq_palavras;

    int i = 0;

    char nome_arqpalavras[DIMENSAONOME];
    char linha[DIMENSAOLINHA], palavra[DIMENSAONOME];
    printf("\nDigite o nome do arquivo de entrada contendo a lista de palavras:\n"); /* Obtem o nome do arquivo de entrada do usuario. */
    gets(nome_arqpalavras);
    system("cls");

    if (!(arq_palavras = fopen(nome_arqpalavras,"r"))) /* Abertura do arquivo de entrada com a lista de palavras, com controle dos casos em que nao e possivel abri-lo. */
    {
        system("cls");
        printf("Erro na abertura do arquivo texto contendo a lista de palavras. Fim do programa.\n");
        getch();
        exit(1); /* Encerra a execucao do programa, retornando um. */
    }

    int fora_alfabetoentrada = 0; /* Flag para indicar se uma palavra foi rejeitada por possuir um caractere fora do alfabeto de entrada. */
    int decisao = 0; /* Flag para indicar se uma palavra foi aceita ou rejeitada. */
    int aceitou = 0; /* Flag para indicar se ao menos uma palavra foi aceita. */

    printf("PALAVRAS DO ARQUIVO DE ENTRADA QUE PODEM SER GERADAS PELA GLUD:\n\n");

    while (!feof(arq_palavras)) /* Percorre o arquivo contendo as palavras, do inicio ao fim. */
    {
        fora_alfabetoentrada = 0; /* Reinicia a flag. */
        fgets(linha,DIMENSAOLINHA,arq_palavras); /* Obtem a palavra da linha. */

        for(i = 0; i < strlen(linha) && linha[i] != '\n'; i++)
        {
            if(linha[i] != '\0' && linha[i] != '\n') /* Realiza a adequacao da palavra antes do teste de aceitacao. */
                palavra[i] = linha[i];

            if(!contem_vetor(informacoes.terminais,palavra[i])) /* Verifica se uma palavra deve ser rejeitada por possuir um caractere fora do alfabeto de entrada. */
                fora_alfabetoentrada = 1;
        }

        palavra[i] = '\0';

        decisao = testa_palavra(afd,informacoes,palavra); /* Testa se o automato aceita a palavra da linha. */

        if (fora_alfabetoentrada == 0 && decisao != 0) /* Indica que uma determinada palavra foi aceita. */
        {
            aceitou = 1;

            if (decisao == CODIGOACEITOUVAZIA) /* Verifica se a palavra vazia foi aceita. */
                printf("<palavra vazia>\n");

            else /* Imprime a palavra que foi aceita, que nao e vazia. */
                printf("%s\n", palavra);
        }

        memset(linha, 0, sizeof(linha)); /* Reinicia todo o array de caracteres da linha antes da proxima iteracao. */
        memset(palavra, 0, sizeof(palavra)); /* Reinicia todo o array de caracteres da palavra antes da proxima iteracao. */
    }

    if (aceitou == 0)
        printf("Nenhuma palavra do arquivo de entrada pode ser gerada pela GLUD inserida.\n");

    fclose(arq_palavras); /* Fecha o arquivo contendo a lista de palavras, apos o fim da leitura. */
}

int linha_vazia(char linha[]) /* Verifica se ha alguma linha vazia no arquivo de entrada. */
{
    for (int i = 0; linha[i] != '\n' && linha[i] != '\0'; i++)
        if (linha[i] != ' ' || linha[i] != '\t')
            return 0; /* Retorna zero, indicando que a linha nao e vazia. */

    return 1; /* Retorna um, indicando que a linha e vazia. */
}

void preenche_estados(AUTOMATO_AFND afnd[],INFOGLUD informacoes, int total_estados) /* Funcao para inicializar as informacoes do AFND. */
{
    for (int i = 0; i < total_estados; i++) /* Preenche o vetor contendo as variaveis. */
    {
        afnd[i].estado = informacoes.variaveis[i];
        afnd[i].ptTransicoes = inicializa_lista(); /* Inicializa a lista que ira conter as transicoes, para cada estado. */
        afnd[i].finais[0] = '\0';
    }
}

TRANSICOES_AFND* tira_repeticao_lista(TRANSICOES_AFND *ptLista) /* Funcao para remover producoes repetidas de cada estado do AFND. */
{
    if (ptLista == NULL)
        return NULL;

    TRANSICOES_AFND *ptLista2, *ptaux;

    ptLista2 = inicializa_lista(); /* Inicializa a nova lista, que nao tera repeticoes. */
    ptLista2 = insereFim_AFND(ptLista2, ptLista->transicao);

    for (ptaux = ptLista->prox; ptaux != NULL; ptaux = ptaux->prox) /* Iteracao para preencher a nova lista, que nao tera repeticoes. */
        if (!contem_lista(ptLista2, ptaux->transicao))
            ptLista2 = insereFim_AFND(ptLista2, ptaux->transicao); /* Atualiza a lista de producoes, evitando repeticoes. */

    destroi_listaAFND(ptLista); /* Destroi a lista antiga. */
    return ptLista2; /* Retorna o ponteiro para a nova lista. */
}

void controla_producoesrepetidas(AUTOMATO_AFND afnd[],INFOGLUD informacoes) /* Controle dos casos em que o usuario inseriu alguma regra de producao repetida. */
{
    TRANSICOES_AFND *ptaux;
    int total_estados = strlen(informacoes.variaveis); /* Variavel para conter o numero de estados do AFND obtido a partir da GLUD. */
    char aux[DIMENSAOLINHA]; /* String destinada a conter todos os caracteres das producoes. */

    memset(aux, 0, sizeof(aux)); /* Inicializa o array de caracteres auxiliar. */

    for (int i = 0; i < total_estados; i++) /* Iteracao para tirar repeticoes de cada lista de cada estado de transicao do AFND. */
       afnd[i].ptTransicoes = tira_repeticao_lista(afnd[i].ptTransicoes);
}

void determina_alcancaveis(AUTOMATO_AFND afnd[],INFOGLUD informacoes, char nome, char alcancaveis[]) /* Recebe um estado do AFND, e retorna todos os estados atingiveis por ele. */
{
    int total_variaveis = strlen(informacoes.variaveis);
    int indice_afnd = retorna_indice_nomeAFND(afnd,nome,total_variaveis);
    int i = 0;

    char aux[2];
    memset(aux, 0, sizeof(aux)); /* Inicializa todo o array de caracteres auxiliar. */

    alcancaveis[0] = nome; /* Inicializa o array de caracteres que ira conter todos os estados atingiveis. */
    alcancaveis[1] = '\0';

    TRANSICOES_AFND *ptaux;

    while (alcancaveis[i] != '\0') /* Iteracao para inserir todos os estados atingiveis no array de caracteres. */
    {
        for (ptaux = afnd[indice_afnd].ptTransicoes; ptaux != NULL; ptaux = ptaux->prox) /* Iteracao para obter todos os estados atingiveis a partir de um estado. */
        {
            aux[0] = ptaux->transicao[1];
            aux[1] = '\0';

            if (!strstr(alcancaveis,aux)) /* Atualiza o vetor de caracteres com o nome dos estados atingiveis, evitando repeticoes. */
                strcat(alcancaveis,aux);
        }

        i++;

        if (alcancaveis[i] != '\0')
        {
            nome = alcancaveis[i]; /* Atualiza o nome do estado sendo analisado, para embutir mais transicoes (novos caminhos). */
            indice_afnd = retorna_indice_nomeAFND(afnd,nome,total_variaveis); /* Obtem o indice no AFND do proximo estado a ser analisado */
        }
    }
}

int verifica_atingefinal(AUTOMATO_AFND afnd[],INFOGLUD informacoes,char nome) /* Verifica se o estado do AFND recebido como parametro atinge um estado final. */
{
    int total_variaveis = strlen(informacoes.variaveis);
    int indice_afnd = 0;
    TRANSICOES_AFND *ptaux;

    char alcancaveis[total_variaveis+1];
    determina_alcancaveis(afnd,informacoes,nome,alcancaveis); /* Obtem os estados atingiveis a partir do estado recebido como parametro. */

    for (int i = 0; alcancaveis[i] != '\0'; i++)  /* Iteracao para verificar se o estado recebido como parametro atinge um estado final. */
    {
        indice_afnd = retorna_indice_nomeAFND(afnd,alcancaveis[i],total_variaveis); /* Obtem o indice no AFND do estado recebido como parametro. */

        if (strlen(afnd[indice_afnd].finais) != 0) /* Verifica se o estado analisado chega em um estado final. */
            return 1; /* Retorna um, indicando que o estado atinge um estado final. */
    }

    return 0; /* Retorna zero, indicando que o estado NAO atinge um estado final. */
}

int verifica_loop_transitivo (AUTOMATO_AFND afnd[],INFOGLUD informacoes,char nome) /* Verifica se existe um loop entre o estado recebido e outro estado qualquer, no AFND. */
{
    int total_variaveis = strlen(informacoes.variaveis);
    int indice_afnd = 0;
    TRANSICOES_AFND *ptaux;

    char alcancaveis[total_variaveis+1];
    determina_alcancaveis(afnd,informacoes,nome,alcancaveis); /* Obtem os estados atingiveis a partir do estado recebido como parametro. */

    for (int i = 1; alcancaveis[i] != '\0'; i++)
    {
        indice_afnd = retorna_indice_nomeAFND(afnd,alcancaveis[i],total_variaveis);

        /* Verifica se algum dos estados atingiveis pelo estado do parametro possui alguma transicao de volta para o estado recebido como parametro. */
        for (ptaux = afnd[indice_afnd].ptTransicoes; ptaux != NULL; ptaux = ptaux->prox)
            if (ptaux->transicao[1] == nome)
                return 1;
    }

    return 0;
}

int verifica_loop(AUTOMATO_AFND afnd[],INFOGLUD informacoes,char nome) /* Verifica se o estado do AFND recebido possui um loop. */
{
    int total_variaveis = strlen(informacoes.variaveis);
    int indice_afnd = retorna_indice_nomeAFND(afnd,nome,total_variaveis);
    TRANSICOES_AFND *ptaux;

    for (ptaux = afnd[indice_afnd].ptTransicoes; ptaux != NULL; ptaux = ptaux->prox)
        if (ptaux->transicao[1] == nome)
            return 1; /* Retorna um, indicando que o estado possui um loop. */

    return 0; /* Retorna zero, indicando que o estado NAO possui um loop. */
}

int propriedade_aux(AUTOMATO_AFND afnd[],INFOGLUD informacoes) /* Analisa o automato AFND e indica se a linguagem e vazia, finita ou infinita. */
{
    if (afnd == NULL) /* Controle dos casos em que o automato e vazio (linguagem finita). */
        return CODIGOLINGUAGEM_VAZIA;

    char inicial = informacoes.inicial;
    int total_variaveis = strlen(informacoes.variaveis);
    int indice_inicial = -1;

    indice_inicial = retorna_indice_nomeAFND(afnd,informacoes.inicial,total_variaveis);

    /* Controle dos casos em que nao existe NENHUMA transicao a partir do estado inicial (linguagem finita). */
    if (afnd[indice_inicial].ptTransicoes == NULL && strlen(afnd[indice_inicial].finais) == 0)
        return CODIGOLINGUAGEM_VAZIA;

    char alcancaveis_inicial[total_variaveis+1]; /* String para conter os estados atingiveis pelo inicial. */
    memset(alcancaveis_inicial, 0, sizeof(alcancaveis_inicial));
    char uteis[total_variaveis+1];
    memset(uteis, 0, sizeof(uteis));

    char nao_atingefinal[total_variaveis+1]; /* String para conter os nomes dos estados que nao atingem um estado final. */
    memset(nao_atingefinal, 0, sizeof(nao_atingefinal));

    char nome_aux[2]; /* String auxiliar para conter o nome de um estado em analise. */
    nome_aux[1] = '\0';

    for (int i = 0; i < total_variaveis; i++) /* Iteracao para obter estados que nao atingem um estado final. */
    {
        nome_aux[0] = afnd[i].estado;

        if (!verifica_atingefinal(afnd,informacoes,afnd[i].estado))
            strcat(nao_atingefinal,nome_aux);
    }

    nome_aux[0] = informacoes.inicial;

    if(strstr(nao_atingefinal,nome_aux)) /* Verifica se o estado inicial nao atinge nenhum estado final (linguagem vazia). */
        return CODIGOLINGUAGEM_VAZIA; /* Indica que a linguagem gerada pela GLUD e vazia. */

    determina_alcancaveis(afnd,informacoes,informacoes.inicial,alcancaveis_inicial); /* Determina os estados que sao atingiveis pelo estado inicial. */

    for (int i = 0; alcancaveis_inicial[i] != '\0'; i++) /* Iteracao para obter os estados atingiveis pelo inicial e que ATINGEM UM ESTADO FINAL. */
    {
        nome_aux[0] = alcancaveis_inicial[i];

        if (!strstr(nao_atingefinal,nome_aux))
            strcat(uteis,nome_aux);
    }

    for (int i = 0; uteis[i] != '\0'; i++) /* Iteracao para determinar se algum dos estados obtidos na ultima iteracao possui algum loop. */
    {
        if (verifica_loop(afnd,informacoes,uteis[i])) /* Verifica se o estado possui um loop (linguagem infinita). */
            return CODIGOLINGUAGEM_INFINITA; /* Indica que a linguagem gerada pela GLUD e infinita. */

        if(verifica_loop_transitivo(afnd,informacoes,uteis[i])) /* Verifica se existe um loop de transicoes entre o estado analisado e outro estado qualquer (linguagem infinita). */
            return CODIGOLINGUAGEM_INFINITA; /* Indica que a linguagem gerada pela GLUD e infinita. */
    }

    return CODIGOLINGUAGEM_FINITA; /* Indica que a linguagem gerada pela GLUD e finita. */
}

void propriedade(AUTOMATO_AFND afnd[], INFOGLUD informacoes) /* Funcao para indicar se o automato AFND obtido a partir da GLUD corresponde a uma linguagem vazia, infinita ou finita. */
{
    int codigo = propriedade_aux(afnd,informacoes); /* Obtem o codigo para determinar se a linguagem e vazia, infinita ou finita. */

    printf("\n------------------------------------------------\n");

    if (codigo == CODIGOLINGUAGEM_VAZIA) /* Verifica o codigo retornado pela funcao, para determinar se a linguagem e vazia, infinita ou finita. */
        printf("\nA GLUD gera uma linguagem regular VAZIA.\n");

    else if (codigo == CODIGOLINGUAGEM_INFINITA)
        printf("\nA GLUD gera uma linguagem regular INFINITA.\n");

    else if (codigo == CODIGOLINGUAGEM_FINITA)
        printf("\nA GLUD gera uma linguagem regular FINITA.\n");
}

void leitura_arquivo(FILE *arq_GLUD) /* Realiza a leitura do arquivo de entrada contendo a GLUD. */
{
    char linha[DIMENSAOLINHA]; /* String para conter o conteudo de cada linha do arquivo de entrada. */

    INFOGLUD informacoes; /* Estrutura para conter certas informacoes da GLUD. */
    AUTOMATO_AFND *afnd; /* Vetor para conter cada um dos estados de transicao do AFND correspondente a GLUD. */

    int conta_linha = 1; /* Contador para indicar qual linha esta sendo lida. */
    int total_estados = 0; /* Variavel para conter o numero de estados do AFND obtido a partir da GLUD. */
    int flag_vazia = 0; /* Flag que indica se uma linha do arquivo de entrada esta vazia. */

    while (!feof(arq_GLUD)) /* Iteracao para percorrer todas as linhas da GLUD. */
    {
        fgets(linha,DIMENSAOLINHA,arq_GLUD); /* Obtem a linha do arquivo de entrada. */
        flag_vazia = linha_vazia(linha);

        if (flag_vazia == 1) /* Controle dos casos em que o usuario insere uma linha vazia. */
            erro_entrada();

        if (flag_vazia == 0 && conta_linha == 1) /* Realiza o controle da primeira linha. */
        {
            controla_primeiralinha(linha, &informacoes); /* Verifica se ha erros na primeira linha, e obtem os seus dados. */
            conta_linha++;
            controla_automato(informacoes); /* Verifica se as informacoes sao todas validas. */
        }

        else if (conta_linha == 2) /* Realiza o controle da segunda linha. */
        {
            controla_segundalinha(linha,informacoes); /* Verifica se ha erros na segunda linha, e obtem os seus dados. */
            conta_linha++;

            total_estados = strlen(informacoes.variaveis);
            afnd = (AUTOMATO_AFND*)calloc(total_estados,sizeof(AUTOMATO_AFND)); /* Aloca espaco de memoria para o AFND. */
            preenche_estados(afnd,informacoes, total_estados);
        }

        else if (conta_linha > 2) /* Realiza o controle das demais linhas do arquivo de entrada, contendo as producoes. */
            controla_producoes(linha,afnd,informacoes); /* Verifica se ha erros nas regras de producao, e obtem os seus dados. */

        memset(linha, 0, sizeof(linha)); /* Reinicia todo o array de caracteres da linha antes da proxima iteracao. */
    }

    controla_producoesrepetidas(afnd,informacoes); /* Controle dos casos em que o usuario inseriu alguma regra de producao repetida. */

    int total_variaveis = strlen(informacoes.variaveis);
    int total_estadosAFD = pow(2,total_variaveis)-1;
    int total_terminais = strlen(informacoes.terminais);

    AUTOMATO_AFD *afd = calloc(total_estadosAFD,sizeof(AUTOMATO_AFD)); /* Declaracao do vetor destinado a conter os estado do AFD obtido a partir do AFND. */
    inicializa_afd(afd,afnd,informacoes.variaveis); /* Inicializa os nomes das variaveis do novo AFD. */

    preenche_afd(afd,afnd,informacoes); /* Preenche cada estado do novo AFD com as informacoes necessarias. */
    atualiza_afd(afd,informacoes); /* Atualiza as regras de producao do novo AFD. */
    atualiza_finais_afd(afd,afnd,informacoes); /* Determina os caracteres que levam a um estado final, para cada estado do novo AFD gerado. */

    recebe_palavras(afd,informacoes); /* Realiza a leitura das palavras do arquivo de palavras, e determina quais delas sao aceitas pelo novo AFD. */
    propriedade(afnd,informacoes); /* Determina se a GLUD do arquivo de entrada gera uma linguagem finita, infinita ou vazia. */

    for (int i = 0; i < total_variaveis; i++) /* Libera o espaco de memoria alocado para as listas de transicoes de cada estado do AFND. */
        destroi_listaAFND(afnd[i].ptTransicoes);

    for (int i = 0; i < total_estadosAFD; i++) /* Libera o espaco de memoria alocado para as listas de transicoes de cada estado do AFD. */
        destroi_listaAFD(afd[i].ptTransicoes);

    free(afnd); /* Libera o espaco de memoria alocado pelo AFND inicial. */
    free(afd); /* Libera o espaco de memoria alocado pelo AFD gerado a partir do AFND inicial. */

    printf("\n\nPressione ESC para encerrar a execucao do programa.\n");
    printf("Pressione qualquer outra tecla para realizar uma nova leitura.");
}

void abre_arquivo() /* Abre e inicia a leitura do arquivo de entrada, contendo a GLUD. */
{
    FILE *arq_GLUD;
    char nome_arquivo[DIMENSAONOME];

    printf("\nDigite o nome do arquivo de entrada contendo a GLUD:\n"); /* Obtem o nome do arquivo de entrada do usuario. */
    gets(nome_arquivo);

    if (!(arq_GLUD = fopen(nome_arquivo,"r"))) /* Abertura do arquivo de entrada contendo a GLUD, com controle dos casos em que nao e possivel abri-lo. */
    {
        system("cls");
        printf("Erro na abertura do arquivo texto contendo a GLUD. Fim do programa.\n");
        getch();
        exit(1); /* Encerra a execucao do programa, retornando um. */
    }

    leitura_arquivo(arq_GLUD); /* Realiza a leitura de todas as linhas do arquivo texto de entrada contendo a GLUD. */
    fclose(arq_GLUD); /* Fecha o arquivo de entrada contendo a GLUD.  */
}

void inicia_programa() /* Inicia a leitura dos arquivos de entrada. */
{
    char tecla;

    do
    {
        system("cls");
        printf("LINGUAGENS FORMAIS E AUTOMATOS\n"); /* Impressao do menu inicial. */
        printf("\nTRABALHO FINAL PARTE 2\n");
        printf("\nAndrei Pochmann Koenich\n");
        printf("\n------------------------------------------------\n");

        abre_arquivo(); /* Abre e inicia a leitura do arquivo de entrada. */

        tecla = getch(); /* Obtem a tecla digitada pelo usuario. */

    } while (tecla != ESC);
}

int main ()
{
    inicia_programa(); /* Inicia a leitura dos arquivos de entrada. */
    printf("\n");
    return 0; /* Retorna zero, indicando que nao houve erro na execucao do programa. */
}
