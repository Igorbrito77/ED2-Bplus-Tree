#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agencia.h"
#define M 2


int buscaDvd(ArvDvd* arv, int num)
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

                if(verificaFolhaDvd(arv) == 1 && num == arv->chave[i-1])
                {
                        printf("DVD  ENCONTRADO .\n");
                        printf("Título: %s\n", arv->dvds[i-1]->titulo);
                        printf("Código: %d\n", arv->dvds[i-1]->cod);
                        printf("Ano: %d\n", arv->dvds[i-1]->ano);
                        return 1;
                }

                else
                    buscaDvd(arv->filho[i], num);
    }

}


void buscaNomeDvd(ArvDvd* arv, char titulo[100])
{

        if(arv == NULL)
            printf("REGISTRO NÃO ENCONTRADO\n");

        else
        {
                if(verificaFolhaDvd(arv) == 0)
                    buscaNomeDvd(arv->filho[0], titulo);

                else
                {
                        int find;
                        ArvDvd* aux;
                        for(aux = arv; aux != NULL && find == 0; aux = aux->proxPag)
                        {
                           // printf("\n\nprimeira = %d\n", aux->chave[0]);
                                for(int i = 0; i < aux->numChaves && find == 0 ; i++)
                                {
                                       if( strcmp(titulo, aux->dvds[i]->titulo) == 0)
                                        {
                                                printf("____________________________TÍTULO ENCONTRADO__________________________________________\n");
                                                printf("Título: %s\nCódigo: %d\nAno de lançamento: %d\n",  aux->dvds[i]->titulo, aux->dvds[i]->cod, aux->dvds[i]->ano);
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

void buscaMaiorAno(ArvDvd* arv, int num)
{
      printf("..\n");

        if(arv == NULL)
            printf("REGISTRO NÃO ENCONTRADO\n");

        else
        {
                if(verificaFolhaDvd(arv) == 0)
                    buscaMaiorAno(arv->filho[0], num);

                else
                {
                        int find;
                        ArvDvd* aux;
                        for(aux = arv; aux != NULL && find == 0; aux = aux->proxPag)
                        {
                           // printf("\n\nprimeira = %d\n", aux->chave[0]);
                                for(int i = 0; i < aux->numChaves && find == 0 ; i++)
                                {
                                       if(aux->dvds[i]->ano >num)
                                        {
                                                printf("____________________________TÍTULO ENCONTRADO______ANO: %d____________________________________\n", aux->dvds[i]->ano);
                                                printf("Título: %s\nCódigo: %d\nAno de lançamento: %d\n",  aux->dvds[i]->titulo, aux->dvds[i]->cod, aux->dvds[i]->ano);
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