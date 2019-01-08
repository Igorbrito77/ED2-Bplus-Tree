RELATÓRIO DA  AA ESTRUTURAS DE DADOS 2

Manual de Uso:

A implementação do sistema da  locadora, foi dividido em vários arquivos, e para melhor execução deste,  foi feito um Makefile para compilar todos de uma vez.

O usuário deve digitar  “make” no terminal do Linux, e em seguida “./main”, para executar o programa.

Iniciando o programa, o usuário cai no menu principal, e dependendo da escolha, é dirigido para um menu secundário.

O programa irá continuar rodando até o usuário pressionar a tecla 0 no menu principal.


Detalhes da Implementação:

O código gira em torno do loop do menu prinicipal, contido na função main.

Grande parte das funções usadas são recursivas, devido ao fato de trabalharmos com a Árvore B+, incluindo as de exclusão, inserção e busca.

Foram criadas 3 árvores ( locações, clientes e dvds ).
Utilizamos 3 arquivos “.dat” para cada árvore .
Sendo que, um é para metadados, outro para os nós internos da árvore ,e o último para armazenar os nós folha ,e consequentemente , os dados armazenados. Assim, totalizando 9 arquivos.

As estruturas e o cabeçalho da maioria das funções utilizadas no código se encontram no arquivo “agencia.h”.

Caso o usuário deseje começar o programa do zero, existem as funções do tipo “criaVazia(...)”, que inicializam uma árvore contendo nenhum dado, ou se já houverem dados gravados nos arquivos, ele pode utilizar as funções do tipo “montaArvore(…)“, que criam a árvore a partir dos dados presentes nos arquivos.

Caso o usuário queira ver os dados  na forma de uma árvore b+ , digitando 777 em qualquer menu secundário, serão impressos os nós folha, os nós internos ,e o arquivo de metadados, proporcionando uma visão de como a árvore se encontra naquele momento.






