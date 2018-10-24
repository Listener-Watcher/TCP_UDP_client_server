/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h> // atoi
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t cli_len;
     char buffer[256];
     char sent_msg[256];
     struct sockaddr_in serv_addr, cli_addr;
     int status;
     if (argc < 2||argc>=3) {
         error("ERROR, no port provided\n");
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]); // convert string to integer, get port number
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     cli_len = sizeof(cli_addr);

     while(newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &cli_len))
     {
     	status = read(newsockfd,buffer,255);
     	if (status < 0) error("ERROR reading from socket");
	char sent_msg[256];
	int invalid = 0;
	for(int i=0;i<strlen(buffer)-1;i++)
	{
		int temp = buffer[i];
		if(temp<48||temp>57)
		{
			invalid = 1;
			strcpy(sent_msg, "Sorry,cannot compute!\n");
		}
	}
	if(invalid)
	{
     		status = write(newsockfd,sent_msg,strlen(sent_msg));
		if(status<0) error("ERROR writing to socket");
	}
	else
	{
		bzero(sent_msg,256);
		int j=0;
			int sum = 0;
			for(int i=0;i<strlen(buffer)-1;i++)
			{
				sum+=buffer[i]-48;
			}
			sent_msg[j] = sum;
			int count=0;
			bzero(buffer,256);
			while(sum>=10)
			{
				buffer[count] = sum%10;
				sum = sum/10;
				count++;
			}
			buffer[count] = sum;
		while(sent_msg[j]>=10)
		{
			j++;
			int sum = 0;
			for(int i=0;i<count+1;i++)
			{
				sum+=buffer[i];
			}
			sent_msg[j] = sum;
			int count=0;
			bzero(buffer,256);
			while(sum>=10)
			{
				buffer[count] = sum%10;
				sum = sum/10;
				count++;
			}
			buffer[count] = sum;
		}
		status = write(newsockfd,sent_msg,strlen(sent_msg));
		if(status<0) error("ERROR writing to socket");
		close(newsockfd);
		bzero(buffer,256);
     	//	close(sockfd);
	}
     }
     error("ERROR on accept");
     close(newsockfd);
     close(sockfd);
     return 0; 
}
