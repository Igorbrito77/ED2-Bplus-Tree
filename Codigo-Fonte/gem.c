#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 2
// typedef struct triangulo T;
// struct triangulo
// {
//         int l;
//         char nome[50];
// };

// typedef struct circulo C;
// struct circulo
// {
//         double r;
//         char nome[50];
// };

// typedef struct figura Figura;
// struct figura{
//         void* dado;
//         char nome[50];
// };


typedef struct cliente Cliente;
struct cliente
{
        int cod;
        char nome[100];
};

typedef struct dvd Dvd;
struct dvd
{
        int cod;
        char titulo[100];
};


typedef struct arvB ArvB;
struct arvB
{
        ArvB* pai;
        ArvB* filho[2*M + 1];
        ArvB*proxPag;
        void* dados[2*M];
        int numChaves;
        int chave[2*M];
        int pos;
};



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
                arv->dados[i] = NULL;
        }

        return arv;
}

// void imprime(Figura* f, void *s, int id)
// {

//                        T* tri = (T*) s;
//             f->dado = tri;
//              printf("NOme: %s . Num: %d\n", f->nome, tri->l);

// }

void insere(ArvB* arv, void* dado, int id)
{
        Cliente* c;
        if(id == 1)
        {
                Cliente* c = (Cliente*) dado;
                arv->dados[0] = c;
        }
        else if(id ==2 )
        {
                Dvd* d = (Dvd*) dado;
                arv->dados[0] = d;
        }
}

void imprime(ArvB* arv, int id)
{
        if(id == 1)
            printf("%s\n", ( (Cliente*) arv->dados[0] )->nome);
        else if(id ==2)
            printf("%s\n", ( (Dvd*) arv->dados[0] )->titulo);
}

int main()
{


        ArvB* arv = criaVazia();
        ArvB* arvDvd = criaVazia();

        Cliente* c1 = (Cliente*) malloc(sizeof(Cliente));
        Dvd* d1 = (Dvd*) malloc(sizeof(Dvd));

        strcpy(c1->nome, "IgorBrito");
        strcpy(d1->titulo, "CapitaoAmerica");

        insere(arv, c1, 1);
        insere(arvDvd, d1, 2);

       // imprime(arv, 1);
        imprime(arvDvd, 2);
    return 0;
}


// void insere(ArvB* arv, Cliente* cliente, FILE* arqMeta, FILE* arqDados, FILE*arqNoInterno)
// {
//         if(arv->numChaves == 2*M && verificaFolha(arv) == 1)
//         {
//                 particiona(arv, cliente, arqMeta, arqDados, arqNoInterno);
//         }
//         else
//         {

//             if(verificaFolha(arv) == 0)
//             {
//                     int i = arv->numChaves;
//                     while(i > 0 && cliente->cod < arv->chave[i - 1])
//                             i--;

//                     insere(arv->filho[i], cliente, arqMeta, arqDados, arqNoInterno);
//             }
//             else
//                 inserePaginaNaoCheia(arv, cliente, arqMeta, arqDados, arqNoInterno);

//         }
// }