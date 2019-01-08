#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agencia.h"

#define M 2


void menuPrincipal()
{
        printf("\n\n\n------------------ MENU  PRINCIPAL---------------\n");
        printf("1- TABELA DE CLIENTES\n2- TABELA DE DVDS\n3- TABELA DE LOCAÇÕES\n0- SAIR\n");

}

void menuClientes()
{
        printf("\n\n\n-------------------  CLIENTES ----------------\n");
        printf("1- Registro de novo cliente \n2- Excluir registro de cliente\n3- Buscar cliente cadastrado\n4- Busca por nome\n5- Exibir tabela \n9- Voltar ao menu princpal\n");
}

void menuDvds()
{
        printf("\n\n\n-------------------  DVDS ----------------\n");
        printf("1- Registro de novo DVD \n2- Excluir registro de DVD\n3- Buscar DVD cadastrado\n4- Buscar por título\n5- Exibir tabela\n9- Voltar ao menu princpal\n");
}


void menuLocacoes()
{
        printf("-------------------  LOCAÇÕES ----------------\n");
        printf("1- Efetuar nova locação  \n2- Excluir locação\n3- Buscar locação efetuada\n4- Alterar locação\n5- Exibir tabela\n6- Busca por clientes com pendência \n9- Voltar ao menu princpal\n");
}


int proxPosicaoFolha(FILE* arqMeta)
{
         int pos;
         fseek(arqMeta, sizeof(int)*2, SEEK_SET);
         fread(&pos, sizeof(int), 1, arqMeta);
        int prox =  pos+ 1;
         fseek(arqMeta, sizeof(int)*2, SEEK_SET);
         fwrite(&prox, sizeof(int), 1, arqMeta);

         return pos;
}

int proxPosicaoInterna(FILE* arqMeta)
{
        int pos;
        fseek(arqMeta, sizeof(int)*3, SEEK_SET);
        fread(&pos, sizeof(int), 1, arqMeta);
        int prox = pos + 1;
        fseek(arqMeta, sizeof(int)*3, SEEK_SET);
        fwrite(&prox, sizeof(int), 1, arqMeta);

        return pos;
}


void imprimeMeta(FILE* arqMeta)
{
        int n;
        rewind(arqMeta);
        printf("_____________________________ METADADOS ___________________________\n");

         fread(&n, sizeof(int), 1, arqMeta);
         printf("Raiz: %d\n", n);
         fread(&n, sizeof(int), 1, arqMeta);
         printf("Flag: %d\n", n);
         fread(&n, sizeof(int), 1, arqMeta);
         printf("Próx Posição de dados: %d\n", n);
         fread(&n, sizeof(int), 1, arqMeta);
         printf("Próx Posição de nó interno: %d\n", n);


}



void atualizaPai(FILE* arqMeta, int pos)
{
                rewind(arqMeta);
                fwrite(&pos, sizeof(int), 1, arqMeta);
}
