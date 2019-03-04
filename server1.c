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
  struct sockaddr_in server;	
  struct sockaddr_in from;	
  char msg[100];		
  char msgrasp[100]=" ";        
  int sd;			//descriptorul de socket
  int multiply, interest, sum;  

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
    server.sin_family = AF_INET;	
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);
  
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  if (listen (sd, 5) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }

  while (1)
    {
      int client;
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      client = accept (sd, (struct sockaddr *) &from, &length);
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
	  close (client);
	  continue;		
	}
	  printf ("[server]Mesajul a fost receptionat: %s\n", msg);/*pregatim mesajul de raspuns */  
	  if(strcmp(msg, "Premium") == 0){
			printf("Premium \n");
		}else{
			printf("Client: %s\n", msg);
			send(client, msg, strlen(msg), 0);
		}


		  if (write (client, msg, 100) <= 0)
		{
		  perror ("[server]Eroare la write() catre client.\n");
		  continue;
		}
    }				/* while */
}				