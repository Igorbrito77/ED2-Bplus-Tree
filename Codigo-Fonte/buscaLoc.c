#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agencia.h"
#define M 2


int v[100];
int v2[100];

void buscaLoc(ArvLoc* arv, int num)
{

        if(arv == NULL)
            printf("REGISTRO NÃO ENCONTRADO\n");

        else
        {
                int i = arv->numChaves;

                while(i > 0 && num < arv->chave[i-1])
                    i--;

                if(verificaFolhaLoc(arv) == 1 && num == arv->chave[i-1])
                {
                        printf("\n\n\n ________________LOCAÇÃO  ENCONTRADA ._________________\n\n");
                        printf("Número da locação: %d\n", arv->locs[i-1]->numLoc);
                        printf("Código do Cliente: %d\n", arv->locs[i-1]->codCliente);
                        printf("Código do Dvd: %d\n", arv->locs[i-1]->codDvd);
                        printf("Data de empréstimo: %s\n", arv->locs[i-1]->dataEmp);
                        printf("Data de devolução: %s\n", arv->locs[i-1]->dataDev);
                        if(arv->locs[i-1]->status == 0)
                            printf("Não devolvido\n");
                        else
                            printf("Devolução efetuada\n");
                }

                else
                    buscaLoc(arv->filho[i], num);
    }

}


int buscaDevolucao(ArvLoc* arv, int num)
{
         if(arv == NULL)
          {
                    printf("REGISTRO NÃO ENCONTRADO\n");
                    return -1;
        }
        else
        {
                int i = arv->numChaves;

                while(i > 0 && num < arv->chave[i-1])
                    i--;

                if(verificaFolhaLoc(arv) == 1 && num == arv->chave[i-1])
                {
                        printf("\n\nLOCAÇÃO  ENCONTRADA .\n");
                        printf("Número da locação: %d\n", arv->locs[i-1]->numLoc);
                        printf("Código do Cliente: %d\n", arv->locs[i-1]->codCliente);
                        printf("Código do Dvd: %d\n", arv->locs[i-1]->codDvd);
                        printf("Data de empréstimo: %s\n", arv->locs[i-1]->dataEmp);

                        printf("\n\nDigite a data da devolução: \n");
                        scanf("%s", arv->locs[i-1]->dataDev);

                        arv->locs[i-1]->status = 1;
                        return arv->pos;
                }

                else
                    buscaLoc(arv->filho[i], num);
        }
}


void buscaPendente(ArvLoc* arv, ArvB* arvCl)
{

        if(arv == NULL)
            printf("REGISTRO NÃO ENCONTRADO\n");

        else
        {
                if(verificaFolhaLoc(arv) == 0)
                    buscaPendente(arv->filho[0], arvCl);

                else
                {
                        int find;
                        ArvLoc* aux;
                        for(aux = arv; aux != NULL /*&& find == 0*/; aux = aux->proxPag)
                        {
                           // printf("\n\nprimeira = %d\n", aux->chave[0]);
                                for(int i = 0; i < aux->numChaves/* && find == 0*/ ; i++)
                                {
                                       if(aux->locs[i]->status == 0)
                                        {
                                                busca(arvCl, aux->locs[i]->codCliente);
                                                printf("____________________________________________________________________________________________________________\n");
                                                find = 1;
                                        }
                                }
                        }

                        if(find == 0)
                                   printf("TÍTULO NÃO ENCONTRADO\n");
                }

            }
}

void buscaPendenteX(ArvLoc* arv, ArvB* arvCl, int x)
{

        if(arv == NULL)
            printf("REGISTRO NÃO ENCONTRADO\n");

        else
        {
                if(verificaFolhaLoc(arv) == 0)
                    buscaPendenteX(arv->filho[0], arvCl, x);

                else
                {
                        ArvLoc* aux;
                        for(aux = arv; aux != NULL /*&& find == 0*/; aux = aux->proxPag)
                        {

                                for(int i = 0; i < aux->numChaves/* && find == 0*/ ; i++)
                                {
                                                 v[aux->locs[i]->codCliente]++;

                                                if(v[aux->locs[i]->codCliente] > x && v2[aux->locs[i]->codCliente] == 0)
                                                {
                                                                busca(arvCl, aux->locs[i]->codCliente);
                                                                printf("____________________________________________________________________________________________________________\n");
                                                                v2[aux->locs[i]->codCliente] = 1;
                                                }

                                }

                        }
                }

            }
}

void buscaPendenciaX(ArvLoc* arv, ArvB* arvCl, int x)
{
        for(int i = 0; i < 100; i++)
        {
                v[i] = 0;
                v2[i] = 0;
        }

        buscaPendenteX(arv, arvCl, x);
}