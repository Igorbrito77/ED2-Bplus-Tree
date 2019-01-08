main:	main.o	insere.o  insereDvd.o insereLoc.o exclui.o excluiDvd.o excluiLoc.o busca.o buscaDvd.o buscaLoc.o  arquivos.o arqCl.o arqLoc.o arqDvd.o interno.o
	gcc main.o insere.o insereDvd.o insereLoc.o exclui.o excluiDvd.o busca.o excluiLoc.o buscaDvd.o buscaLoc.o arquivos.o arqCl.o arqLoc.o arqDvd.o interno.o -o main

main.o: main.c
	gcc -c main.c

insere.o: insere.c
	  gcc -c insere.c

insereDvd.o: insereDvd.c
	gcc -c insereDvd.c

insereLoc.o: insereLoc.c
	gcc -c insereLoc.c

exclui.o: exclui.c
	gcc -c exclui.c

excluiDvd.o: excluiDvd.c
	gcc -c excluiDvd.c

excluiLoc.o: excluiLoc.c
	gcc -c excluiLoc.c

busca.o: busca.c 
	gcc -c busca.c

buscaDvd.o: buscaDvd.c
	gcc -c buscaDvd.c

buscaLoc.o: buscaLoc.c 
	gcc -c buscaLoc.c 

arquivos.o:	arquivos.c
	gcc -c arquivos.c

arqCl.o:	arqCl.c
	gcc -c arqCl.c

arqLoc.o: 	arqLoc.c
	gcc -c arqLoc.c

arqDvd.o:	arqDvd.c
	gcc -c arqDvd.c 

interno.o:	interno.c
	gcc -c interno.c

clean:
	rm*.o  
