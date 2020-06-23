#include"lib/common.h"

#define MAX_LINE   2048

void main(int argc, char **argv)
{
    int ret = 0;

    if (argc != 2)
    {
        error(1, 0, "usage:wrong input param num %d, %s,%s", argc, argv[1], argv[2]);
    }

    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    socklen_t server_len = sizeof(server_addr);
    ret = connect(socket_fd, &server_addr, server_len);
    if (ret < 0){
        error(1,errno, "connect refuse!");
    }

    char send_line[1000],recv_line[10000];
    int n;
    fd_set allreads;
    fd_set readmask;

    FD_ZERO(&allreads);
    FD_SET(0, &allreads);//标准输入
    FD_SET(socket_fd, &allreads);


    while(1){
        readmask = allreads;
        int rc = select(socket_fd + 1, &readmask, NULL, NULL, NULL);
        if (rc < 0){
            error(1, errno, "select error");
        }
        if (FD_ISSET(socket_fd, &readmask)){
            n = read(socket_fd, 1000, MAX_LINE);
            if (n < 0){
                error(0, errno, "Failed to read");
            }else if (n == 0){
                error(0, errno, "server termina\n");
            }
            recv_line[n] = 0;
            fputs(recv_line, stdout);
            fputs("\n", stdout);
        }
        if (FD_ISSET(0, &readmask)){
            if (fgets(send_line, MAX_LINE, stdin) != NULL){
                printf("%s", send_line);

                size_t rt = write(socket_fd, send_line, strlen(send_line));
                if (rt < 0){
                    error(0,errno, "write failed");
            }
        }
    }
    }
}