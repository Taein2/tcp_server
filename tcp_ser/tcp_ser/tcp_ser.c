#include "inet.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int sockfd, newsockfd, aa;
    struct sockaddr_in  cli_addr, serv_addr;
    pname = argv[0];
    pid_t pid;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Server : can¡¯t open stream socket\n"); return(0);
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_TCP_PORT);
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Server : can¡¯t bind local address \n"); return(0);
    }
    listen(sockfd, 5);
    aa = sizeof(cli_addr);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &aa);
        pid = fork();
        if (pid == -1) { close(newsockfd); continue; }
        else if (pid > 0) {
            close(newsockfd); continue;
        }
        else {
            while (1) {
                char buff[20] = {};
                char left[10] = {};
                char right[10] = {};
                int result = 0, loc = 0, op = 0;
                if (newsockfd < 0) {
                    printf("Server : accept error  \n"); return(0);
                }

                if (read(newsockfd, &buff, sizeof(buff)) <= 0) {
                    printf("Server : readn error or client connection is closed  \n");  return(0);
                }
                printf("Server : received string : %s\n", buff);
                for (int i = 0; i < sizeof(buff); i++) {
                    if (buff[i] == '+') {
                        op = buff[i];
                        loc = i;
                    }
                }
                int length = strlen(buff);
                for (int i = 0; i < loc; i++) {
                    left[i] = buff[i];
                }
                for (int i = loc + 1; i < length; i++) {
                    if (buff[i] != ' ') {
                        right[i - loc - 1] = buff[i];
                    }
                }
                int r = 0, l = 0;
                for (int i = 0; i < loc; i++) {

                    l = (l * 10 + left[i] - '0');
                }
                for (int i = loc + 1; i < length; i++) {
                    r = (r * 10 + right[i - loc - 1] - '0');
                }
                result = l + r;
                char result_toArr[20] = {};
                sprintf(result_toArr, "%d", result);
                if (write(newsockfd, &result_toArr, sizeof(result_toArr)) <= 0) {
                    printf("Server : written error \n"); return(0);
                }
            }
            close(newsockfd);
        }
    }
    close(sockfd);

}
