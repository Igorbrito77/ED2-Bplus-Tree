#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agencia.h"
#define M 2


ArvB* criaVazia()
{
        ArvB* arv = (ArvB*) malloc(sizeof(ArvB));
        arv->numChaves = 0;
        arv->pai = NULL;
        arv->pos = 0;

        for(int i = 0; i  < 2*M + 1; i++)
                 arv->filho[i] = NULL;
        for(int i = 0; i < 2*M; i++)
        {
                arv->chave[i] = 0;
                arv->clientes[i] = NULL;
        }

        return arv;
}


int verificaFolha(ArvB* arv)
{
        for(int i = 0; i < 2*M+1; i++)
        {
                if(arv->filho[i] != NULL)
                    return 0;
        }
        return 1;
}


void inserePaginaNaoCheia(ArvB* arv, Cliente* cliente, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)
{

            int i = arv->numChaves, pai;
            int num = cliente->cod;

                    while(i > 0 && num < arv->clientes[i- 1]->cod)
                    {
                            arv->clientes[i] = arv->clientes[i- 1];
                            arv->chave[i] = arv->chave[i-1];
                            i--;
                    }


                    arv->clientes[i] = cliente;
                    arv->chave[i] = cliente->cod;
                    arv->numChaves++;


                int pos;
                fseek(arqMeta, sizeof(int)*2, SEEK_SET);
                fread(&pos, sizeof(int), 1, arqMeta);


                if(arv->pai != NULL)
                {
                        i= 0;
                        while(arv->pai->filho[i]->clientes[0]->cod != arv->clientes[0]->cod)
                            i++;

                        arv->proxPag =  arv->pai->filho[i+1];

                }

                if(pos == 0)// raiz ainda é nula
                {

                     arv->pos  = 0;

                     escreveNovo(arv, arqMeta, arqDados, arqNoInterno);
                     atualizaPai(arqMeta, arv->pos);
                   //  escreveNoInterno(arv, arqNoInterno, arv->pos);

                }
            else
                 {
                        atualizaNo(arv, arqMeta, arqDados, arqNoInterno);
                        //escreveNoInterno(arv, arqNoInterno ,arv->pos);
                }
}


ArvB* insereFilho(ArvB* pai, ArvB* filho, Cliente* cliente, FILE* arqMeta, FILE*arqDados, FILE* arqNoInterno)
{
        int i = pai->numChaves;
        int num = filho->clientes[0]->cod;

         while(i > 0 && num < pai->chave[i - 1])
        {
                pai->filho[i+1] = pai->filho[i];
                pai->chave[i] = pai->chave[i - 1];
                  i--;
        }

        pai->chave[i] = cliente->cod;
        pai->filho[i+ 1]  = filho;
        pai->filho[i+1]->pai = pai;
        pai->numChaves++;
        pai->filho[i]->proxPag = pai->filho[i+1];
        pai->filho[i+1]->proxPag = pai->filho[i+2];
        pai->filho[i + 1]->pos = proxPosicaoFolha(arqMeta);
        atualizaNo(pai->filho[i + 1], arqMeta, arqDados, arqNoInterno);
        //atualizaNo(pai, arqMeta, arqDados);
        return pai;
}

void posicionaFilho(ArvB* pai, ArvB* filho, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)
{

        int num = filho->clientes[0]->cod;
         int i = pai->numChaves;
                    while(i > 0 && num < pai->chave[i - 1])
                            i--;

        if ( (pai->filho[i] != NULL)  && ( pai->filho[i]->chave[0]!= num  ||  (pai->filho[i]->chave[0] == num && verificaFolha(pai->filho[i]) != 1)   ) )
                posicionaFilho(pai->filho[i], filho, arqMeta, arqDados, arqNoInterno);


        else
             {
               // printf("hacker\n");
                 pai->filho[i] = filho;
                 if(pai->filho[i-1] != NULL)
                     pai->filho[i-1]->proxPag = pai->filho[i];

                 pai->filho[i]->proxPag = pai->filho[i+1];
                 pai->filho[i]->pai = pai;

                //  for(int j =0; j < pai->filho[i]->numChaves; j++)
                // {
                //         printf("\n\n\nFILHOS ADICIONADOS ->");
                //        if(pai->filho[i]->clientes[j] != NULL)
                //         printf("%d\n", pai->filho[i]->clientes[j]->cod);
                // }

                 atualizaNo(pai->filho[i], arqMeta, arqDados, arqNoInterno);
             }

}

ArvB* particionaNoInterno(ArvB* arv, int num, FILE*arqMeta, FILE*arqDados, FILE* arqNoInterno)
{

                    ArvB*  filho1 = criaVazia();
                    ArvB* filho2 = criaVazia();

                int *v = (int*) malloc(sizeof(int) * (2*M+1)), pos = arv->pos;
                int i = 2*M;

                v = arv->chave;
                v[i] = num;


                for(int i = 0; i < M; i++)
                {

                        filho1->chave[i] = v[i];
                        filho2->chave[i] = v[i + M + 1];
                        filho2->filho[i] = arv->filho[i + M + 1];
                        filho1->filho[i] = arv->filho[i];

                        if(filho1->filho[i-1] != NULL)
                            filho1->filho[i-1]->proxPag = filho1->filho[i];
                        if(filho2->filho[i-1] != NULL)
                            filho2->filho[i-1]->proxPag = filho2->filho[i];
                }


                filho1->filho[M] = arv->filho[M];
                filho1->filho[M-1]->proxPag = filho1->filho[M];
                filho1->filho[M]->proxPag = filho2->filho[0];
                filho1->numChaves = M;
                filho2->numChaves = M;

             if(arv->pai == NULL)// caso a página seja a raiz
             {
                        arv->numChaves = 1;
                        arv->chave[0] = v[M];
                        arv->clientes[0] = NULL;

                        arv->pai = NULL;
                        arv->pos = pos;
                        filho2->pai = arv;
                        filho1->pai = arv;


                        for(int i = 0; i < filho1->numChaves+1; i++)
                            if(filho1->filho[i]!= NULL)
                                filho1->filho[i]->pai = filho1;

                          for(int i = 0; i < filho2->numChaves+1; i++)
                                  if(filho2->filho[i]!= NULL)
                                        filho2->filho[i]->pai = filho2;


                        arv->filho[0] = filho1;
                        arv->filho[1]= filho2;

                        arv->filho[0]->filho[arv->filho[0]->numChaves]->proxPag = arv->filho[1]->filho[0];

                      for(int i = 1; i < 2*M; i++)// zera as clientes restantes da arvore (clientes a partir do M)
                      {
                                 arv->chave[i] = 0;
                                arv->clientes[i] = NULL;
                                arv->filho[i + 1] = NULL;
                     }

                        arv->filho[0]->pos = proxPosicaoInterna(arqMeta);
                        arv->filho[1]->pos = proxPosicaoInterna(arqMeta);
                        //printf("\n\n\nna função a pos é ::::::%d\n",   arv->filho[1]->pos);

                        atualizaPai(arqMeta, pos);
                        escreveNoInterno(arv, arqNoInterno, pos);
                        escreveNoInterno(arv->filho[0], arqNoInterno, arv->filho[0]->pos);
                        escreveNoInterno(arv->filho[1], arqNoInterno, arv->filho[1]->pos);

        }

            // else if(arv->pai->numChaves == 2*M)// Caso a página pai também esteja cheia
            // {

            //         filho2->pos = proxPosicaoFolha(arqMeta);

            //         // if(verificaFolha(filho1) == 1)
            //         // {
            //         //         filho1->proxPag = filho2;
            //         // }

            //         particionaNoInterno(arv->pai, num, arqMeta, arqDados, arqNoInterno);
            //         posicionaFilho(arv->pai, filho1, arqMeta, arqDados, arqNoInterno);
            //         posicionaFilho(arv->pai, filho2, arqMeta, arqDados, arqNoInterno);
            //         escreveNoInterno(arv->pai,arqNoInterno, arv->pai->pos);


            //     //    atualizaNo(arv->pai, arqMeta, arqDados);

            // }

        return arv;
}


void particiona(ArvB* arv,  Cliente* cliente, FILE*arqMeta, FILE*arqDados, FILE* arqNoInterno)
{
        int pos = arv->pos;

        ArvB*  filho1 = criaVazia();
        ArvB* filho2 = criaVazia();

                Cliente** v = (Cliente**) malloc(sizeof(Cliente*));
                for(int i = 0; i < 1+2*M; i++)
                    v[i] = (Cliente*) malloc(sizeof(Cliente));

                int i = 2*M ;
                //v = arv->clientes;
                for(int j = 0; j < 2*M; j++)
                    v[j] = arv->clientes[j];

                v[i] = cliente;

                while(i > 0 && v[i]->cod < v[i-1]->cod)
                {
                                   Cliente* aux= v[i];
                                    v[i] = v[i- 1];
                                    v[i - 1] = aux;
                                    i--;
                }

                for(int i = 0; i < M; i++)
                {
                        filho1->clientes[i] = v[i];
                        filho1->chave[i] = v[i]->cod;
                        filho2->clientes[i+1] = v[i + M+ 1];
                        filho2->chave[i+1] = v[i+M+1]->cod;
                        // filho2->filho[i+2] = arv->filho[i + M + 1];
                        // filho1->filho[i] = arv->filho[i];
                }

                filho1->pos = pos;
                filho1->filho[M] = arv->filho[M];
                filho2->clientes[0] = v[M];
                filho2->chave[0] = v[M]->cod;
                filho1->numChaves = M;
                filho2->numChaves = M+1;


        if(arv->pai == NULL)// caso a página seja a raiz
        {

                arv->numChaves = 1;
                arv->chave[0] = v[M]->cod;
                arv->clientes[0] = NULL;

                arv->pai = NULL;
                filho2->pai = arv;
                filho1->pai = arv;


                for(int i = 0; i < filho1->numChaves+1; i++)
                    if(filho1->filho[i]!= NULL)
                        filho1->filho[i]->pai = filho1;

                  for(int i = 0; i < filho2->numChaves+1; i++)
                          if(filho2->filho[i]!= NULL)
                                filho2->filho[i]->pai = filho2;


                arv->filho[0] = filho1;
                arv->filho[1]= filho2;
                arv->filho[0]->proxPag = arv->filho[1];
                arv->pos = pos;

              for(int i = 1; i < 2*M; i++)// zera as clientes restantes da arvore (clientes a partir do M)
              {
                         arv->chave[i] = 0;
                        arv->clientes[i] = NULL;
                        arv->filho[i + 1] = NULL;
             }

             //   arv->pos = proxPosicaoInterna(arqMeta);
                arv->filho[1]->pos = proxPosicaoFolha(arqMeta);

                  atualizaPai(arqMeta, pos);
                  atualizaNo(arv->filho[0], arqMeta, arqDados, arqNoInterno);
                atualizaNo(arv->filho[1], arqMeta, arqDados, arqNoInterno);
               //  escreveNoInterno(arv, arqNoInterno, arv->pos);
        }

        else
        {


            if(arv->pai->numChaves == 2*M)// Caso a página pai também esteja cheia
            {

                    filho2->pos = proxPosicaoFolha(arqMeta);

                   arv->pai =  particionaNoInterno(arv->pai, v[M]->cod, arqMeta, arqDados, arqNoInterno);

                    posicionaFilho(arv->pai, filho1, arqMeta, arqDados, arqNoInterno);
                    posicionaFilho(arv->pai, filho2, arqMeta, arqDados, arqNoInterno);

                    arv->pai->filho[0]->filho[arv->pai->filho[0]->numChaves]->proxPag = arv->pai->filho[1]->filho[0];
                   // printf("\n\n\n\n\n PROXPAAAAAAAAAAAAAAAAG = %d\n",arv->pai->filho[0]->filho[arv->pai->filho[0]->numChaves]->proxPag->chave[0]);

                   //      escreveNoInterno(arv->pai->pai->filho[1],arqNoInterno, arv->pai->pai->filho[1]->pos);

            }

            else
            {
                filho2->pai = arv->pai;
                filho1->pai = arv->pai;

                int i = arv->pai->numChaves;

                  while(i > 0 && filho1->chave[0] < arv->pai->chave[i - 1])
                        i--;


                arv->pai->filho[i]  = filho1;
                if(arv->pai->filho[i-1] != NULL)
                    arv->pai->filho[i-1]->proxPag = arv->pai->filho[i];

                 arv->pai =  insereFilho(arv->pai, filho2, v[M] , arqMeta, arqDados, arqNoInterno);

                atualizaNo(arv->pai->filho[i], arqMeta, arqDados, arqNoInterno);
            }

        }

}


void insere(ArvB* arv, Cliente* cliente, FILE* arqMeta, FILE* arqDados, FILE*arqNoInterno)
{
           int i = arv->numChaves;

            while(i > 0 &&  cliente->cod < arv->chave[i-1])
                    i--;

            if(arv->chave[i-1] == cliente->cod)
                    printf("O CÓDIGO %d JÁ ESTÁ ESTÁ REGISTRADO! \n", cliente->cod);

            else
            {
                        if(arv->numChaves == 2*M && verificaFolha(arv) == 1)
                        {
                                    particiona(arv, cliente, arqMeta, arqDados, arqNoInterno);
                        }

                        else
                        {

                            if(verificaFolha(arv) == 0)
                            {
                                    int i = arv->numChaves;
                                    while(i > 0 && cliente->cod < arv->chave[i - 1])
                                            i--;
                                        insere(arv->filho[i], cliente, arqMeta, arqDados, arqNoInterno);
                            }

                            else
                                    inserePaginaNaoCheia(arv, cliente, arqMeta, arqDados, arqNoInterno);

                        }

            }
}

