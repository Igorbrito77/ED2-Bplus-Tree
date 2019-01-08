#ifndef AGENCIA_H   // guardas de cabeçalho, impedem inclusões cíclicas
#define AGENCIA__H
#define M 2

typedef struct cliente Cliente;
struct cliente
{
        int cod; // Índice Primário
        char nome[100];
        char tel[20];
};

typedef struct dvd Dvd;
struct dvd
{
      int cod; // Índice Primário
      char titulo[100];
      int ano;
};

typedef struct locacao Locacao;
struct locacao
{
      int codCliente;
      int codDvd;
      char dataEmp[20];
      char dataDev[20];
      int status;
      int numLoc; // Índice Primário
};


typedef struct arvB ArvB;
struct arvB
{
        ArvB* pai;
        ArvB* filho[2*M + 1];
        ArvB*proxPag;
        Cliente*clientes[2*M];
        int numChaves;
        int chave[2*M];
        int pos;
};



typedef struct arvDvd ArvDvd;
struct arvDvd
{
        ArvDvd* pai;
        ArvDvd* filho[2*M + 1];
        Dvd* dvds[2*M];
        ArvDvd*proxPag;
        int numChaves;
        int pos;
        int chave[2*M];
};

typedef struct arvLoc ArvLoc;
struct arvLoc
{
           ArvLoc* pai;
          ArvLoc* filho[2*M + 1];
          ArvLoc*proxPag;
          Locacao* locs[2*M];
          int numChaves;
          int chave[2*M];
          int pos;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void insere(ArvB* arv, Cliente* cliente, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno);
void insercao(ArvB* arv, FILE* arqMeta, FILE*arqDados, FILE*ArqNoInterno/*, int cod, char nome[100]*/);

void exclui(ArvB* arv, int num, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno);
void exclusao(ArvB* arv, FILE* arqMeta, FILE* arqDados , FILE* arqNoInterno/*, int cod*/);

void procura(ArvB* arv);
void procuraNome(ArvB* arv);
int busca(ArvB* arv, int num);
void buscaNome(ArvB* arv, char nome[100]);


void particiona(ArvB* arv,  Cliente* cliente, FILE*arqMeta, FILE*arqDados, FILE* arqNoInterno);
void posicionaFilho(ArvB* pai, ArvB* filho, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno);
ArvB* insereFilho(ArvB* pai, ArvB* filho, Cliente* cliente, FILE* arqMeta, FILE*arqDados, FILE* arqNoInterno);
void inserePaginaNaoCheia(ArvB* arv, Cliente* cliente, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno);


ArvB* criaNo(FILE* arqDados, int num);
ArvB* criaVazia();
int verificaFolha(ArvB* arv);

void atualizaPai(FILE* arqMeta, int pos);

ArvB* criaArvore(FILE* arqMeta, FILE* arqDados);
ArvB* criaNo(FILE* arqDados, int num);
void atualizaNo(ArvB* arv, FILE* arqMeta, FILE*arqDados, FILE* arqNoInterno);



void imprimeDados(FILE* arqMeta, FILE*arqDados);
int proxPosicaoInterna(FILE* arqMeta);
int proxPosicaoFolha(FILE* arqMeta);


void menuPrincipal();
void menuClientes();
void menuDvds();
void menuLocacoes();

void escreveCliente(ArvB* arv, FILE* arqClientes, int pos);
void  escreveNovo(ArvB* arv, FILE*arqMeta, FILE*arqDados, FILE* arqNoInterno);
void escreveDvd(ArvDvd* arv, FILE* arqDvd, int pos);
void escreveNoInterno(ArvB* arv, FILE* arqNoClt, int pos);


void imprimeClientes(FILE*arqClientes);
void imprimeNoInterno(FILE* arqNoInterno);
void imprimeMeta(FILE* arqMeta);

ArvB* montaArvore(FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno);
ArvB* criaNo(FILE* arqDados, int pos);
ArvB* criaNoInterno(FILE* arqDados, FILE* arqNoInterno, int pos);


int tamanhoNoInterno();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void  escreveNovoDvd(ArvDvd* arv, FILE*arqMeta, FILE*arqDados, FILE* arqNoInterno);
void atualizaPaiDvd(FILE* arqMeta, int pos);
void atualizaNoDvd(ArvDvd* arv, FILE* arqMeta, FILE*arqDados, FILE* arqNoInterno);
void escreveNoInternoDvd(ArvDvd* arv, FILE* arqNoClt, int pos);
void escreveDvd(ArvDvd* arv, FILE* arqDvd, int pos);


void imprimeDvd(FILE* arqDvd);

int verificaFolhaDvd(ArvDvd* arv);

void insereDvd(ArvDvd* arv, Dvd* dvd, FILE* arqMeta, FILE* arqDados, FILE*arqNoInterno);
void insercaoDvd(ArvDvd* arv, FILE*arqMeta, FILE* arqDvd,  FILE* arqNoInterno/*, int cod, char titulo[100]*/);

void excluiDvd(ArvDvd* arv, int num, FILE* arqMeta, FILE* arqDados , FILE* arqNoInterno);
void exclusaoDvd(ArvDvd* arv, FILE*arqMeta, FILE* arqDados, FILE* arqNoInterno/*, int cod*/);


int buscaDvd(ArvDvd* arv, int num);
void buscaNomeDvd(ArvDvd* arv, char titulo[100]);
void buscaMaiorAno(ArvDvd* arv, int num);
void procuraDvdAno(ArvDvd* arv, FILE* arqDados);
void procuraDvd(ArvDvd* arv);
void procuraNomeDvd(ArvDvd* arv);

ArvDvd* montaArvoreDvd(FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno);
//ArvDvd* montaArvoreDvd(FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno);
ArvDvd* criaNoDvd(FILE* arqDados, int pos);
ArvDvd* criaNoInternoDvd(FILE* arqDados, FILE* arqNoInterno, int pos);

ArvDvd* criaDvdVazia();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


ArvLoc* criaLocVazia();
int verificaFolhaLoc(ArvLoc* arv);

//int insercaoLoc(ArvLoc* arv, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno /*, int cod, int cod2*/);
int insercaoLoc(ArvLoc* arv,  ArvB* arvCliente, ArvDvd* arvDvd,  FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno);
void insereLoc(ArvLoc* arv, Locacao* locacao, FILE* arqMeta, FILE* arqDados, FILE*arqNoInterno);

void exclusaoLoc(ArvLoc* arv, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno);
void excluiLoc(ArvLoc* arv, int num, FILE* arqMeta, FILE* arqDados , FILE* arqNoInterno);

void escreveLocacao(ArvLoc* arv, FILE* arqLoc, int pos);
void escreveNovaLocacao(ArvLoc* arv, FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno);
void escreveNoInternoLoc(ArvLoc* arv, FILE* arqNoClt, int pos);
void atualizaNoLoc(ArvLoc* arv, FILE* arqMeta, FILE*arqDados, FILE* arqNoInterno);

void devolucao(ArvLoc* arv, FILE* arqDados);
int buscaDevolucao(ArvLoc* arv, int num);

void procuraLoc(ArvLoc* arv);
void buscaLoc(ArvLoc* arv, int num);
void procuraPendencia(ArvLoc* arv, ArvB* arvCl);
void buscaPendente(ArvLoc* arv, ArvB* arvCl);
void buscaPendenciaX(ArvLoc* arv, ArvB* arvCl, int x);
void procuraPendenciaX(ArvLoc* arv, ArvB* arvCl);

void imprimeLoc(FILE* arqLoc);

ArvLoc* criaNoLocacao(FILE* arqDados, int pos);
ArvLoc* criaNoInternoLoc(FILE* arqDados, FILE* arqNoInterno, int pos);
ArvLoc* montaArvoreLocacao(FILE* arqMeta, FILE* arqDados, FILE* arqNoInterno);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void internoCliente(FILE* arqDados, FILE* arqNoInterno, FILE* arqMeta);
void internoDvd(FILE* arqDados, FILE* arqNoInterno, FILE* arqMeta);
void internoLocacao(FILE* arqDados, FILE* arqNoInterno, FILE* arqMeta);


#endif

