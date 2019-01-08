#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agencia.h"
#define M 2

void imprimeClienteInterno(FILE*arqDados)
{
        printf("....................................... LISTA DE CLIENTES ....................\n");

        int cod;
        char nome[100], tel[20];
        int cont = 0, pos = 0 ;

      rewind(arqDados);

        while( fread(nome, sizeof(char), sizeof(nome), arqDados) > 0)
        {
                fread(&cod, sizeof(int), 1, arqDados);
                fread(tel, sizeof(char), sizeof(tel), arqDados);

                        printf("Nome: %s\n", nome);
                        printf("Código: %d\n", cod);
                        printf("Telefone: %s\n", tel);
                        printf("_____________________________________________________\n");

                    if(cont == 3)
                    {
                            printf("......................................POSICAO................:: %d\n", pos);
                            cont = 0;
                            pos++;
                    }
                    else
                        cont++;

        }
}



void imprimeDvdInterno(FILE* arqDvd)
{
            printf("...................................................LISTA DE DVDS...............................\n");
            int cod,ano, cont = 0, pos = 0;
            char titulo[100];
            rewind(arqDvd);

            while(fread(&cod, sizeof(int), 1, arqDvd) > 0)
            {
                    fread(titulo, sizeof(char), sizeof(titulo), arqDvd);
                    fread(&ano, sizeof(int), 1, arqDvd);

                            printf("Código do Dvd: %d\n", cod);
                            printf("Título: %s\n", titulo);
                            printf("Ano de lançamento: %d\n", ano);
                            printf("_____________________________________________________________\n");

                        if(cont == 3)
                            {
                                    printf("......................................POSICAO................:: %d\n", pos);
                                    cont = 0;
                                    pos++;
                            }
                            else
                                cont++;

            }
}


void imprimeLocacaoInterno(FILE* arqLoc)
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

                if(cont == 3)
                    {
                            printf(".......................................................POSICAO................:: %d\n", pos);
                            cont = 0;
                            pos++;
                    }
                    else
                        cont++;
        }
}


void imprimeNoInternoClt(FILE* arqNoInterno)
{

        printf("...............................NÓS INTERNOS.............................\n");

        int num;
        rewind(arqNoInterno);

        while(fread(&num, sizeof(int), 1, arqNoInterno) > 0)
        {
                printf("Número de chaves: %d\n", num);

                fread(&num, sizeof(int), 1, arqNoInterno);
                if(num == 1)
                    printf("Aponta pra nó folha.\n");
                else
                    printf("Aponta para nó interno\n");

                   fread(&num, sizeof(int), 1, arqNoInterno);
                   printf("Nó pai: %d\n", num);

                   printf("Chaves: \n");
                   for(int i = 0; i < 2*M; i++)
                    {
                            fread(&num, sizeof(int), 1, arqNoInterno);
                            printf("%d\n", num);
                    }

                    printf("Filhos: \n");
                     for(int i = 0; i < 2*M+ 1; i++)
                    {
                            fread(&num, sizeof(int), 1, arqNoInterno);
                            printf("%d\n", num);
                    }

                    printf("_______________________________________________________\n");
         }

}

void imprimeMetaDados(FILE* arqMeta)
{
        rewind(arqMeta);
        int num;
        printf("                            METADADOS \n");
        while(fread(&num, sizeof(int), 1, arqMeta) > 0)
        {
                printf("Raiz: %d\n", num);
                fread(&num, sizeof(int), 1, arqMeta);
                //printf("Flag: %d\n", num);
                fread(&num, sizeof(int), 1, arqMeta);
                printf("Próxima Posição de folha: %d\n", num);
                fread(&num, sizeof(int), 1, arqMeta);
                printf("Próxuma posição interna: %d\n", num);
        }
}

void internoCliente(FILE* arqDados, FILE* arqNoInterno, FILE* arqMeta)
{
        imprimeClienteInterno(arqDados);
        imprimeNoInternoClt(arqNoInterno);
        imprimeMetaDados(arqMeta);
}

void internoDvd(FILE* arqDados, FILE* arqNoInterno, FILE* arqMeta)
{
        imprimeDvdInterno(arqDados);
        imprimeNoInternoClt(arqNoInterno);
        imprimeMetaDados(arqMeta);
}

void internoLocacao(FILE* arqDados, FILE* arqNoInterno, FILE* arqMeta)
{
        imprimeLocacaoInterno(arqDados);
        imprimeNoInternoClt(arqNoInterno);
        imprimeMetaDados(arqMeta);
}
