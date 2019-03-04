#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PORT 9091
extern int errno;

int main ()
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;	
  char msg[100];		//mesajul primit de la client 
  char msgrasp[100]=" ";        //mesaj de raspuns pentru client
  int sd;			//descriptorul de socket
  int multiply, interest, sum;  

  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }

  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;	
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 5) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }

  /* servim in mod iterativ clientii... */
  while (1)
    {
      int client;
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      client = accept (sd, (struct sockaddr *) &from, &length);

      /* eroare la acceptarea conexiunii de la un client */
      if (client < 0)
	{
	  perror ("[server]Eroare la accept().\n");
	  continue;
	}

      /* s-a realizat conexiunea, se astepta mesajul */
      bzero (msg, 100);
      printf ("[server]Asteptam mesajul...\n");
      fflush (stdout);
      
      /* citirea mesajului */
      if (read (client, msg, 100) <= 0)
	{
	  perror ("[server]Eroare la read() de la client.\n");
	  close (client);	/* inchidem conexiunea cu clientul */
	  continue;		/* continuam sa ascultam */
	}
     
	 printf ("[server]Mesajul a fost receptionat...%s\n", msg);
      /*pregatim mesajul de raspuns */
      bzero(msgrasp,100);
      strcat(msgrasp,msg);
	  
	  interest = 2;
	  sum = atoi(msg);
	  multiply = sum * interest;
      printf("[server]Multiplicat:%d\n",multiply);
      sprintf(msgrasp, "%d", multiply);
      /* returnam mesajul clientului */
      if (write (client, msgrasp, 100) <= 0)
	{
	  perror ("[server]Eroare la write() catre client.\n");
	  continue;		/* continuam sa ascultam */
	}
      else
	printf ("[server]Mesajul a fost trasmis cu succes.\n");
    close (client);
    }				/* while */
}				