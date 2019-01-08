#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agencia.h"
#define M 2

void concatenacaoLoc(ArvLoc* arv, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno, int posIrmao)
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


                                 ArvLoc* vazia = criaLocVazia();
                                 vazia->pos = arv->pai->filho[posIrmao]->pos;
                                 escreveNoInternoLoc(vazia, arqNoInterno, vazia->pos);// escreve vazio no arquivo., na posição do irmão adjacente

                                  arv->pai->filho[posIrmao] = NULL;
                                  arv->pai = NULL;

                                  vazia->pos = posTroca; // escreve vazio no aruivo , na posição do pai
                                 escreveNoInternoLoc(vazia, arqNoInterno, vazia->pos);
                                 escreveNoInternoLoc(arv, arqNoInterno, arv->pos); // atualiza no aqrquivo, o próprio nó da árvore
                        }
                        else
                                escreveNoInternoLoc(arv, arqNoInterno, arv->pos);

            }

}


void redistribuicaoLoc(ArvLoc* arv, int pos, int posIrmao, int posPai, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)
{

            printf("REDISTRIBUIÇÂO  FEITA\n");
            int i = 0,  posArv = arv->pos,  posArvAnt = arv->pai->filho[posIrmao]->pos;

            while(arv->pai->filho[i]->locs[0]->codCliente != arv->locs[0]->codCliente)
                i++;

            arv->filho[pos+1] = NULL;
            arv->chave[pos] = 0;
            arv->locs[pos] = NULL;

            if(posIrmao == i+1)
            {
                        printf("IRMÃO ADJACENTE À DIREITA\n");
                       arv->pai->chave[posPai] = arv->pai->filho[posIrmao]->chave[1];
                       arv->chave[pos] = arv->pai->filho[posIrmao]->chave[0];
                       arv->locs[pos] = arv->pai->filho[posIrmao]->locs[0];

                       arv->pai->filho[posIrmao]->chave[0] = 0;
                       arv->pai->filho[posIrmao]->locs[0] = NULL;

                      if(arv->filho[pos+1]!= NULL)
                            arv->filho[pos+1] = arv->pai->filho[posIrmao]->filho[0];


                       for(int i = 0; i <  arv->pai->filho[posIrmao]->numChaves; i++)
                        {
                                arv->pai->filho[posIrmao]->chave[i] = arv->pai->filho[posIrmao]->chave[i+1];
                                arv->pai->filho[posIrmao]->locs[i] = arv->pai->filho[posIrmao]->locs[i+1];
                                arv->pai->filho[posIrmao]->filho[i] = arv->pai->filho[posIrmao]->filho[i+1];

                                arv->pai->filho[posIrmao]->chave[i+1] = 0;
                                arv->pai->filho[posIrmao]->locs[i+1] = NULL;
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
                                                        Locacao* aux  = arv->locs[j];
                                                        int auxChave = arv->chave[j];
                                                        arv->locs[j] = arv->locs[j + 1];
                                                        arv->chave[j] = arv->chave[j+1];
                                                        arv->locs[j+ 1] = aux;
                                                        arv->chave[j+1] = auxChave;
                                                    }
                                            }
                                            k--;
                                    }

                        }
            }

            else if(posIrmao == i-1)
            {
                        printf("IRMÃO ADJACENTE À ESQUERDA\n");
                        int numChavesIrmao = arv->pai->filho[posIrmao]->numChaves;

                         arv->pai->chave[posPai] = arv->pai->filho[posIrmao]->chave[numChavesIrmao-1]; // Transfiro a ultima chave do irmão para o nó pai

                        //avançamos as chaves, os filhos e locs

                        if(pos != 0)
                        {
                            for(int i = 2*M-1; i >0; i--)
                            {
                                    arv->chave[i] = arv->chave[i-1];
                                    arv->locs[i] = arv->locs[i-1];
                                    arv->filho[i] = arv->filho[i-1];
                            }
                        }

                        arv->chave[0] = arv->pai->filho[posIrmao]->chave[numChavesIrmao-1];
                        arv->locs[0] = arv->pai->filho[posIrmao]->locs[numChavesIrmao-1];
                        arv->filho[0] = arv->pai->filho[posIrmao]->filho[numChavesIrmao];

                        arv->pai->filho[posIrmao]->locs[numChavesIrmao-1] = NULL;
                        arv->pai->filho[posIrmao]->chave[numChavesIrmao-1] = 0;
                        arv->pai->filho[posIrmao]->filho[numChavesIrmao] = NULL;
                        arv->pai->filho[posIrmao]->numChaves--;

            }

            arv->pos = posArv;
            arv->pai->filho[posIrmao]->pos = posArvAnt;

            escreveLocacao(arv, arqDados, arv->pos);
            escreveLocacao(arv->pai->filho[posIrmao], arqDados, arv->pai->filho[posIrmao]->pos);
            escreveNoInternoLoc(arv->pai, arqNoInterno, arv->pai->pos);

}


void excluiRegistroLoc(ArvLoc* arv, int pos, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)// pos = posição da chave
{
            int i = 0;
            int posIrmao, posExclusao, posPai;

            if(arv->pai != NULL)
            {
                while(arv->pai->filho[i]->chave[pos] != arv->locs[pos]->codCliente)
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

            if(verificaFolhaLoc(arv) == 1 && arv->numChaves-1 >= M || verificaFolhaLoc(arv) == 1 && arv->pai == NULL)// ExcluiLoc registro de nó folha (numero de locs - 1 >= M)
            {
                    printf("EXCLUSÃO SIMPLES\n");
                    arv->locs[pos] = NULL;
                    arv->chave[pos] = 0;


                    for(int i = pos; i < arv->numChaves; i++)
                    {
                            arv->locs[i] = arv->locs[i + 1];
                            arv->chave[i] = arv->chave[i+1];
                            arv->locs[i +  1] = NULL;
                            arv->chave[i+1] = 0;
                    }
                    arv->numChaves--;

                        atualizaNoLoc(arv, arqMeta, arqDados, arqNoInterno);
             }

            else if(arv->numChaves - 1 < M) // concatenaLoc
            {

                        int posTroca = arv->pos;
                        int posTrocaPai = arv->pai->pos;

                    if(arv->numChaves -  1+ arv->pai->filho[posIrmao]->numChaves  < 2*M)
                    {
                            printf("\n\n\nCONCATENENAÇÃO FEITA !\n");
                             int posAnt = arv->pai->filho[posIrmao]->numChaves;


                             arv->pai->chave[posPai] = 0; // "apaga " a chave do nó pai, já que ela não carrega dados
                             arv->pai->numChaves--;

                              for(int k = pos; k < arv->numChaves; k++) // reordena as locs aós a exclusão do elemento
                            {
                                   arv->locs[k] = arv->locs[k + 1];
                                   arv->chave[k] = arv->chave[k+1];
                                    arv->locs[k +  1] = NULL;
                                    arv->chave[k+1] = 0;
                            }

                            arv->numChaves--;

                            for(int j = 0; j < arv->numChaves; j++)// recebe as locs e os filhos do nó que será eliminado
                            {
                                    arv->pai->filho[posIrmao]->locs[posAnt] = arv->locs[j];
                                    arv->pai->filho[posIrmao]->chave[posAnt] = arv->chave[j];
                                    arv->pai->filho[posIrmao]->filho[posAnt + 1] = arv->filho[j + 1];///////

                                    if(arv->pai->filho[posIrmao]->filho[posAnt+ 1] != NULL)
                                        arv->pai->filho[posIrmao]->filho[posAnt+1]->pai = arv->pai->filho[posIrmao];
                                    arv->pai->filho[posIrmao]->numChaves++;
                                    arv->locs[j] = NULL;
                                    arv->chave[j] = 0;
                                    arv->filho[j+1] = NULL;
                                    arv->numChaves--;
                                    posAnt++;
                            }


                            int k = arv->pai->filho[posIrmao]->numChaves -1;
                             for(int i = 0; i < arv->pai->filho[posIrmao]->numChaves; i++) // ordena os locs após a concatenação
                            {
                                for(int j = 0; j < k; j++)
                                {
                                        if(arv->pai->filho[posIrmao]->locs[j + 1]->codCliente < arv->pai->filho[posIrmao]->locs[j]->codCliente)
                                        {
                                            Locacao* aux = arv->pai->filho[posIrmao]->locs[j];
                                            int auxChave = arv->pai->filho[posIrmao]->chave[j];
                                            arv->pai->filho[posIrmao]->locs[j] = arv->pai->filho[posIrmao]->locs[j + 1];
                                            arv->pai->filho[posIrmao]->locs[j + 1]  = aux;
                                            arv->pai->filho[posIrmao]->chave[j] = arv->pai->filho[posIrmao]->chave[j + 1];
                                            arv->pai->filho[posIrmao]->chave[j + 1]  = auxChave;
                                        }
                                }

                                k--;
                            }

                             for(int j = i; j < arv->pai->numChaves + 1; j++) // ordena as chaves e os filhos da arvore pai após a remoção de um nó
                            {
                                    ArvLoc* aux = arv->pai->filho[j+1];
                                    arv->pai->filho[j] = aux;

                                    if(i != 0)
                                     {
                                             arv->pai->chave[j-1] = arv->pai->chave[j];
                                             arv->pai->chave[j] = 0;
                                    }

                                    else
                                     {
                                            arv->pai->chave[j] = arv->pai->chave[j+1];
                                              arv->pai->chave[j+1] = 0;
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

                           ArvLoc* vazia = criaLocVazia(); // árvore criada apenas para atualizar o nó excluído no arquivo.


                            vazia->pos = posTroca;
                            escreveLocacao(vazia, arqDados, vazia->pos);

                            if(posIrmao != 0)
                                atualizaNoLoc(arv->pai->filho[posIrmao-1], arqMeta, arqDados, arqNoInterno);
                            else
                                  atualizaNoLoc(arv->pai->filho[posIrmao], arqMeta, arqDados, arqNoInterno);


                            if(arv->pai->numChaves < M && arv->pai->pai != NULL)// verifico se o pai é raiz
                            {
                                    if(arv->pai->pai->filho[i+1] != NULL)
                                              concatenacaoLoc(arv->pai, arqMeta, arqDados, arqNoInterno, i+1);

                                    else if(arv->pai->pai->filho[i-1] != NULL)
                                              concatenacaoLoc(arv->pai, arqMeta, arqDados, arqNoInterno, i-1);


                                    free(arv->pai->pai);
                                    arv->pai = arv;

                            }

                         //     arv = NULL;
                    }
                          else
                          {
                                redistribuicaoLoc(arv, pos, posIrmao, posPai, arqMeta, arqDados, arqNoInterno);
                          }

            }

            else
            {

                    int i;

                    arv->locs[pos] = arv->filho[pos + 1]->locs[0];

                    arv->filho[pos + 1]->locs[0] = NULL;
                    for( i = 0; i < arv->filho[pos+1]->numChaves; i++)
                    {
                        arv->filho[pos + 1]->locs[i] = arv->filho[pos + 1]->locs[i + 1];
                        arv->filho[pos + 1]->filho[i] = arv->filho[pos+1]->filho[i+1];
                        arv->filho[pos +1]->locs[i +  1] = NULL;
                    }

                    arv->filho[pos+1]->numChaves--;
                    arv->filho[pos+1]->filho[i] = NULL;

                    atualizaNoLoc(arv, arqMeta, arqDados, arqNoInterno);
                    atualizaNoLoc(arv->filho[pos+1], arqMeta, arqDados, arqNoInterno);

            }
}


void excluiLoc(ArvLoc* arv, int num, FILE* arqMeta, FILE* arqDados , FILE* arqNoInterno)
{

        if(arv == NULL)
            printf("REGISTRO NÃO ENCONTRADO\n");

        else
        {

                int i = arv->numChaves;

                while(i > 0 && num < arv->chave[i-1])
                    i--;


                if(verificaFolhaLoc(arv) == 1 && num == arv->locs[i -1]->codCliente)
                {
                        excluiRegistroLoc(arv, i-1, arqMeta, arqDados, arqNoInterno);
                }

                else
                   {
                         excluiLoc(arv->filho[i], num, arqMeta, arqDados, arqNoInterno);
                    }
        }

}