#include"lib/common.h"

void main(int argc, char **argv){
    int listen_fd;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(SERV_PORT);

    int ret = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0){
        error(0, errno, "bind error");
    }

    int ret1 = listen(listen_fd, LISTENQ);
    if (ret1 < 0){
        error(1, errno, "listen error");
    }

    signal(SIGINT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    int connfd;
    struct sockaddr_in client_addr;
    socklen_t client_len =sizeof(client_addr);

    if ((connfd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len)) < 0){
        error(1, errno, "accept error");
    }

    char message[2048];
    char send_line[2048];

    while (1){
        int n = read(connfd, message, 2048);
        if (n < 0){
            error(1, errno, "read error");
        }else if (n == 0){
            error(1, errno, "client close");
        }
        message[n] = 0;

        char *token = strtok(message, " ");

        if(strcmp(token, "pwd") == 0){
            getcwd(send_line, 2048);
        }else if(strcmp(token, "cd") == 0){
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                chdir(token);
                printf("%s", token);
            }
        }

        int write_nc = send(connfd, send_line, strlen(send_line), 0);
        if (write_nc < 0){
            error(1, errno, "error write");
        }
    }
}