/*
Program: ftserver (server.h)
Description: Spawn a TCP server for a file transfer system
Written by: Alexander Nead-Work (neadwora)
Course: CS 372-400 (Fall 2019, Redfield)
Last Modified: 2019 Nov 10
*/

#ifndef __SERVER_H
#define __SERVER_H

#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <netinet/in.h>

#define BUF_LEN 500

int main(int, char **);

void spawn_server(int);
void handle_client(int, struct sockaddr_in);
int spawn_data_connection(struct sockaddr_in, int);
int net_getline(int, char *);

void ls(int, int);
int open_file(char *, char **);
void send_file(int, int, char *);

#endif