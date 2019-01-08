#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agencia.h"
#define M 2


int tamanhoLocacao()
{
        return  ( sizeof(int)*4 +
                    (sizeof(char)*20)*2 )*4;
}


int insercaoLoc(ArvLoc* arv,  ArvB* arvCliente, ArvDvd* arvDvd,  FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)
{
        printf("_______________________EFETUAR LOCAÇÃO DE DVD_______________\n");

        Locacao* loc = (Locacao*) malloc(sizeof(Locacao));

       printf("Digite o código do cliente: \n");
       scanf("%d", &loc->codCliente);

       //verifica se o códifo do cliente é válido
       printf("\n\nVerificando se o código de cliente é válido...\n");
       if(busca(arvCliente, loc->codCliente) == 1)
        {
                printf("Código Válido !\n");
        }
        else
         {
                    printf("Código inválido !\n");
                       return 0;
        }

       printf("Digite o código do Dvd: \n");
       scanf("%d", &loc->codDvd);

        printf("\n\n Verificando se o código do Dvd é válido...\n\n\n");
        if(buscaDvd(arvDvd, loc->codDvd) == 1)
            printf("\n\n______________Código Válido!________________\n\n");
        else
          {
                    printf("\n\n_________Código inválido!________________\n\n");
                    return 0;
            }

       printf("Digite a data de empréstimo: \n");
       scanf("%s", loc->dataEmp);

       printf("Digite o número da locação: \n");
       scanf("%d", &loc->numLoc);

        strcpy(loc->dataDev, "--");
        loc->status = 0;

        insereLoc(arv, loc, arqMeta, arqDados, arqNoInterno);
}


void exclusaoLoc(ArvLoc* arv, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)
{
        printf("_________________EXCLUSÃO DE LOCAÇÃO DE DVD______________________\n");
            int cod;

            printf("Digite o código do cliente que recebeu o dvd: \n");
            scanf("%d", &cod);

            excluiLoc(arv, cod, arqMeta, arqDados, arqNoInterno);
}

void devolucao(ArvLoc* arv, FILE* arqDados)
{
        printf("_____________________ REGISTRAR DEVOLUÇÃO ___________________________\n");
        int num;

        printf("Digite o número da locação: \n");
        scanf("%d", &num);

        int pos = buscaDevolucao(arv, num);
        if(pos != -1)
            escreveLocacao(arv,arqDados, pos);
}

void procuraPendencia(ArvLoc* arv, ArvB* arvCl)
{
        printf("\n\n______________________________PROCURANDO POR PENDÊNCIA NA DEVOLUÇÃO.... ____________\n");

        buscaPendente(arv, arvCl);
}

void procuraPendenciaX(ArvLoc* arv, ArvB* arvCl)
{
        printf("Digite o número de locações como parâmetro: \n");
        int x;
        scanf("%d", &x);

        buscaPendenciaX(arv, arvCl, x);
}

void procuraLoc(ArvLoc* arv)
{
        printf("_________________ BUSCA DE LOCAÇÃO __________________________\n");

        int num;
        printf("Digite o número da locação:\n");
        scanf("%d", &num);

        buscaLoc(arv, num);
}

// void insercaoLoc(ArvLoc* arv, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno, int cod, int cod2)
// {
//         Locacao* loc = (Locacao*) malloc(sizeof(Locacao));
//         loc->codCliente = cod;
//         loc->codDvd = cod2;
//         strcpy(loc->dataEmp, "01/01/01");
//         strcpy(loc->dataDev, "07/07/07");
//         loc->status = 0;

//         insereLoc(arv, loc, arqMeta, arqDados, arqNoInterno);
// }

void escreveNovaLocacao(ArvLoc* arv, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)
{
            int pos = proxPosicaoFolha(arqMeta);
            escreveLocacao(arv, arqDados, pos);
            pos = proxPosicaoInterna(arqMeta);
            escreveNoInternoLoc(arv, arqNoInterno, pos);
}


void atualizaNoLoc(ArvLoc* arv, FILE* arqMeta, FILE*arqDados, FILE* arqNoInterno)
{

          escreveLocacao(arv, arqDados, arv->pos);
          if(arv->pai != NULL)
                escreveNoInternoLoc(arv->pai, arqNoInterno, arv->pai->pos);

            else
                escreveNoInternoLoc(arv, arqNoInterno, arv->pos);

}

void escreveLocacao(ArvLoc* arv, FILE* arqLoc, int pos)
{
        fseek(arqLoc, tamanhoLocacao()*pos, SEEK_SET);
        int n = -1;
        char vazio[20] = "--";

        for(int i = 0; i < 2*M; i++)
        {
                if(arv->locs[i] == NULL)
                {
                            fwrite(&n, sizeof(int),1 , arqLoc);
                            fwrite(&n, sizeof(int),1 , arqLoc);
                            fwrite(&n, sizeof(int),1 , arqLoc);
                            fwrite(vazio, sizeof(char), sizeof(vazio), arqLoc);
                            fwrite(vazio, sizeof(char), sizeof(vazio), arqLoc);
                            fwrite(&n, sizeof(int),1 , arqLoc);
                         //   printf("escrevi nulll\n");
                }

                else
                {
                        fwrite(&arv->locs[i]->numLoc, sizeof(int), 1, arqLoc);
                        fwrite(&arv->locs[i]->codCliente, sizeof(int), 1, arqLoc);
                      //  printf("escrevendooo:: %d\n", arv->locs[i]->codCliente);
                        fwrite(&arv->locs[i]->codDvd, sizeof(int), 1, arqLoc);
                        fwrite(arv->locs[i]->dataEmp, sizeof(char), sizeof(arv->locs[i]->dataEmp), arqLoc);
                        fwrite(arv->locs[i]->dataDev, sizeof(char), sizeof(arv->locs[i]->dataDev), arqLoc);
                        fwrite(&arv->locs[i]->status, sizeof(int), 1, arqLoc);
                }
        }
}


void imprimeLoc(FILE* arqLoc)
{
              printf("....................................................................LISTA DE LOCAÇÕES.........................................\n");

        int  numLoc,  codCliente, codDvd, status;
        char dataEmp[20], dataDev[20], pos = 0, cont = 0;

        rewind(arqLoc);

        while(fread(&numLoc, sizeof(int), 1, arqLoc) > 0)
        {
                fread(&codCliente, sizeof(int), 1, arqLoc);
                fread(&codDvd, sizeof(int), 1, arqLoc);
                fread(dataEmp, sizeof(char), sizeof(dataEmp), arqLoc);
                fread(dataDev, sizeof(char), sizeof(dataDev), arqLoc);
                fread(&status, sizeof(int), 1, arqLoc);

                if(numLoc != -1){

                        printf("Número da locação: %d\n", numLoc);
                        printf("Código do Cliente: %d\n", codCliente);
                        printf("Código do Dvd: %d\n", codDvd);
                        printf("Data de empréstimo: %s\n", dataEmp);
                        printf("Data de devolução: %s\n", dataDev);

                        if(status == 0)
                            printf("Não devolvido\n");
                        else
                            printf("Devolução efetuada\n");

                        printf("______________________________________________________________________________\n");
            }

        }
}

void escreveNoInternoLoc(ArvLoc* arv, FILE* arqNoLoc, int pos)
{
//printf("ESCREVI LOCAÇÂO.............................................................................................................................\n");
         int num  = 1;
         fseek(arqNoLoc, tamanhoNoInterno()*pos, SEEK_SET);

         fwrite(&arv->numChaves, sizeof(int), 1, arqNoLoc);

         if(verificaFolhaLoc(arv) == 1 || (arv->filho[0] != NULL && verificaFolhaLoc(arv->filho[0]) == 1 && verificaFolhaLoc(arv) == 0) )// se 1 , aponta pra folha , 0 aponta pra nó interno
                fwrite(&num, sizeof(int), 1, arqNoLoc);
            else
            {
                     num = 0;
                      fwrite(&num, sizeof(int), 1, arqNoLoc);
            }

            if(arv->pai != NULL)
                    fwrite(&arv->pai->pos, sizeof(int), 1, arqNoLoc);///// eescreve a posição do pai
            else
            {
                    num = -1;
                    fwrite(&num, sizeof(int), 1, arqNoLoc);
            }

            for(int i = 0; i < 2*M; i++)  // escre as chaves
                    fwrite(&arv->chave[i], sizeof(int), 1, arqNoLoc);

            for(int i = 0; i < 2*M+1; i++) // escreve a posição dos filhos
            {
                    if(arv->filho[i] != NULL)
                        fwrite(&arv->filho[i]->pos, sizeof(int), 1, arqNoLoc);
                    else
                    {
                            num = -1;
                            fwrite(&num, sizeof(int), 1, arqNoLoc);
                    }
            }

}



ArvLoc* montaArvoreLocacao(FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)
{
        ArvLoc* arv = criaLocVazia();

        int raiz, no, n;
        int tam = sizeof(int)* 7;

        fseek(arqMeta, sizeof(int), SEEK_SET);
        fread(&raiz, sizeof(int), 1, arqMeta);

        fseek(arqNoInterno, raiz*tamanhoNoInterno(), SEEK_SET);


        fread(&arv->numChaves, sizeof(int), 1, arqNoInterno); // lê o número de chaves
        fread(&no, sizeof(int), 1, arqNoInterno); // verifica se aponta para folha

        fseek(arqNoInterno, sizeof(int), SEEK_CUR);
        arv->pai = NULL;

        for(int i = 0; i < 2*M; i++)
                fread(&arv->chave[i], sizeof(int), 1, arqNoInterno); // recebe as chaves

         if(no == 1)
        {
              //  printf("FOLHA\n");
                for(int i = 0; i < 2*M+1; i++)
                {
                        fread(&n, sizeof(int), 1, arqNoInterno);

                        if(n == -1)
                            arv->filho[i] = NULL;
                        else
                        {
                                arv->filho[i] = criaNoLocacao(arqDados, n);
                                arv->filho[i]->pai = arv;
                                if(arv->filho[i-1] != NULL)
                                    arv->filho[i-1]->proxPag = arv->filho[i];
                        }

                }

                if(verificaFolhaLoc(arv) == 1)
                    arv = criaNoLocacao(arqDados, 0);
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
                              arv->filho[i] = criaNoInternoLoc(arqDados, arqNoInterno, n);
                              arv->filho[i]->pai = arv;

                            if(arv->filho[i-1] != NULL)
                                    arv->filho[i-1]->proxPag = arv->filho[i];
                    }
            }


        }

        return arv;
}

ArvLoc* criaNoInternoLoc(FILE* arqDados, FILE* arqNoInterno, int pos)
{
            int no, n, tam = sizeof(int)*7;
            ArvLoc* arv = criaLocVazia();

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
                                        arv->filho[i] = criaNoLocacao(arqDados, n);
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
                                arv->filho[i] = criaNoInternoLoc(arqDados, arqNoInterno, n);
                    }
        }
}


ArvLoc* criaNoLocacao(FILE* arqDados, int pos)
{
//printf("CRIA NOOOOOOOOOOOOOOOoo\n");
        ArvLoc* arv = criaLocVazia();
        char nome[100];
        int n, tamPula = sizeof(int) + sizeof(char)*100;

        fseek(arqDados, tamanhoLocacao()*pos, SEEK_SET);

        for(int i = 0; i < 2*M; i++)
        {
                fread(&n, sizeof(int), 1, arqDados);// recebe

                if(n == -1)
                {
                        arv->locs[i] = NULL;
                        fseek(arqDados, tamPula, SEEK_CUR);
                }
                else
                {
                    arv->locs[i] = (Locacao*) malloc(sizeof(Locacao));

                    arv->locs[i]->numLoc = n; // recebe o código da locação
                    arv->chave[i] = arv->locs[i]->numLoc;
                    fread(&n, sizeof(int), 1, arqDados);//recebe o código do cliente
                    arv->locs[i]->codCliente = n;
                    fread(&n, sizeof(int), 1, arqDados);// recebe o código do dvd
                    arv->locs[i]->codDvd = n;
                    fread(arv->locs[i]->dataEmp, sizeof(char), sizeof(arv->locs[i]->dataEmp), arqDados); //recebe a data de emprestimo
                    fread(arv->locs[i]->dataDev, sizeof(char), sizeof(arv->locs[i]->dataDev), arqDados); // recebe a dat de devolução
                    fread(&arv->locs[i]->status, sizeof(int), 1, arqDados);
                    arv->numChaves++; // conta o  o número de chave
                }
        }

        printf("_____________________________________________________________________________\n");

        arv->pos = pos;

        //printf("arv->pos == %d\n", arv->pos);
        for(int j =0; j < 2*M+1; j++)
                    arv->filho[j] = NULL;

        return arv;
}
