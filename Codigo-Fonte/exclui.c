#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agencia.h"
#define M 2

void concatenacao(ArvB* arv, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno, int posIrmao)
{
            printf("\n\n\nCONCATENAÇÃO PROLIFERADA  \n");

            int i = 0, posTroca = arv->pai->pos;
            while(arv->pai->filho[i]->chave[0]!= arv->chave[0])
                i++;

            if(arv->numChaves - 1+ arv->pai->filho[posIrmao]->numChaves < 2*M)// condição para fazer a concatenação
            {

                    arv->chave[arv->numChaves] = arv->pai->chave[i]; //recebe a chave do pai
                    arv->numChaves++;
                    arv->pai->numChaves--;
                    arv->filho[arv->numChaves] = arv->pai->filho[posIrmao]->filho[0];// recebe o primeiro filho do irmão adjacente

                        for(int j = 0; j < arv->pai->filho[posIrmao]->numChaves; j++)
                        {
                                arv->chave[arv->numChaves] = arv->pai->filho[posIrmao]->chave[j];
                                arv->filho[arv->numChaves +1] = arv->pai->filho[posIrmao]->filho[j+1];// o primeiro filho é inserido depois
                                arv->numChaves++;
                        }


                        if(arv->pai->numChaves == 0)
                        {
                                if(arv->pai->pai == NULL) // Caso o nó pai seja a raiz, deve- se atualizar o novo nó raiz no arquivo
                                    atualizaPai(arqMeta, arv->pos);


                                 ArvB* vazia = criaVazia();
                                 vazia->pos = arv->pai->filho[posIrmao]->pos;
                                 escreveNoInterno(vazia, arqNoInterno, vazia->pos);// escreve vazio no arquivo., na posição do irmão adjacente

                                  arv->pai->filho[posIrmao] = NULL;
                                  arv->pai = NULL;

                                  vazia->pos = posTroca; // escreve vazio no aruivo , na posição do pai
                                 escreveNoInterno(vazia, arqNoInterno, vazia->pos);
                                 escreveNoInterno(arv, arqNoInterno, arv->pos); // atualiza no aqrquivo, o próprio nó da árvore
                        }
                        else
                                escreveNoInterno(arv, arqNoInterno, arv->pos);

            }

}


void redistribuicao(ArvB* arv, int pos, int posIrmao, int posPai, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)
{

            printf("REDISTRIBUIÇÂO  FEITA\n");
            int i = 0,  posArv = arv->pos,  posArvAnt = arv->pai->filho[posIrmao]->pos;

            while(arv->pai->filho[i]->clientes[0]->cod != arv->clientes[0]->cod)
                i++;

            arv->filho[pos+1] = NULL;
            arv->chave[pos] = 0;
            arv->clientes[pos] = NULL;

            if(posIrmao == i+1)
            {
                        printf("IRMÃO ADJACENTE À DIREITA\n");
                       arv->pai->chave[posPai] = arv->pai->filho[posIrmao]->chave[1];
                      // printf("\n\n%d\n",  arv->pai->filho[posIrmao]->chave[1]);
                       arv->chave[pos] = arv->pai->filho[posIrmao]->chave[0];
                       arv->clientes[pos] = arv->pai->filho[posIrmao]->clientes[0];

                       arv->pai->filho[posIrmao]->chave[0] = 0;
                       arv->pai->filho[posIrmao]->clientes[0] = NULL;

                      if(arv->filho[pos+1]!= NULL)
                            arv->filho[pos+1] = arv->pai->filho[posIrmao]->filho[0];


                       for(int i = 0; i <  arv->pai->filho[posIrmao]->numChaves; i++)
                        {
                                arv->pai->filho[posIrmao]->chave[i] = arv->pai->filho[posIrmao]->chave[i+1];
                                arv->pai->filho[posIrmao]->clientes[i] = arv->pai->filho[posIrmao]->clientes[i+1];
                                arv->pai->filho[posIrmao]->filho[i] = arv->pai->filho[posIrmao]->filho[i+1];

                                arv->pai->filho[posIrmao]->chave[i+1] = 0;
                                arv->pai->filho[posIrmao]->clientes[i+1] = NULL;
                                arv->pai->filho[posIrmao]->filho[i+1] = NULL;
                        }

                            arv->pai->filho[posIrmao]->numChaves--;

                       if(pos != arv->numChaves-1)
                        {
                                int k = arv->numChaves-1;

                                 for(int i = 0; i < arv->numChaves; i++)
                                    {
                                            for(int j = 0; j < k; j++)
                                            {
                                                    if(arv->chave[j] > arv->chave[j + 1])
                                                    {
                                                        Cliente* aux  = arv->clientes[j];
                                                        int auxChave = arv->chave[j];
                                                        arv->clientes[j] = arv->clientes[j + 1];
                                                        arv->chave[j] = arv->chave[j+1];
                                                        arv->clientes[j+ 1] = aux;
                                                        arv->chave[j+1] = auxChave;
                                                    }
                                            }
                                            k--;
                                    }

                        }

                        // for(int i = 0; i < arv->pai->filho[posIrmao]->numChaves; i++)
                        // {
                        //         printf("IRMAOOO: %d\n", arv->pai->filho[posIrmao]->clientes[i]->cod);
                        // }
            }

            else if(posIrmao == i-1)
            {
                         printf("IRMÃO ADJACENTE À ESQUERDA\n");
                          int numChavesIrmao = arv->pai->filho[posIrmao]->numChaves;

                         arv->pai->chave[posPai] = arv->pai->filho[posIrmao]->chave[numChavesIrmao-1]; // Transfiro a ultima chave do irmão para o nó pai

                        //avançamos as chaves, os filhos e clientes

                        if(pos != 0)
                        {
                            for(int i = 2*M-1; i >0; i--)
                            {
                                    arv->chave[i] = arv->chave[i-1];
                                    arv->clientes[i] = arv->clientes[i-1];
                                    arv->filho[i] = arv->filho[i-1];
                            }
                        }

                        // printf("\n\n\n chave de 111= ==== %d\n", arv->chave[1]);

                        arv->chave[0] = arv->pai->filho[posIrmao]->chave[numChavesIrmao-1];
                        arv->clientes[0] = arv->pai->filho[posIrmao]->clientes[numChavesIrmao-1];
                        arv->filho[0] = arv->pai->filho[posIrmao]->filho[numChavesIrmao];

                        arv->pai->filho[posIrmao]->clientes[numChavesIrmao-1] = NULL;
                        arv->pai->filho[posIrmao]->chave[numChavesIrmao-1] = 0;
                        arv->pai->filho[posIrmao]->filho[numChavesIrmao] = NULL;
                        arv->pai->filho[posIrmao]->numChaves--;

            }

            arv->pos = posArv;
            arv->pai->filho[posIrmao]->pos = posArvAnt;

            escreveCliente(arv, arqDados, arv->pos);
            escreveCliente(arv->pai->filho[posIrmao], arqDados, arv->pai->filho[posIrmao]->pos);
            escreveNoInterno(arv->pai, arqNoInterno, arv->pai->pos);

}


void excluiRegistro(ArvB* arv, int pos, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)// pos = posição da chave
{
            int i = 0;
            int posIrmao, posExclusao, posPai;

            if(arv->pai != NULL)
            {
                while(arv->pai->filho[i]->chave[pos] != arv->clientes[pos]->cod)
                          i++;


                        if(arv->pai->filho[i-1] != NULL && i-1 >=0)
                         {
                               posIrmao = i-1;
                               posExclusao = i+1;
                               posPai = i-1;
                        }
                      else if(arv->pai->filho[i+1] != NULL)
                        {
                                posIrmao = i+1;
                                posExclusao = i-1;
                                posPai = i;
                        }

            }

            if(verificaFolha(arv) == 1 && arv->numChaves-1 >= M ||  (verificaFolha(arv) == 1 && arv->pai == NULL) )// Exclui registro de nó folha (numero de clientes - 1 >= M)
            {
                    printf("EXCLUSÃO SIMPLES\n");
                    arv->clientes[pos] = NULL;
                    arv->chave[pos] = 0;


                    for(int i = pos; i < arv->numChaves; i++)
                    {
                            arv->clientes[i] = arv->clientes[i + 1];
                            arv->chave[i] = arv->chave[i+1];
                            arv->clientes[i +  1] = NULL;
                            arv->chave[i+1] = 0;
                    }
                    arv->numChaves--;

                    //printf("arv->pos :: %d\n", arv->pos);

                        atualizaNo(arv, arqMeta, arqDados, arqNoInterno);
             }

            else if(arv->numChaves - 1 < M) // concatena
            {

                    //printf("ENTREI NA SEGUNDA CONDIÇÂO\n");
                        int posTroca = arv->pos;
                        int posTrocaPai = arv->pai->pos;

                    if(arv->numChaves -  1+ arv->pai->filho[posIrmao]->numChaves  < 2*M)
                    {
                            printf("\n\n\nCONCATENENAÇÃO FEITA !\n");
                             int posAnt = arv->pai->filho[posIrmao]->numChaves;


                             arv->pai->chave[posPai] = 0; // "apaga " a chave do nó pai, já que ela não carrega dados
                             arv->pai->numChaves--;

                              for(int k = pos; k < arv->numChaves; k++) // reordena as clientes aós a exclusão do elemento
                            {
                                   arv->clientes[k] = arv->clientes[k + 1];
                                   arv->chave[k] = arv->chave[k+1];
                                    arv->clientes[k +  1] = NULL;
                                    arv->chave[k+1] = 0;
                                    // arv->filho[k] = arv->filho[k+1];
                                    // arv->filho[k+1] = NULL;
                                      //printf("ANTES NUMCHAVES   %d\n", arv->numChaves);
                            }

                            arv->numChaves--;

                            for(int j = 0; j < arv->numChaves; j++)// recebe as clientes e os filhos do nó que será eliminado
                            {
                                    arv->pai->filho[posIrmao]->clientes[posAnt] = arv->clientes[j];
                                    arv->pai->filho[posIrmao]->chave[posAnt] = arv->chave[j];
                                    arv->pai->filho[posIrmao]->filho[posAnt + 1] = arv->filho[j + 1];///////

                                    if(arv->pai->filho[posIrmao]->filho[posAnt+ 1] != NULL)
                                        arv->pai->filho[posIrmao]->filho[posAnt+1]->pai = arv->pai->filho[posIrmao];
                                    arv->pai->filho[posIrmao]->numChaves++;
                                    arv->clientes[j] = NULL;
                                    arv->chave[j] = 0;
                                    arv->filho[j+1] = NULL;
                                    arv->numChaves--;
                                    posAnt++;
                            }


                            int k = arv->pai->filho[posIrmao]->numChaves -1;
                             for(int i = 0; i < arv->pai->filho[posIrmao]->numChaves; i++) // ordena os clientes após a concatenação
                            {
                                for(int j = 0; j < k; j++)
                                {
                                        if(arv->pai->filho[posIrmao]->clientes[j + 1]->cod < arv->pai->filho[posIrmao]->clientes[j]->cod)
                                        {
                                            Cliente* aux = arv->pai->filho[posIrmao]->clientes[j];
                                            int auxChave = arv->pai->filho[posIrmao]->chave[j];
                                            arv->pai->filho[posIrmao]->clientes[j] = arv->pai->filho[posIrmao]->clientes[j + 1];
                                            arv->pai->filho[posIrmao]->clientes[j + 1]  = aux;
                                            arv->pai->filho[posIrmao]->chave[j] = arv->pai->filho[posIrmao]->chave[j + 1];
                                            arv->pai->filho[posIrmao]->chave[j + 1]  = auxChave;
                                        }
                                }

                                k--;
                            }

                             for(int j = i; j < arv->pai->numChaves + 1; j++) // ordena as chaves e os filhos da arvore pai após a remoção de um nó
                            {
                                    ArvB* aux = arv->pai->filho[j+1];
                                    arv->pai->filho[j] = aux;

                                    if(i != 0)
                                     {
                                             arv->pai->chave[j-1] = arv->pai->chave[j];
                                             arv->pai->chave[j] = 0;
                                    }

                                    else
                                     {
                                            if(arv->pai->numChaves == 1 && arv->pai->chave[j+1] == 0){
                                                arv->pai->chave[j] = arv->pai->filho[0]->chave[arv->pai->filho[0]->numChaves - 1];
                                                //printf("\n\n\n\n\n DOIDÃO !!!!!!!!!!!!!!!!\n");
                                            }
                                            else{
                                                    arv->pai->chave[j] = arv->pai->chave[j+1];
                                                arv->pai->chave[j+1] = 0;
                                          }
                                    }
                                    arv->pai->filho[j+1] = NULL;
                            }


                            if(posIrmao == i-1)
                            {
                                        printf("IRMÃO ADJACENTE À ESQUERDA\n");
                                          if(arv->pai->filho[i-1] != NULL)
                                                arv->pai->filho[posIrmao]->proxPag = arv->pai->filho[i+1];
                            }
                            else if(posIrmao == i+1)
                            {
                                          printf("IRMÃO ADJACENTE À DIREITA\n");
                                            if(arv->pai->filho[i-1] != NULL )
                                                arv->pai->filho[i-1]->proxPag = arv->pai->filho[i+1];
                            }
                              // printf("pos do irmão :: %d\n", posIrmao);

                           ArvB* vazia = criaVazia(); // árvore criada apenas para atualizar o nó excluído no arquivo.

                         //   arv->pai->filho[i] = NULL;

                                    // if(arv->pai->numChaves == 0)
                                    // {
                                    //      atualizaPai(arqMeta, arv->pai->filho[posIrmao]->pos);
                                    //      arv->pai->filho[posIrmao]->pai = NULL;
                                    //      vazia->pos = posTrocaPai;
                                    //      atualizaNo(vazia, arqMeta, arqDados);
                                    // }
                                    // else
                                    //         atualizaNo(arv->pai, arqMeta, arqDados);


                            vazia->pos = posTroca;
                            escreveCliente(vazia, arqDados, vazia->pos);

                            if(posIrmao != 0)
                                atualizaNo(arv->pai->filho[posIrmao-1], arqMeta, arqDados, arqNoInterno);
                            else
                                  atualizaNo(arv->pai->filho[posIrmao], arqMeta, arqDados, arqNoInterno);


                            if(arv->pai->numChaves < M && arv->pai->pai != NULL)// verifico se o pai é raiz
                            {
                                    if(arv->pai->pai->filho[i+1] != NULL)
                                              concatenacao(arv->pai, arqMeta, arqDados, arqNoInterno, i+1);

                                    else if(arv->pai->pai->filho[i-1] != NULL)
                                              concatenacao(arv->pai, arqMeta, arqDados, arqNoInterno, i-1);

                                   // arv->pai->pai = NULL;
                                   // arv->pai = NULL;
                                    free(arv->pai->pai);
                                    arv->pai = arv;
                                    //printf("\n\n\n\n\n ARV CONCAT : %d", arv->chave[0]);
                            }

                         //     arv = NULL;
                    }
                          else
                          {
                                redistribuicao(arv, pos, posIrmao, posPai, arqMeta, arqDados, arqNoInterno);
                          }

            }

            else
            {

                    int i;

                    arv->clientes[pos] = arv->filho[pos + 1]->clientes[0];

                    arv->filho[pos + 1]->clientes[0] = NULL;
                    for( i = 0; i < arv->filho[pos+1]->numChaves; i++)
                    {
                        arv->filho[pos + 1]->clientes[i] = arv->filho[pos + 1]->clientes[i + 1];
                        arv->filho[pos + 1]->filho[i] = arv->filho[pos+1]->filho[i+1];
                        arv->filho[pos +1]->clientes[i +  1] = NULL;
                    }

                    arv->filho[pos+1]->numChaves--;
                    arv->filho[pos+1]->filho[i] = NULL;

                    atualizaNo(arv, arqMeta, arqDados, arqNoInterno);
                    atualizaNo(arv->filho[pos+1], arqMeta, arqDados, arqNoInterno);

            }
}


void exclui(ArvB* arv, int num, FILE* arqMeta, FILE* arqDados , FILE* arqNoInterno)
{

        if(arv == NULL)
            printf("REGISTRO NÃO ENCONTRADO\n");

        else
        {

                int i = arv->numChaves;

                while(i > 0 && num < arv->chave[i-1])
                    i--;


                if(verificaFolha(arv) == 1 && num == arv->clientes[i -1]->cod)
                {
                        excluiRegistro(arv, i-1, arqMeta, arqDados, arqNoInterno);
                }

                else
                   {
                         exclui(arv->filho[i], num, arqMeta, arqDados, arqNoInterno);
                    }
        }

}