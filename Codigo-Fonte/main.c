#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agencia.h"
#define M 2

int main()
{

    FILE* arqMetaClt = fopen("metaClientes.dat", "r+b");
    FILE* arqMetaDvd = fopen("metaDvds.dat", "r+b");
    FILE* arqMetaLoc = fopen("metaLocacao.dat", "r+b");

    FILE*  arqClientes = fopen("clientes.dat", "r+b");
     FILE* arqDvd = fopen("dvds.dat", "r+b");
     FILE* arqLoc = fopen("locacoes.dat", "r+b");

   FILE* arqNoDvd = fopen("niDvd.dat", "r+b");
   FILE* arqNoLoc = fopen("niLocacao.dat", "r+b");
    FILE* arqNoClt = fopen("niClientes.dat", "r+b");

    ArvB* arvCl;
    ArvDvd* arvDvd;
    ArvLoc* arvLoc;

   //arvCl = montaArvore(arqMetaClt, arqClientes, arqNoClt);
  // arvDvd = montaArvoreDvd(arqMetaDvd, arqDvd, arqNoDvd);
  // arvLoc = montaArvoreLocacao(arqMetaLoc, arqLoc, arqNoLoc);
   arvCl = criaVazia();
   arvDvd = criaDvdVazia();
  arvLoc = criaLocVazia();

       int sel;

        do{

                menuPrincipal();
                scanf("%d", &sel);

                if(sel == 1)
                {
                        do{
                                menuClientes();
                                scanf("%d", &sel);

                                if(sel == 1)
                                    insercao(arvCl, arqMetaClt, arqClientes, arqNoClt);
                                else if(sel == 2)
                                    exclusao(arvCl, arqMetaClt, arqClientes, arqNoClt);
                                else if(sel == 3)
                                    procura(arvCl);
                                else if(sel == 4)
                                    procuraNome(arvCl);
                                  else if(sel == 5)
                                      imprimeClientes(arqClientes);
                                  else if(sel == 777)
                                      internoCliente(arqClientes, arqNoClt, arqMetaClt);

                            }while(sel != 9);
                }
                else if(sel == 2)
                {
                      do{
                              menuDvds();
                              scanf("%d", &sel);

                              if(sel == 1)
                                  insercaoDvd(arvDvd, arqMetaDvd, arqDvd, arqNoDvd);
                              else if(sel == 2)
                                  exclusaoDvd(arvDvd, arqMetaDvd, arqDvd, arqNoDvd);
                              else if(sel == 3)
                                  procuraDvd(arvDvd);
                              else if(sel == 4)
                                  procuraNomeDvd(arvDvd);
                              else if(sel == 5)
                                  imprimeDvd(arqDvd);
                              else if(sel == 7)
                                  procuraDvdAno(arvDvd, arqDvd);
                              else if(sel == 777)
                                  internoDvd(arqDvd, arqNoDvd, arqMetaDvd);
                      }while(sel != 9);
                }

                else if(sel == 3)
               {
                        do{
                                      menuLocacoes();
                                      scanf("%d", &sel);

                                      if(sel == 1)
                                          insercaoLoc(arvLoc, arvCl, arvDvd, arqMetaLoc, arqLoc, arqNoLoc);
                                      else if(sel == 2)
                                          exclusaoLoc(arvLoc, arqMetaLoc, arqLoc, arqNoLoc);
                                      else if(sel == 3)
                                          procuraLoc(arvLoc);
                                      else if(sel == 4)
                                            devolucao(arvLoc, arqLoc);
                                      else if(sel == 5)
                                            imprimeLoc(arqLoc);
                                      else if(sel == 6)
                                            procuraPendencia(arvLoc, arvCl);
                                      else if (sel == 7)
                                            procuraPendenciaX(arvLoc,arvCl);
                                      else if(sel == 777)
                                            internoLocacao(arqLoc, arqNoLoc, arqMetaDvd);

                        }while(sel != 9);

               }

        }while(sel != 0);

    fclose(arqClientes);
     fclose(arqDvd);
    fclose(arqLoc);

    fclose(arqMetaClt);
    fclose(arqMetaDvd);
   fclose(arqMetaLoc);

    fclose(arqNoClt);
    fclose(arqNoDvd);
     fclose(arqNoLoc);
    return 0;
}