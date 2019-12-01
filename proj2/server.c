/*
Program: ftserver (server.c)
Description: Spawn a TCP server for a file transfer system
Written by: Alexander Nead-Work (neadwora)
Course: CS 372-400 (Fall 2019, Redfield)
Last Modified: 2019 Nov 10
*/

#include "server.h"

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "[!] usage: %s [port to listen on]\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);

    // spawn main server (for control connection)
    spawn_server(port);

    return 0;
}

void spawn_server(int port) {
    struct sockaddr_in sa_s, sa_c;
    int server_fd, client_fd, ret;
    socklen_t sa_c_s;

    sa_s.sin_family = AF_INET;
    sa_s.sin_addr.s_addr = htonl(INADDR_ANY);
    sa_s.sin_port = htons(port);

    // register socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        fprintf(stderr, "[!] error on socket(): %s", strerror(errno));
        exit(1);
    }

    // bind socket to tcp port
    ret = bind(server_fd, (struct sockaddr *)&sa_s, (socklen_t)sizeof(sa_s));
    if (ret == -1) {
        fprintf(stderr, "[!] error on bind(): %s", strerror(errno));
        exit(1);
    }

    // establish listener
    ret = listen(server_fd, 5);
    if (ret == -1) {
        fprintf(stderr, "[!] error on listen(): %s", strerror(errno));
        exit(1);
    }

    printf("[*] server listening on port %d\n", port);

    // get client connection
    sa_c_s = sizeof(sa_c);
    client_fd = accept(server_fd, (struct sockaddr *)&sa_c, &sa_c_s);

    // multithreaded
    if (fork() == 0) {
        // child
        printf("[*] got control connection from %s on port %d\n", inet_ntoa(sa_c.sin_addr), sa_c.sin_port);
        handle_client(client_fd, sa_c);
    } else {
        // parent
        close(client_fd);
    }
}

void handle_client(int fd, struct sockaddr_in sa) {
    char *buf = malloc(BUF_LEN);

    // client sends port first
    net_getline(fd, buf);

    int data_port = atoi(buf);

    // client then sends command
    net_getline(fd, buf);

    // establish data connection
    int data_fd = spawn_data_connection(sa, data_port);

    if (strncmp(buf, "-l", 2) == 0) {
        // client sent list command
        puts("[*] got list command");
        ls(fd, data_fd);
    } else if (strncmp(buf, "-g", 2) == 0) {
        // client sent get command
        char *filepath = buf+3;
        puts("[*] got get command");
        send_file(fd, data_fd, filepath);
    } else {
        // invalid command
        puts("[*] got invalid command");
        char error[] = "invalid command received\n";
        write(fd, error, strlen(error));
    }

    close(data_fd);
    close(fd);

    free(buf);
}

// write line to buf
// replaces \n with \0
int net_getline(int fd, char *buf) {
    if (!buf) return -1;

    int len = 0;

    bzero(buf, BUF_LEN);
    
    while (len < BUF_LEN) {
        recv(fd, buf+len, 1, 0);

        if (buf[len] == '\n') {
            buf[strcspn(buf, "\n")] = 0;
            return len;
        }

        len++;
    }

    return -1;
}

int spawn_data_connection(struct sockaddr_in sa, int port) {
    sa.sin_port = htons(port);

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

    printf("[*] established data connection to %s:%d\n", inet_ntoa(sa.sin_addr), port);

    return fd;
}

// sends dir contents to data fd
void ls(int control_fd, int data_fd) {
    struct dirent *de;
    char *buf = malloc(BUF_LEN);

    // open directory
    DIR *d = opendir(".");
    if (!d) {
        fputs("failed to open current dir", stderr);
    }

    // send each directory entry to client
    while ((de = readdir(d))) {
        snprintf(buf, 500, "%s\n", de->d_name);

        write(data_fd, buf, strlen(buf));
    }
    write(data_fd, "\0\0", 2);

    free(buf);
}

int open_file(char *path, char **buf) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "[!] failed to open file \"%s\": %s", path, strerror(errno));
        return -1;
    }

    struct stat st;
    fstat(fd, &st);
    int pagesize = getpagesize();
    int size = st.st_size + (pagesize - (st.st_size % pagesize));
    *buf = (char *)mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
    return (int)st.st_size;
}

// send file to data fd
void send_file(int control_fd, int data_fd, char *path) {
    char *contents;
    int len;

    // open file
    len = open_file(path, &contents);
    if (len < 0) {
        char error[] = "failed to open file\n";
        write(control_fd, error, strlen(error));
        return;
    }
    
    // send file len
    char *buf = malloc(BUF_LEN);
    sprintf(buf, "%d\n", len);
    write(data_fd, buf, strlen(buf));

    // send file in 1500 byte chunks
    for (int i = 0; i < len; i+=1500) {
        write(data_fd, buf+i, (len-i < 1500 ? 1500 : len-i));
    }

    // unmap
    int pgsz = getpagesize();
    munmap(buf, len + (pgsz - (len % pgsz)));
}