// 20CS10012
// Atulya Sharma
// Assignment 1 Q 2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 20000
#define LOCAL "127.0.0.1"
#define MAXLINE 10

int main()
{
    // AF_INET = Address Family Internet
    // SOCK_STREAM = Socket Type Stream
    // 0 -> TCP
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    if(sockfd < 0)
    {
        printf("Error creating socket");
        return 0;
    }
    serv_addr.sin_family = AF_INET;
    inet_aton(LOCAL, &serv_addr.sin_addr);
    serv_addr.sin_port = htons(PORT);
    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Error connecting to server");
        return 0;
    }
    while(1)
    {
        char buf[MAXLINE+1], *expression = NULL;
        buf[MAXLINE] = '\0';
        int size = 0;
        getline(&expression, &size, stdin);
        
        int index = 0, len = strlen(expression);
        while(index <= len)
        {
            if(index == len) 
            {
                buf[0] = 0;
                index++;
            }
            else
            {
                int i;
                for(i=0; i<MAXLINE && index<len; i++, index++)
                    buf[i] = expression[index];
                if(i < MAXLINE) 
                {
                    buf[i] = 0;
                    index++;
                }
            }
            send(sockfd, buf, MAXLINE, 0);
        }
        if(strcmp(expression, "-1\n") == 0)
        {
            printf("Exiting..\n");
            break;
        }
        // send(sockfd, expression, len+1, 0);
        recv(sockfd, buf, MAXLINE, 0);
        printf("Answer: %s\n", buf);
        
    }
    close(sockfd);
    return 0;
}