RECONHECEDOR DE PALAVRAS GERADAS POR GRAMÁTICA LINEAR UNITÁRIA À DIREITA (GLUD)

Autor: Andrei Pochmann Koenich

Este projeto foi desenvolvido na disciplina de Linguagens Formais e Autômatos.

O objetivo deste programa é receber um arquivo de entrada contendo uma definição de uma Gramática Linear Unitária à Direita (GLUD), e outro arquivo de entrada
com uma palavra em cada linha do arquivo. O programa irá converter a gramática inserida em um Autômato Finito Determinístico (AFD), e utilizará este autômato
para determinar quais das palavras existentes no segundo arquivo de entrada podem ser geradas pela gramática inserida no primeiro arquivo de entrada. As palavras
que podem ser geradas pela gramática inserida serão exibidas no terminal.

Abaixo, segue a descrição de cada um dos módulos existentes na pasta "Sources":

inputFilesReader.ts -> faz a leitura dos dois arquivos de entrada fornecidos pelo usuário, capturando e armazenando os dados fornecidos;

fileDataController.ts -> analisa a sintaxe e a semântica do arquivo de entrada contendo a definição da GLUD. Caso existam erros, esse módulo encerra a execução do programa e informa o usuário sobre o erro ocorrido;

automataBuilder.ts -> constrói o Autômato Finito Não Determinístico (AFND) correspondente à GLUD definida pelo usuário, no primeiro arquivo de entrada;

automataConverter.ts -> converte o Autômato Finito Não Determinístico (AFND) em um Autômato Finito Determinístico (AFD), para que seja possível reconhecer as palavras inseridas pelo usuário no segundo arquivo de entrada;

wordsTester.ts -> testa todas as palavras existentes no segundo arquivo de entrada, com o Autômato Finito Determinístico (AFD). As palavras que podem ser reconhecidas pelo autômato são impressas no terminal.

Os módulos "classes.ts", "constants.ts" e "utils.ts" possuem definições auxiliares, utilizadas pelos módulos principais.

O diretório "Test Cases" possui arquivos com definições de gramáticas que podem ser usados para testes. Abaixo, segue a descrição de cada arquivo:

gludTest1.txt -> contém uma GLUD capaz de gerar somente palavras constituídas pelas letras "a" e "b", com o tamanho da palavra (quantidade de caracteres) sendo sempre um valor múltiplo de 3;

gludTest2.txt -> contém uma GLUD capaz de gerar somente palavras constituídas pelas letras "a", "b" e "c". Essa gramática gera apenas palavras que possuem no mínimo três ocorrências dos caracteres "a", "b", ou "c";

gludTest3.txt -> contém uma GLUD capaz de gerar somente palavras constituídas pelas letras "a" e "b", gerando apenas palavras que não possuam a subpalavra "aa" nos quatro últimos caracteres.

words.txt -> contém uma lista de palavras (uma palavra em cada linha). Este arquivo pode ser usado como o segundo arquivo de entrada, a fim de verificar se a gramática do primeiro arquivo de entrada pode gerar as palavras nele existentes.