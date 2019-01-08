#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agencia.h"
#define M 2


ArvDvd* criaDvdVazia()
{
        ArvDvd* arv = (ArvDvd*) malloc(sizeof(ArvDvd));
        arv->numChaves = 0;
        arv->pai = NULL;
        arv->pos = 0;

        for(int i = 0; i  < 2*M + 1; i++)
                 arv->filho[i] = NULL;
        for(int i = 0; i < 2*M; i++)
        {
                arv->chave[i] = 0;
                arv->dvds[i] = NULL;
        }

        return arv;
}


int verificaFolhaDvd(ArvDvd* arv)
{
        for(int i = 0; i < 2*M+1; i++)
        {
                if(arv->filho[i] != NULL)
                    return 0;
        }
        return 1;
}


void inserePaginaNaoCheiaDvd(ArvDvd* arv, Dvd* dvd, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)
{

            int i = arv->numChaves, pai;
            int num = dvd->cod;

                    while(i > 0 && num < arv->dvds[i- 1]->cod)
                    {
                        printf("..\n");
                            printf("%d\n", arv->dvds[i-1]->cod);
                            arv->dvds[i] = arv->dvds[i- 1];
                            arv->chave[i] = arv->chave[i-1];
                            i--;
                    }


                    arv->dvds[i] = dvd;
                    arv->chave[i] = dvd->cod;
                    arv->numChaves++;


                int pos;
                fseek(arqMeta, sizeof(int)*2, SEEK_SET);
                fread(&pos, sizeof(int), 1, arqMeta);


                if(arv->pai != NULL)
                {
                        i= 0;
                        while(arv->pai->filho[i]->dvds[0]->cod != arv->dvds[0]->cod)
                            i++;

                        arv->proxPag =  arv->pai->filho[i+1];

                }

                if(pos == 0)// raiz ainda é nula
                {

                     arv->pos  = 0;

                     escreveNovoDvd(arv, arqMeta, arqDados, arqNoInterno);
                     atualizaPai(arqMeta, arv->pos);
                   //  escreveNoInternoDvd(arv, arqNoInterno, arv->pos);

                }
            else
                 {
                        atualizaNoDvd(arv, arqMeta, arqDados, arqNoInterno);
                        //escreveNoInternoDvd(arv, arqNoInterno ,arv->pos);
                }
}


ArvDvd* insereFilhoDvd(ArvDvd* pai, ArvDvd* filho, Dvd* dvd, FILE* arqMeta, FILE*arqDados, FILE* arqNoInterno)
{
        int i = pai->numChaves;
        int num = filho->dvds[0]->cod;

         while(i > 0 && num < pai->chave[i - 1])
        {
                pai->filho[i+1] = pai->filho[i];
                pai->chave[i] = pai->chave[i - 1];
                  i--;
        }

        pai->chave[i] = dvd->cod;
        pai->filho[i+ 1]  = filho;
        pai->filho[i+1]->pai = pai;
        pai->numChaves++;
        pai->filho[i]->proxPag = pai->filho[i+1];
        pai->filho[i+1]->proxPag = pai->filho[i+2];
        pai->filho[i + 1]->pos = proxPosicaoFolha(arqMeta);
        atualizaNoDvd(pai->filho[i + 1], arqMeta, arqDados, arqNoInterno);
        //atualizaNoDvd(pai, arqMeta, arqDados);
        return pai;
}

void posicionaFilhoDvd(ArvDvd* pai, ArvDvd* filho, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)
{

        int num = filho->dvds[0]->cod;
         int i = pai->numChaves;
                    while(i > 0 && num < pai->chave[i - 1])
                            i--;

        if ( (pai->filho[i] != NULL)  && ( pai->filho[i]->chave[0]!= num  ||  (pai->filho[i]->chave[0] == num && verificaFolhaDvd(pai->filho[i]) != 1)   ) )
                posicionaFilhoDvd(pai->filho[i], filho, arqMeta, arqDados, arqNoInterno);


        else
             {

                 pai->filho[i] = filho;
                 if(pai->filho[i-1] != NULL)
                     pai->filho[i-1]->proxPag = pai->filho[i];

                 pai->filho[i]->proxPag = pai->filho[i+1];
                 pai->filho[i]->pai = pai;

                //  for(int j =0; j < pai->filho[i]->numChaves; j++)
                // {
                //         printf("\n\n\nFILHOS ADICIONADOS ->");
                //        if(pai->filho[i]->dvds[j] != NULL)
                //         printf("%d\n", pai->filho[i]->dvds[j]->cod);
                // }

                 atualizaNoDvd(pai->filho[i], arqMeta, arqDados, arqNoInterno);
             }

}

ArvDvd* particionaNoInternoDvd(ArvDvd* arv, int num, FILE*arqMeta, FILE*arqDados, FILE* arqNoInterno)
{

                    ArvDvd*  filho1 = criaDvdVazia();
                    ArvDvd* filho2 = criaDvdVazia();

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
                        arv->dvds[0] = NULL;

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

                      for(int i = 1; i < 2*M; i++)// zera as dvds restantes da arvore (dvds a partir do M)
                      {
                                 arv->chave[i] = 0;
                                arv->dvds[i] = NULL;
                                arv->filho[i + 1] = NULL;
                     }

                        arv->filho[0]->pos = proxPosicaoInterna(arqMeta);
                        arv->filho[1]->pos = proxPosicaoInterna(arqMeta);


                        atualizaPai(arqMeta, pos);
                        escreveNoInternoDvd(arv, arqNoInterno, pos);
                        escreveNoInternoDvd(arv->filho[0], arqNoInterno, arv->filho[0]->pos);
                        escreveNoInternoDvd(arv->filho[1], arqNoInterno, arv->filho[1]->pos);

        }

            // else if(arv->pai->numChaves == 2*M)// Caso a página pai também esteja cheia
            // {

            //         filho2->pos = proxPosicaoFolha(arqMeta);

            //         // if(verificaFolhaDvd(filho1) == 1)
            //         // {
            //         //         filho1->proxPag = filho2;
            //         // }

            //         particionaNoInternoDvd(arv->pai, num, arqMeta, arqDados, arqNoInterno);
            //         posicionaFilhoDvd(arv->pai, filho1, arqMeta, arqDados, arqNoInterno);
            //         posicionaFilhoDvd(arv->pai, filho2, arqMeta, arqDados, arqNoInterno);
            //         escreveNoInternoDvd(arv->pai,arqNoInterno, arv->pai->pos);


            //     //    atualizaNoDvd(arv->pai, arqMeta, arqDados);

            // }

        return arv;
}


void particionaDvd(ArvDvd* arv,  Dvd* dvd, FILE*arqMeta, FILE*arqDados, FILE* arqNoInterno)
{
        int pos = arv->pos;

        ArvDvd*  filho1 = criaDvdVazia();
        ArvDvd* filho2 = criaDvdVazia();

                Dvd** v = (Dvd**) malloc(sizeof(Dvd*));
                for(int i = 0; i < 1+2*M; i++)
                    v[i] = (Dvd*) malloc(sizeof(Dvd));

                int i = 2*M ;
                //v = arv->dvds;
                for(int j = 0; j < 2*M; j++)
                    v[j] = arv->dvds[j];

                v[i] = dvd;

                while(i > 0 && v[i]->cod < v[i-1]->cod)
                {
                                   Dvd* aux= v[i];
                                    v[i] = v[i- 1];
                                    v[i - 1] = aux;
                                    i--;
                }

                for(int i = 0; i < M; i++)
                {
                        filho1->dvds[i] = v[i];
                        filho1->chave[i] = v[i]->cod;
                        filho2->dvds[i+1] = v[i + M+ 1];
                        filho2->chave[i+1] = v[i+M+1]->cod;
                        // filho2->filho[i+2] = arv->filho[i + M + 1];
                        // filho1->filho[i] = arv->filho[i];
                }

                filho1->pos = pos;
                filho1->filho[M] = arv->filho[M];
                filho2->dvds[0] = v[M];
                filho2->chave[0] = v[M]->cod;
                filho1->numChaves = M;
                filho2->numChaves = M+1;


        if(arv->pai == NULL)// caso a página seja a raiz
        {

                arv->numChaves = 1;
                arv->chave[0] = v[M]->cod;
                arv->dvds[0] = NULL;

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

              for(int i = 1; i < 2*M; i++)// zera as dvds restantes da arvore (dvds a partir do M)
              {
                         arv->chave[i] = 0;
                        arv->dvds[i] = NULL;
                        arv->filho[i + 1] = NULL;
             }

             //   arv->pos = proxPosicaoInterna(arqMeta);
                arv->filho[1]->pos = proxPosicaoFolha(arqMeta);

                  atualizaPai(arqMeta, pos);
                  atualizaNoDvd(arv->filho[0], arqMeta, arqDados, arqNoInterno);
                atualizaNoDvd(arv->filho[1], arqMeta, arqDados, arqNoInterno);
               //  escreveNoInternoDvd(arv, arqNoInterno, arv->pos);
        }

        else
        {


            if(arv->pai->numChaves == 2*M)// Caso a página pai também esteja cheia
            {

                    filho2->pos = proxPosicaoFolha(arqMeta);

                   arv->pai =  particionaNoInternoDvd(arv->pai, v[M]->cod, arqMeta, arqDados, arqNoInterno);

                    posicionaFilhoDvd(arv->pai, filho1, arqMeta, arqDados, arqNoInterno);
                    posicionaFilhoDvd(arv->pai, filho2, arqMeta, arqDados, arqNoInterno);

                    arv->pai->filho[0]->filho[arv->pai->filho[0]->numChaves]->proxPag = arv->pai->filho[1]->filho[0];
                   // printf("\n\n\n\n\n PROXPAAAAAAAAAAAAAAAAG = %d\n",arv->pai->filho[0]->filho[arv->pai->filho[0]->numChaves]->proxPag->chave[0]);

                   //      escreveNoInternoDvd(arv->pai->pai->filho[1],arqNoInterno, arv->pai->pai->filho[1]->pos);

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

                 arv->pai =  insereFilhoDvd(arv->pai, filho2, v[M] , arqMeta, arqDados, arqNoInterno);

                atualizaNoDvd(arv->pai->filho[i], arqMeta, arqDados, arqNoInterno);
            }

        }

}


void insereDvd(ArvDvd* arv, Dvd* dvd, FILE* arqMeta, FILE* arqDados, FILE*arqNoInterno)
{
              int i = arv->numChaves;

                while(i > 0 &&  dvd->cod < arv->chave[i-1])
                        i--;

                if(arv->chave[i-1] == dvd->cod)
                        printf("O CÓDIGO %d JÁ ESTÁ ESTÁ REGISTRADO! \n", dvd->cod);

                else
                {
                            if(arv->numChaves == 2*M && verificaFolhaDvd(arv) == 1)
                            {
                                    particionaDvd(arv, dvd, arqMeta, arqDados, arqNoInterno);
                            }
                            else
                            {

                                if(verificaFolhaDvd(arv) == 0)
                                {
                                        int i = arv->numChaves;
                                        while(i > 0 && dvd->cod < arv->chave[i - 1])
                                                i--;

                                        insereDvd(arv->filho[i], dvd, arqMeta, arqDados, arqNoInterno);
                                }
                                else
                                    inserePaginaNaoCheiaDvd(arv, dvd, arqMeta, arqDados, arqNoInterno);

                            }
            }
}

