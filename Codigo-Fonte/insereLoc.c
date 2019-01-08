#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agencia.h"
#define M 2


ArvLoc* criaLocVazia()
{
        ArvLoc* arv = (ArvLoc*) malloc(sizeof(ArvLoc));
        arv->numChaves = 0;
        arv->pai = NULL;
        arv->pos = 0;

        for(int i = 0; i  < 2*M + 1; i++)
                 arv->filho[i] = NULL;
        for(int i = 0; i < 2*M; i++)
        {
                arv->chave[i] = 0;
                arv->locs[i] = NULL;
        }

        return arv;
}


int verificaFolhaLoc(ArvLoc* arv)
{
        for(int i = 0; i < 2*M+1; i++)
        {
                if(arv->filho[i] != NULL)
                    return 0;
        }
        return 1;
}


void inserePaginaNaoCheiaLoc(ArvLoc* arv, Locacao* locacao, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)
{

            int i = arv->numChaves, pai;
            int num = locacao->numLoc;

                    while(i > 0 && num < arv->locs[i- 1]->numLoc)
                    {
                            arv->locs[i] = arv->locs[i- 1];
                            arv->chave[i] = arv->chave[i-1];
                            i--;
                    }


                    arv->locs[i] = locacao;
                    arv->chave[i] = locacao->numLoc;
                    arv->numChaves++;


                int pos;
                fseek(arqMeta, sizeof(int)*2, SEEK_SET);
                fread(&pos, sizeof(int), 1, arqMeta);


                if(arv->pai != NULL)
                {
                        i= 0;
                        while(arv->pai->filho[i]->locs[0]->numLoc != arv->locs[0]->numLoc)
                            i++;

                        arv->proxPag =  arv->pai->filho[i+1];

                }

                if(pos == 0)// raiz ainda é nula
                {

                     arv->pos  = 0;

                     escreveNovaLocacao(arv, arqMeta, arqDados, arqNoInterno);
                     atualizaPai(arqMeta, arv->pos);
                   //  escreveNoInternoLoc(arv, arqNoInterno, arv->pos);

                }
            else
                 {
                        atualizaNoLoc(arv, arqMeta, arqDados, arqNoInterno);
                        //escreveNoInternoLoc(arv, arqNoInterno ,arv->pos);
                }
}


ArvLoc* insereFilhoLoc(ArvLoc* pai, ArvLoc* filho, Locacao* locacao, FILE* arqMeta, FILE*arqDados, FILE* arqNoInterno)
{
        int i = pai->numChaves;
        int num = filho->locs[0]->numLoc;

         while(i > 0 && num < pai->chave[i - 1])
        {
                pai->filho[i+1] = pai->filho[i];
                pai->chave[i] = pai->chave[i - 1];
                  i--;
        }

        pai->chave[i] = locacao->numLoc;
        pai->filho[i+ 1]  = filho;
        pai->filho[i+1]->pai = pai;
        pai->numChaves++;
        pai->filho[i]->proxPag = pai->filho[i+1];
        pai->filho[i+1]->proxPag = pai->filho[i+2];
        pai->filho[i + 1]->pos = proxPosicaoFolha(arqMeta);
        atualizaNoLoc(pai->filho[i + 1], arqMeta, arqDados, arqNoInterno);
        //atualizaNoLoc(pai, arqMeta, arqDados);
        return pai;
}

void posicionaFilhoLoc(ArvLoc* pai, ArvLoc* filho, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)
{

        int num = filho->locs[0]->numLoc;
         int i = pai->numChaves;
                    while(i > 0 && num < pai->chave[i - 1])
                            i--;

        if ( (pai->filho[i] != NULL)  && ( pai->filho[i]->chave[0]!= num  ||  (pai->filho[i]->chave[0] == num && verificaFolhaLoc(pai->filho[i]) != 1)   ) )
                posicionaFilhoLoc(pai->filho[i], filho, arqMeta, arqDados, arqNoInterno);


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
                //        if(pai->filho[i]->locs[j] != NULL)
                //         printf("%d\n", pai->filho[i]->locs[j]->numLoc);
                // }

                 atualizaNoLoc(pai->filho[i], arqMeta, arqDados, arqNoInterno);
             }

}

ArvLoc* particionaNoInternoLoc(ArvLoc* arv, int num, FILE*arqMeta, FILE*arqDados, FILE* arqNoInterno)
{

                    ArvLoc*  filho1 = criaLocVazia();
                    ArvLoc* filho2 = criaLocVazia();

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
                        arv->locs[0] = NULL;

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

                      for(int i = 1; i < 2*M; i++)// zera as locs restantes da arvore (locs a partir do M)
                      {
                                 arv->chave[i] = 0;
                                arv->locs[i] = NULL;
                                arv->filho[i + 1] = NULL;
                     }

                        arv->filho[0]->pos = proxPosicaoInterna(arqMeta);
                        arv->filho[1]->pos = proxPosicaoInterna(arqMeta);


                        atualizaPai(arqMeta, pos);
                        escreveNoInternoLoc(arv, arqNoInterno, pos);
                        escreveNoInternoLoc(arv->filho[0], arqNoInterno, arv->filho[0]->pos);
                        escreveNoInternoLoc(arv->filho[1], arqNoInterno, arv->filho[1]->pos);

        }

            // else if(arv->pai->numChaves == 2*M)// Caso a página pai também esteja cheia
            // {

            //         filho2->pos = proxPosicaoFolha(arqMeta);

            //         // if(verificaFolhaLoc(filho1) == 1)
            //         // {
            //         //         filho1->proxPag = filho2;
            //         // }

            //         particionaNoInternoLoc(arv->pai, num, arqMeta, arqDados, arqNoInterno);
            //         posicionaFilhoLoc(arv->pai, filho1, arqMeta, arqDados, arqNoInterno);
            //         posicionaFilhoLoc(arv->pai, filho2, arqMeta, arqDados, arqNoInterno);
            //         escreveNoInternoLoc(arv->pai,arqNoInterno, arv->pai->pos);


            //     //    atualizaNoLoc(arv->pai, arqMeta, arqDados);

            // }

        return arv;
}


void particionaLoc(ArvLoc* arv,  Locacao* locacao, FILE*arqMeta, FILE*arqDados, FILE* arqNoInterno)
{
        int pos = arv->pos;

        ArvLoc*  filho1 = criaLocVazia();
        ArvLoc* filho2 = criaLocVazia();

                Locacao** v = (Locacao**) malloc(sizeof(Locacao*));
                for(int i = 0; i < 1+2*M; i++)
                    v[i] = (Locacao*) malloc(sizeof(Locacao));

                int i = 2*M ;
                //v = arv->locs;
                for(int j = 0; j < 2*M; j++)
                    v[j] = arv->locs[j];

                v[i] = locacao;

                while(i > 0 && v[i]->numLoc < v[i-1]->numLoc)
                {
                                   Locacao* aux= v[i];
                                    v[i] = v[i- 1];
                                    v[i - 1] = aux;
                                    i--;
                }

                for(int i = 0; i < M; i++)
                {
                        filho1->locs[i] = v[i];
                        filho1->chave[i] = v[i]->numLoc;
                        filho2->locs[i+1] = v[i + M+ 1];
                        filho2->chave[i+1] = v[i+M+1]->numLoc;
                        // filho2->filho[i+2] = arv->filho[i + M + 1];
                        // filho1->filho[i] = arv->filho[i];
                }

                filho1->pos = pos;
                filho1->filho[M] = arv->filho[M];
                filho2->locs[0] = v[M];
                filho2->chave[0] = v[M]->numLoc;
                filho1->numChaves = M;
                filho2->numChaves = M+1;


        if(arv->pai == NULL)// caso a página seja a raiz
        {

                arv->numChaves = 1;
                arv->chave[0] = v[M]->numLoc;
                arv->locs[0] = NULL;

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

              for(int i = 1; i < 2*M; i++)// zera as locs restantes da arvore (locs a partir do M)
              {
                         arv->chave[i] = 0;
                        arv->locs[i] = NULL;
                        arv->filho[i + 1] = NULL;
             }

             //   arv->pos = proxPosicaoInterna(arqMeta);
                arv->filho[1]->pos = proxPosicaoFolha(arqMeta);

                  atualizaPai(arqMeta, pos);
                  atualizaNoLoc(arv->filho[0], arqMeta, arqDados, arqNoInterno);
                atualizaNoLoc(arv->filho[1], arqMeta, arqDados, arqNoInterno);
               //  escreveNoInternoLoc(arv, arqNoInterno, arv->pos);
        }

        else
        {


            if(arv->pai->numChaves == 2*M)// Caso a página pai também esteja cheia
            {

                    filho2->pos = proxPosicaoFolha(arqMeta);

                   arv->pai =  particionaNoInternoLoc(arv->pai, v[M]->numLoc, arqMeta, arqDados, arqNoInterno);

                    posicionaFilhoLoc(arv->pai, filho1, arqMeta, arqDados, arqNoInterno);
                    posicionaFilhoLoc(arv->pai, filho2, arqMeta, arqDados, arqNoInterno);

                    arv->pai->filho[0]->filho[arv->pai->filho[0]->numChaves]->proxPag = arv->pai->filho[1]->filho[0];
                   // printf("\n\n\n\n\n PROXPAAAAAAAAAAAAAAAAG = %d\n",arv->pai->filho[0]->filho[arv->pai->filho[0]->numChaves]->proxPag->chave[0]);

                   //      escreveNoInternoLoc(arv->pai->pai->filho[1],arqNoInterno, arv->pai->pai->filho[1]->pos);

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

                 arv->pai =  insereFilhoLoc(arv->pai, filho2, v[M] , arqMeta, arqDados, arqNoInterno);

                atualizaNoLoc(arv->pai->filho[i], arqMeta, arqDados, arqNoInterno);
            }

        }

}


void insereLoc(ArvLoc* arv, Locacao* locacao, FILE* arqMeta, FILE* arqDados, FILE*arqNoInterno)
{
          int i = arv->numChaves;

            while(i > 0 &&  locacao->numLoc < arv->chave[i-1])
                    i--;
            if(arv->chave[i-1] == locacao->numLoc)
                    printf("O CÓDIGO %d JÁ ESTÁ ESTÁ REGISTRADO! \n", locacao->numLoc);
            else
            {

                        if(arv->numChaves == 2*M && verificaFolhaLoc(arv) == 1)
                        {
                                particionaLoc(arv, locacao, arqMeta, arqDados, arqNoInterno);
                        }
                        else
                        {

                            if(verificaFolhaLoc(arv) == 0)
                            {
                                    int i = arv->numChaves;
                                    while(i > 0 && locacao->numLoc < arv->chave[i - 1])
                                            i--;

                                    insereLoc(arv->filho[i], locacao, arqMeta, arqDados, arqNoInterno);
                            }
                            else
                                inserePaginaNaoCheiaLoc(arv, locacao, arqMeta, arqDados, arqNoInterno);

                        }
            }
}

