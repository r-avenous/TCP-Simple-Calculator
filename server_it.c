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
#define MAXLINE 10

typedef struct _dynamic_string
{
    char *str;
    int size;
}dynstr;
// dynamic string code
void append(dynstr* ds, char* next)
{
    if(ds->str == NULL)
    {
        ds->str = (char *) malloc(sizeof(char) * (strlen(next) + 1));
        strcpy(ds->str, next);
        ds->size = strlen(next) + 1;
    }
    else
    {
        ds->str = (char *) realloc(ds->str, sizeof(char) * (ds->size + strlen(next) + 1));
        strcat(ds->str, next);
        ds->size += strlen(next) + 1;
    }
}
// stacks for operators and operands
typedef struct _stack_float
{
    int size;
    float *arr;
}stack_float;
typedef struct _stack_char
{
    char *arr;
    int size;
}stack_char;
void pushc(stack_char* st, char val)
{
    st->arr[st->size] = val;
    st->size++;
}
void pushf(stack_float* st, float val)
{
    st->arr[st->size] = val;
    st->size++;
}
char popc(stack_char *st)
{
    st->size--;
    return st->arr[st->size];
}
float popf(stack_float *st)
{
    st->size--;
    return st->arr[st->size];
}
// function to evaluate the expression; assumed to be valid; all assumptions taken from the question
float evaluateExpression(char* exp)
{
    char* string = (char*)malloc(3*strlen(exp));
    stack_char operators;
    operators.arr = (char*)malloc(strlen(exp));
    operators.size = 0;
    stack_float operands;
    operands.arr = (float*)malloc(strlen(exp)*sizeof(float));
    operands.size = 0;
    int index=0;
    for(int i=0; exp[i] != '\0'; i++)
    {
        if(exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/' || exp[i] == '(' || exp[i] == ')')
        {
            string[index++] = ' ';
            string[index++] = exp[i];
            string[index++] = ' ';
        }
        else
            string[index++] = exp[i];
    }
    char* token = strtok(string, " ");
    while(token != NULL) 
    {
        // printf("%s\n", token);
        if(token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/')
        {
            pushc(&operators, token[0]);
        }
        else if(token[0] == '(')
        {
            pushc(&operators, token[0]);
        }
        else if(token[0] == ')')
        {
            popc(&operators);
            if(operators.size > 0)
            {
                float op2 = popf(&operands);
                float op1 = popf(&operands);
                char op = popc(&operators);
                if(op == '+')
                    pushf(&operands, op1+op2);
                else if(op == '-')
                    pushf(&operands, op1-op2);
                else if(op == '*')
                    pushf(&operands, op1*op2);
                else if(op == '/')
                    pushf(&operands, op1/op2);
            }
        }
        else
        {
            float val;
            sscanf(token, "%f", &val);
            pushf(&operands, val);
            if(operators.size > 0 && operators.arr[operators.size-1] != '(' && operators.arr[operators.size-1] != ')')
            {
                float op2 = popf(&operands);
                float op1 = popf(&operands);
                char op = popc(&operators);
                if(op == '+')
                    pushf(&operands, op1+op2);
                else if(op == '-')
                    pushf(&operands, op1-op2);
                else if(op == '*')
                    pushf(&operands, op1*op2);
                else if(op == '/')
                    pushf(&operands, op1/op2);
            }
        }
        token = strtok(NULL, " ");
    }
    return operands.arr[0];
}

int main()
{
    // AF_INET = Address Family Internet
    // SOCK_STREAM = Socket Type Stream
    // 0 -> TCP
    int sockfd = socket(AF_INET, SOCK_STREAM, 0), newsockfd;
    // server address, client address
    struct sockaddr_in serv_addr, cli_addr;
    // socket() returns a file descriptor, if = -1 it is an error in connecting
    if(sockfd < 0)
    {
        printf("Error creating socket");
        return 0;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    // host to network short; converts to network standard 
    serv_addr.sin_port = htons(PORT);
    // bind() assigns the address specified by serv_addr to the socket referred to by the file descriptor sockfd
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Error binding socket");
        return 0;
    }
    // listen() marks the socket referred to by sockfd as a passive socket, that is, as a socket that will be used to accept incoming connection requests using accept()
    listen(sockfd, 10);
    int cli_len = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
    if(newsockfd < 0)
    {
        printf("Error accepting connection");
        return 0;
    }
    while(1)
    {
        // accept() extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket
        
        char buf[MAXLINE+1];
        buf[MAXLINE] = '\0';
        dynstr expression;
        expression.str = NULL;
        expression.size = 0;
        int recieved = 0;
        while(!recieved)
        {
            recv(newsockfd, buf, MAXLINE, 0);
            for(int i=0; i<MAXLINE; i++) if(buf[i] == 0) recieved = 1;
            append(&expression, buf);
        }
        if(strcmp(expression.str, "-1\n") == 0)
        {
            printf("Ending connection with current client..\n");
            close(newsockfd);
            cli_len = sizeof(cli_addr);
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
            if(newsockfd < 0)
            {
                printf("Error accepting connection");
                break;
            }
            continue;
        }
        printf("Received Expression: %s\n", expression.str);
        sprintf(buf, "%.2f", evaluateExpression(expression.str));
        printf("Sending Result: %s\n", buf);
        send(newsockfd, buf, MAXLINE, 0);
    }
    return 0;
}