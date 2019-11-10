/*
Program: Chat Client (chatclient.c)
Description: Connects to a TCP server for a novel chat system
Written by: Alexander Nead-Work (neadwora)
Course: CS 372-400 (Fall 2019, Redfield)
Last Modified: 2019 Oct 23
*/

#include "chatclient.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "[!] usage: %s [host] [port]\n", argv[0]);
        return 1;
    }

    char username[MAX_UNAME_BUF_LEN] = {0};
    get_username(username);
    printf("[*] welcome %s\n", username);

    int port = atoi(argv[2]);

    int socket = connect_to_server(argv[1], port);

    char buf[MAX_BUF_LEN];
    char input[500];
    int ret;
    while (1) {
        bzero(buf, MAX_BUF_LEN);
        bzero(input, 500);

        // get message
        printf("%s> ", username);
        fgets(input, 500, stdin);
        input[strcspn(input, "\n")] = 0;
        snprintf(buf, MAX_BUF_LEN, "%s> %s", username, input);

        // send message
        ret = write(socket, buf, MAX_BUF_LEN);
        if (ret < 0) {
            fprintf(stderr, "[!] error writing to socket: %s\n", strerror(errno));
        } else if (ret == 0) {
            puts("[*] socket closed by server");
            return 0;
        }

        // check if quit
        if (strcmp(input, "\\quit") == 0) {
            puts("[*] goodbye!");
            close(socket);
            return 0;
        }

        // recv message
        if (read(socket, buf, MAX_BUF_LEN) < 0) {
            fprintf(stderr, "[!] error reading from socket: %s\n", strerror(errno));
        }

        // check if server quit
        if (strstr(buf, "\\quit")) {
            puts("[*] server closed connection, goodbye!");
            close(socket);
            return 0;
        }

        // print
        printf("%s\n", buf);

        // repeat
    }
}

// get_username
// Arguments: username
// Returns: length of username
// Pre-cond: none
// Post-cond: Username stored in $username
int get_username(char *username) {
    printf("Username (max len 10 char): ");
    fgets(username, MAX_UNAME_BUF_LEN, stdin);
    username[10] = 0;
    username[strcspn(username, "\n")] = 0;

    return strlen(username);
}

// connect_to_server
// Arguments: host, port
// Returns: socket fd
// Pre-cond: none
// Post-cond: connected to server
int connect_to_server(char *host, int port) {
    struct sockaddr_in sa;

    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);

    // if we don't have an IP, need to get one
    int is_ip = inet_pton(AF_INET, host, &(sa.sin_addr));
    if (!is_ip) {
        // do dns
        struct hostent *he;
        he = gethostbyname(host);
        sa.sin_addr = *((struct in_addr *)he->h_addr);
    }

    // setup socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        fprintf(stderr, "[!] error on socket: %s\n", strerror(errno));
        return 1;
    }

    // connect to server
    if (connect(fd, (struct sockaddr *)&sa, (socklen_t)sizeof(sa)) < 0) {
        fprintf(stderr, "[!] error on connect: %s\n", strerror(errno));
        return 1;
    }

    printf("[*] connected to %s:%d\n", host, port);

    return fd;
}
