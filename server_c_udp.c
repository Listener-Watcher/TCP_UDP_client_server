/* Creates a datagram server.  The port 
   number is passed as an argument.  This
   server runs forever */

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
   int sock, length, status;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[1024];

   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }
   
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   while (1) 
   {
       status = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
       if (status < 0) error("recvfrom");
	char sent_msg[1024];
	int invalid = 0;
	for(int i=0;i<strlen(buf)-1;i++)
	{
		int temp=buf[i];
		if(temp<48||temp>57)
		{
			invalid = 1;
			strcpy(sent_msg,"Sorry,cannot compute!\n");
		}
	}
	if(invalid)
	{
		status = sendto(sock,sent_msg,strlen(sent_msg),
                  0,(struct sockaddr *)&from,fromlen);
       		if (status < 0) error("sendto");
	}
	else
	{
		bzero(sent_msg,1024);
		int j=0;
		int sum=0;
		for(int i=0;i<strlen(buf)-1;i++)
		{
			sum+=buf[i]-48;
		}
		sent_msg[j]=sum;
		int count=0;
		bzero(buf,1024);
		while(sum>=10)
		{
			buf[count]=sum%10;
			sum = sum/10;
			count++;
		}
		buf[count]=sum;
		while(sent_msg[j]>=10)
		{
			j++;
			int sum=0;
			for(int i=0;i<count+1;i++)
			{
				sum+=buf[i];
			}
			sent_msg[j] = sum;
			int count = 0;
			bzero(buf,1024);
			while(sum>=10)
			{
				buf[count]=sum%10;
				sum=sum/10;
				count++;
			}
			buf[count]=sum;	
		}
		status = sendto(sock,sent_msg,strlen(sent_msg),
                  0,(struct sockaddr *)&from,fromlen);
       		if (status < 0) error("sendto");
		bzero(buf,256);
	}
   }
   close(sock);
   return 0;
 }
