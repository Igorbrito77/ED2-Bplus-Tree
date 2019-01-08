#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agencia.h"

#define M 2

int tamanhoDvd()
{
        return (sizeof(int) *2 +
                    sizeof(char)*100) *4;
}


void atualizaPaiDvd(FILE* arqMeta, int pos)
{
                rewind(arqMeta);
                fwrite(&pos, sizeof(int), 1, arqMeta);
}

void atualizaNoDvd(ArvDvd* arv, FILE* arqMeta, FILE*arqDados, FILE* arqNoInterno)
{

          escreveDvd(arv, arqDados, arv->pos);
          if(arv->pai != NULL)
                escreveNoInternoDvd(arv->pai, arqNoInterno, arv->pai->pos);
            else
                escreveNoInternoDvd(arv, arqNoInterno, arv->pos);

}

void escreveNoInternoDvd(ArvDvd* arv, FILE* arqNoClt, int pos)
{

         int num  = 1;
         fseek(arqNoClt, tamanhoNoInterno()*pos, SEEK_SET);

         fwrite(&arv->numChaves, sizeof(int), 1, arqNoClt);

         if(verificaFolhaDvd(arv) == 1 || (arv->filho[0] != NULL && verificaFolhaDvd(arv->filho[0]) == 1 && verificaFolhaDvd(arv) == 0))// se 1 , aponta pra olha , 0 aponta pra nó interno
                fwrite(&num, sizeof(int), 1, arqNoClt);
            else
            {
                     num = 0;
                      fwrite(&num, sizeof(int), 1, arqNoClt);
            }

            if(arv->pai != NULL)
                    fwrite(&arv->pai->pos, sizeof(int), 1, arqNoClt);///// eescreve a posição do pai
            else
            {
                    num = -1;
                    fwrite(&num, sizeof(int), 1, arqNoClt);
            }

            for(int i = 0; i < 2*M; i++)  // escre as chaves
                    fwrite(&arv->chave[i], sizeof(int), 1, arqNoClt);

            for(int i = 0; i < 2*M+1; i++) // escreve a posição dos filhos
            {
                    if(arv->filho[i] != NULL)
                        fwrite(&arv->filho[i]->pos, sizeof(int), 1, arqNoClt);
                    else
                    {
                            num = -1;
                            fwrite(&num, sizeof(int), 1, arqNoClt);
                    }
            }

}


void  escreveNovoDvd(ArvDvd* arv, FILE*arqMeta, FILE*arqDados, FILE* arqNoInterno)
{
                int pos = proxPosicaoFolha(arqMeta);
                escreveDvd(arv, arqDados, pos);
                pos = proxPosicaoInterna(arqMeta);
                escreveNoInternoDvd(arv, arqNoInterno, pos);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void insercaoDvd(ArvDvd* arv, FILE*arqMeta, FILE* arqDados,  FILE* arqNoInterno)
{
            printf("________________EFETUAR REGISTRO DVDS_____\n");
            Dvd* dvd  = (Dvd*) malloc(sizeof(Dvd));

            printf("Digite o código do Dvd: \n");
            scanf("%d", &dvd->cod);

            printf("Digite o título: \n");
            scanf("%s", dvd->titulo);

            printf("Digite o ano de lançamento: \n");
            scanf("%d", &dvd->ano);

            insereDvd(arv, dvd, arqMeta, arqDados, arqNoInterno);
}

// void insercaoDvd(ArvDvd* arv, FILE*arqMeta, FILE* arqDados,  FILE* arqNoInterno, int cod, char titulo[100])
// {
//             Dvd* dvd  = (Dvd*) malloc(sizeof(Dvd));
//             dvd->cod = cod;
//             strcpy(dvd->titulo, titulo);
//             dvd->ano = 1900;

//             insereDvd(arv, dvd, arqMeta, arqDados, arqNoInterno);
// }


void exclusaoDvd(ArvDvd* arv, FILE*arqMeta, FILE* arqDados, FILE* arqNoInterno)
{
        printf("_______________ ESCLUSÃO DE REGISTRO DE DVD_______\n");

        int cod;
        printf("Digite o código do Dvd: \n");
        scanf("%d", &cod);
        excluiDvd(arv, cod, arqMeta, arqDados, arqNoInterno);

}

// void exclusaoDvd(ArvDvd* arv, FILE*arqMeta, FILE* arqDados, FILE* arqNoInterno, int cod)
// {
//         excluiDvd(arv, cod, arqMeta, arqDados, arqNoInterno);

// }

void procuraDvd(ArvDvd* arv)
{
            printf("\n\n\n_____________BUSCA DE REGISTRO DE DVD__________\n");
            int cod;

            printf("Digite o código do dvd: \n");
            scanf("%d", &cod);

            buscaDvd(arv, cod);
}

void procuraNomeDvd(ArvDvd* arv)
{
            printf("\n\n\n_____________BUSCA DE REGISTRO DE DVD POR NOME__________\n");
            char titulo[100];

            printf("Digite o título do dvd: \n");
            scanf("%s", titulo);

            buscaNomeDvd(arv, titulo);
}


void procuraDvdAno(ArvDvd* arv, FILE* arqDados)
{
        printf("____________________BUSCA DE DVD POR ANO SUPERIOR__________________\n");
        int x;

        printf("Digite o ano x de referência: \n");
        scanf("%d", &x);

        buscaMaiorAno(arv, x);
}


void escreveDvd(ArvDvd* arv, FILE* arqDvd, int pos)
{
        fseek(arqDvd, tamanhoDvd()*pos, SEEK_SET);

        int n = -1;
        char vazio[100] = "vazio";

        for(int i = 0; i < 2*M; i++)
        {
                    if(arv->dvds[i] == NULL)
                    {
                            fwrite(&n, sizeof(int), 1, arqDvd);
                            fwrite(vazio, sizeof(char), sizeof(vazio), arqDvd);
                            fwrite(&n, sizeof(int), 1, arqDvd);
                    }

                    else
                    {
                            fwrite(&arv->dvds[i]->cod, sizeof(int),1, arqDvd);
                            fwrite(arv->dvds[i]->titulo, sizeof(char), sizeof(arv->dvds[i]->titulo), arqDvd);
                            fwrite(&arv->dvds[i]->ano, sizeof(int), 1, arqDvd);
                    }
        }
}


void imprimeDvd(FILE* arqDvd)
{
            printf("...................................................LISTA DE DVDS...............................\n");
            int cod,ano, cont = 0, pos = 0;
            char titulo[100];
            rewind(arqDvd);

            while(fread(&cod, sizeof(int), 1, arqDvd) > 0)
            {
                    fread(titulo, sizeof(char), sizeof(titulo), arqDvd);
                    fread(&ano, sizeof(int), 1, arqDvd);

                    if(cod != -1)
                   {

                            printf("Código do Dvd: %d\n", cod);
                            printf("Título: %s\n", titulo);
                            printf("Ano de lançamento: %d\n", ano);
                            printf("_____________________________________________________________\n");
                    }


            }
}


ArvDvd* montaArvoreDvd(FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)
{
        ArvDvd* arv = criaDvdVazia();

        int raiz, no, n;
        int tam = sizeof(int)* 7;

        fseek(arqMeta, sizeof(int), SEEK_SET);
        fread(&raiz, sizeof(int), 1, arqMeta);
      //  printf("raiz == %d\n", raiz);

        fseek(arqNoInterno, raiz*tamanhoNoInterno(), SEEK_SET);


        fread(&arv->numChaves, sizeof(int), 1, arqNoInterno); // lê o número de chaves
        //printf("numChaves == %d\n", arv->numChaves);
        fread(&no, sizeof(int), 1, arqNoInterno); // verifica se aponta para folha
        //printf("No == %d\n", no);
        fseek(arqNoInterno, sizeof(int), SEEK_CUR);
        arv->pai = NULL;

        for(int i = 0; i < 2*M; i++)
        {
                fread(&arv->chave[i], sizeof(int), 1, arqNoInterno); // recebe as chaves

               //  printf("chaves == %d\n", arv->chave[i]);
        }


         if(no == 1)
        {
              //  printf("FOLHA\n");
                for(int i = 0; i < 2*M+1; i++)
                {
                        fread(&n, sizeof(int), 1, arqNoInterno);
                      //  printf("filhos = %d\n", n);
                        if(n == -1)
                            arv->filho[i] = NULL;
                        else
                        {
                                arv->filho[i] = criaNoDvd(arqDados, n);
                                arv->filho[i]->pai = arv;
                                if(arv->filho[i-1] != NULL)
                                    arv->filho[i-1]->proxPag = arv->filho[i];
                        }

                }

                if(verificaFolhaDvd(arv) == 1)
                    arv = criaNoDvd(arqDados, 0);
        }

        else
        {
            //printf("INTERNO\n");
            for(int i = 0; i < 2*M+1; i++)
             {
                    fseek(arqNoInterno, tam + sizeof(int)*i, SEEK_SET);// faço isso para retornar a posição do prox filho do arquivo
                    fread(&n, sizeof(int), 1, arqNoInterno);
                    //printf("\n\nDO INTERNO É %d\n", n);

                    if(n == -1)
                                arv->filho[i] = NULL;

                    else
                     {
                              arv->filho[i] = criaNoInternoDvd(arqDados, arqNoInterno, n);
                              arv->filho[i]->pai = arv;

                            if(arv->filho[i-1] != NULL)
                                    arv->filho[i-1]->proxPag = arv->filho[i];
                    }
            }


        }

        return arv;
}

ArvDvd* criaNoInternoDvd(FILE* arqDados, FILE* arqNoInterno, int pos)
{
            int no, n, tam = sizeof(int)*7;
            ArvDvd* arv = criaDvdVazia();

            fseek(arqNoInterno, tamanhoNoInterno()* pos, SEEK_SET);

                fread(&arv->numChaves, sizeof(int), 1, arqNoInterno); // lê o número de chaves
                //printf("numChaves == %d\n", arv->numChaves);
                fread(&no, sizeof(int), 1, arqNoInterno); // verifica se aponta para folha
                //printf("No == %d\n", no);
                fseek(arqNoInterno, sizeof(int), SEEK_CUR);


            for(int i = 0; i < 2*M; i++)//recebe as chaves
            {
                        fread(&arv->chave[i], sizeof(int), 1, arqNoInterno);
                        //printf("chaves == %d\n", arv->chave[i]);
            }

                if(no == 1)
                {

                        for(int i = 0; i < 2*M+1; i++)
                        {
                                fseek(arqNoInterno, pos*tamanhoNoInterno()+ tam+sizeof(int)*i, SEEK_SET); // para retornar à posição do arquivo , para o próximo filho
                                fread(&n, sizeof(int), 1, arqNoInterno);
                                //printf("filhos = %d\n", n);
                                if(n == -1)
                                    arv->filho[i] = NULL;
                                else
                                {
                                        arv->filho[i] = criaNoDvd(arqDados, n);
                                        arv->filho[i]->pai = arv;
                                        if(arv->filho[i-1] != NULL)
                                            arv->filho[i-1]->proxPag = arv->filho[i];
                                }

                        }
                }

                else
                {
                    for(int i = 0; i < 2*M+1; i++)
                     {
                            fread(&n, sizeof(int), 1, arqNoInterno);
                            if(n == -1)
                                arv->filho[i] = NULL;

                            else
                                arv->filho[i] = criaNoInternoDvd(arqDados, arqNoInterno, n);
                    }
        }
}


ArvDvd* criaNoDvd(FILE* arqDados, int pos)
{
        ArvDvd* arv = criaDvdVazia();
        char nome[100];
        int n, tamPula = sizeof(int) + sizeof(char)*100;

        fseek(arqDados, tamanhoDvd()*pos, SEEK_SET);

        for(int i = 0; i < 2*M; i++)
        {
                fread(&n, sizeof(int), 1, arqDados);// recebe

                if(n == -1)
                {
                        arv->dvds[i] = NULL;
                        fseek(arqDados, tamPula, SEEK_CUR);
                }
                else
                {
                    arv->dvds[i] = (Dvd*) malloc(sizeof(Dvd));

                    arv->dvds[i]->cod = n; // recebe o código do dvd
                    arv->chave[i] = arv->dvds[i]->cod;
                    fread(nome, sizeof(char), sizeof(nome), arqDados);
                    strcpy(arv->dvds[i]->titulo, nome);  //Recebe o nome do dvd
                    fread(&arv->dvds[i]->ano, sizeof(int), 1, arqDados);
                  //  printf("%d\n", arv->dvds[i]->cod);
                    arv->chave[i] = arv->dvds[i]->cod; // recebe o ano
                    arv->numChaves++; // conta o  o número de chaves
                    //printf("_____________________________________________________________________________\n");
                }
        }

     //   printf("_____________________________________________________________________________\n");

        arv->pos = pos;

        //printf("arv->pos == %d\n", arv->pos);
        for(int j =0; j < 2*M+1; j++)
                    arv->filho[j] = NULL;

        return arv;
}
