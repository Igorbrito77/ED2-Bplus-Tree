#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agencia.h"
#define M 2


int busca(ArvB* arv, int num)
{

        if(arv == NULL)
         {
                printf("REGISTRO NÃO ENCONTRADO\n");
                return 0;
        }
        else
        {
                int i = arv->numChaves;

                while(i > 0 && num < arv->chave[i-1])
                    i--;

                if(verificaFolha(arv) == 1 && num == arv->chave[i-1])
                {
                        printf("\n\n\nREGISTRO  ENCONTRADO .\n");
                        printf("Nome: %s\n", arv->clientes[i-1]->nome);
                        printf("Código: %d\n\n\n", arv->clientes[i-1]->cod);
                        return 1;
                }

                else
                    busca(arv->filho[i], num);
    }

}


void buscaNome(ArvB* arv, char nome[100])
{
    printf("..\n");

        if(arv == NULL)
            printf("REGISTRO NÃO ENCONTRADO\n");

        else
        {
                if(verificaFolha(arv) == 0)
                    buscaNome(arv->filho[0], nome);

                else
                {
                        int find;
                        ArvB* aux;
                        for(aux = arv; aux != NULL && find == 0; aux = aux->proxPag)
                        {
                           // printf("\n\nprimeira = %d\n", aux->chave[0]);
                                for(int i = 0; i < aux->numChaves && find == 0 ; i++)
                                {
                                       if( strcmp(nome, aux->clientes[i]->nome) == 0)
                                        {
                                                printf("\n\n\n____________________________REGISTRO ENCONTRADO__________________________________________\n");
                                                printf("Nome: %s\nCódigo: %d\nTelefone: %s\n",  aux->clientes[i]->nome, aux->clientes[i]->cod, aux->clientes[i]->tel);
                                                printf("____________________________________________________________________________________________________________\n");
                                                find = 1;
                                        }
                                }
                        }

                        if(find == 0)
                                   printf("REGISTRO NÃO ENCONTRADO\n");
                }

            }
}