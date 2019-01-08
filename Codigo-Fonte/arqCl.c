#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agencia.h"

#define M 2

int tamanhoCliente()
{
        return( sizeof(int)+
                    sizeof(char)*100 +
                    sizeof(char)*20) * 4;
}


int tamanhoNoInterno()
{
        return sizeof(int)*12;
}


void insercao(ArvB* arv, FILE* arqMeta, FILE*arqDados, FILE*arqNoInterno)
{
       Cliente* c = (Cliente*) malloc(sizeof(Cliente));

        printf("\n\n\n______________EFETUAR REGISTRO DE CLIENTE_______________\n");
        printf("Digite o código :\n");
        scanf("%d", &c->cod);

        printf("Digite o nome : \n");
        scanf("%s", c->nome);

        printf("Insira o telefone: \n");
        scanf("%s", c->tel);

        insere(arv, c, arqMeta, arqDados, arqNoInterno);

}


// void insercao(ArvB* arv, FILE* arqMeta, FILE*arqDados, FILE*arqNoInterno ,int cod, char nome[100])
// {
//        Cliente* c = (Cliente*) malloc(sizeof(Cliente));
//        c->cod = cod;
//        strcpy(c->nome, nome);
//        strcpy(c->tel , "1111");

//         insere(arv, c, arqMeta, arqDados, arqNoInterno);

// }


void exclusao(ArvB* arv, FILE* arqMeta, FILE* arqDados ,FILE* arqNoInterno)
{
        int cod;

        printf("\n\n\n______________APAGAR REGISTRO_____________________\n");
        printf("Digite o código do cliente:\n");
        scanf("%d", &cod);

        exclui(arv, cod, arqMeta, arqDados, arqNoInterno);

}


// void exclusao(ArvB* arv, FILE* arqMeta, FILE* arqDados ,FILE* arqNoInterno, int cod)
// {
//         exclui(arv, cod, arqMeta, arqDados, arqNoInterno);
// }

void procura(ArvB* arv)
{
        int cod;

        printf("\n\n\n_______________BUSCAR REGISTRO DO CLIENTE______\n");
        printf("Digite o código do cliente: \n");
        scanf("%d", &cod);

        busca(arv, cod);
}

void procuraNome(ArvB* arv)
{
        char nome[100];

        printf("\n\n\n_______________BUSCA DE CLIENTE POR NOME______\n");
        printf("Digite o nome do cliente: \n");
        scanf("%s", nome);

        buscaNome(arv, nome);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void escreveCliente(ArvB* arv, FILE* arqClientes, int pos)
{
        fseek(arqClientes, tamanhoCliente()*pos, SEEK_SET);

        int num  = -1;
        char vazio[100] = "vazio";
        char telVazio[20] = "--";

        for(int i = 0; i < 2*M; i++)
        {

                if(arv->clientes[i] == NULL)
                {
                        fwrite(vazio, sizeof(char), sizeof(vazio), arqClientes);
                        fwrite(&num, sizeof(int), 1, arqClientes);
                        fwrite(telVazio, sizeof(char), sizeof(telVazio), arqClientes);
                }

                else
                {
                    fwrite(arv->clientes[i]->nome, sizeof(char), sizeof(arv->clientes[i]->nome), arqClientes);
                    fwrite(&arv->clientes[i]->cod,  sizeof(int), 1, arqClientes);
                    fwrite(arv->clientes[i]->tel, sizeof(char), sizeof(arv->clientes[i]->tel), arqClientes);
                }
        }

}



void imprimeClientes(FILE*arqClientes)
{
        printf("....................................... LISTA DE CLIENTES ....................\n");

        int cod;
        char nome[100], tel[20];
        int cont = 0, pos = 0 ;

      rewind(arqClientes);

        while( fread(nome, sizeof(char), sizeof(nome), arqClientes) > 0)
        {
                fread(&cod, sizeof(int), 1, arqClientes);
                fread(tel, sizeof(char), sizeof(tel), arqClientes);

             if(cod != -1)
                 {
                        printf("Nome: %s\n", nome);
                        printf("Código: %d\n", cod);
                        printf("Telefone: %s\n", tel);
                        printf("_____________________________________________________\n");
                }
        }
}

void escreveNoInterno(ArvB* arv, FILE* arqNoClt, int pos)
{

         int num  = 1;
       //  printf("\n\n\n POS: --------> %d\n", pos);
         fseek(arqNoClt, tamanhoNoInterno()*pos, SEEK_SET);

         fwrite(&arv->numChaves, sizeof(int), 1, arqNoClt);

         if(verificaFolha(arv) == 1 || (arv->filho[0] != NULL && verificaFolha(arv->filho[0]) == 1 && verificaFolha(arv) == 0) )// se 1 , aponta pra folha , 0 aponta pra nó interno
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void atualizaNo(ArvB* arv, FILE* arqMeta, FILE*arqDados, FILE* arqNoInterno)
{

          escreveCliente(arv, arqDados, arv->pos);
          if(arv->pai != NULL)
                escreveNoInterno(arv->pai, arqNoInterno, arv->pai->pos);

            else
                escreveNoInterno(arv, arqNoInterno, arv->pos);

}


void  escreveNovo(ArvB* arv, FILE*arqMeta, FILE*arqDados, FILE* arqNoInterno)
{
                int pos = proxPosicaoFolha(arqMeta);
                escreveCliente(arv, arqDados, pos);
                pos = proxPosicaoInterna(arqMeta);
                escreveNoInterno(arv, arqNoInterno, pos);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int profundidade(ArvB* arv)
{
        static int cont = 0;

        if(verificaFolha(arv) == 0)
         {
                cont++;
                profundidade(arv->filho[0]);
         }

        return cont;
}


ArvB* montaArvore(FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno)
{
        ArvB* arv = criaVazia();

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

                for(int i = 0; i < 2*M+1; i++)
                {
                        fread(&n, sizeof(int), 1, arqNoInterno);
                      //  printf("filhos = %d\n", n);
                        if(n == -1)
                            arv->filho[i] = NULL;
                        else
                        {
                                arv->filho[i] = criaNo(arqDados, n);
                                arv->filho[i]->pai = arv;
                                if(arv->filho[i-1] != NULL)
                                    arv->filho[i-1]->proxPag = arv->filho[i];
                        }

                }
                        if(verificaFolha(arv) == 1)
                            arv = criaNo(arqDados, 0);
        }

        else
        {
            for(int i = 0; i < 2*M+1; i++)
             {
                    fseek(arqNoInterno, tam + sizeof(int)*i, SEEK_SET);// faço isso para retornar a posição do prox filho do arquivo
                    fread(&n, sizeof(int), 1, arqNoInterno);
                    //printf("\n\nDO INTERNO É %d\n", n);

                    if(n == -1)
                                arv->filho[i] = NULL;

                    else
                     {
                              arv->filho[i] = criaNoInterno(arqDados, arqNoInterno, n);
                              arv->filho[i]->pai = arv;

                            if(arv->filho[i-1] != NULL)
                                    arv->filho[i-1]->proxPag = arv->filho[i];
                    }
            }


        }

        return arv;
}

ArvB* criaNoInterno(FILE* arqDados, FILE* arqNoInterno, int pos)
{
            int no, n, tam = sizeof(int)*7;
            ArvB* arv = criaVazia();

            fseek(arqNoInterno, tamanhoNoInterno()* pos, SEEK_SET);

                fread(&arv->numChaves, sizeof(int), 1, arqNoInterno); // lê o número de chaves
              //  printf("numChaves == %d\n", arv->numChaves);
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
                                        arv->filho[i] = criaNo(arqDados, n);
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
                                arv->filho[i] = criaNoInterno(arqDados, arqNoInterno, n);
                    }
        }
}

ArvB* criaNo(FILE* arqDados, int pos)
{
       // printf("CRIA NOOOOOOOOO DO CLIENTEEEEEEEEE\n");
        ArvB* arv = criaVazia();
        char nome[100];
        int tamPula = sizeof(int) + sizeof(char)*20;

        fseek(arqDados, tamanhoCliente()*pos, SEEK_SET);

        for(int i = 0; i < 2*M; i++)
        {
                fread(nome, sizeof(char), sizeof(nome), arqDados);
               // printf("nome: %s\n", nome);
                if(strcmp(nome, "vazio") == 0)
                {
                        arv->clientes[i] = NULL;
                        fseek(arqDados, tamPula, SEEK_CUR);
                }
                else
                {
                    arv->clientes[i] = (Cliente*) malloc(sizeof(Cliente));

                    //printf("não é vazio\n");
                    strcpy(arv->clientes[i]->nome, nome);  //Recebe o nome do cliente
                    fread(&arv->clientes[i]->cod, sizeof(int), 1, arqDados);// lê o código
                   // printf("código:: %d\n", arv->clientes[i]->cod);
                    arv->chave[i] = arv->clientes[i]->cod; // recebe a chave
                    //printf("arv->chave = %d\n", arv->chave[i]);
                    fread(arv->clientes[i]->tel, sizeof(char), sizeof(arv->clientes[i]->tel), arqDados); // lê io telefone
                   // printf("telefone: %s\n", arv->clientes[i]->tel);
                    arv->numChaves++; // conta o  o número de chaves
                    //printf("_____________________________________________________________________________\n");
                }
        }

        //printf("_____________________________________________________________________________\n");

        arv->pos = pos;

        //printf("arv->pos == %d\n", arv->pos);
        for(int j =0; j < 2*M+1; j++)
                    arv->filho[j] = NULL;

        return arv;
}