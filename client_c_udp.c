/*original source:http://www.linuxhowtos.org/data/6/client_udp.c* 
 The source is obtained by the slide provided on the section.
 The copied part contains including part, void error function and from line 19 to 47.
/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void error(const char *);//copied,imlemented in the end
int main(int argc, char *argv[])
{
// copied from line 19 to 47
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[1024];
   
   if (argc != 3) { printf("Usage: server port\n");
                    exit(1);
   }
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);
   printf("Enter string: ");
   bzero(buffer,1024);
   fgets(buffer,1023,stdin);
   n=sendto(sock,buffer,
            strlen(buffer),0,(const struct sockaddr *)&server,length);
   if (n < 0) error("Sendto");
   n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
   if (n < 0) error("recvfrom");
   if(buffer[0]=='S')
   {
	   printf("From server: %s",buffer);
   }
   else if(buffer[0]=='\0')
   {
	   printf("From server: 0\n");
   }
   else
   {
	   buffer[n]='\0';
	   for(int i=0;i<strlen(buffer);i++)
	   {
		   printf("From server: %d\n",buffer[i]);
	   }
   }
   close(sock);
   return 0;
}
//error function implemented.copied
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
