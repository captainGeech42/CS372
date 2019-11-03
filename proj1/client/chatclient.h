/*
Program: Chat Client (chatclient.h)
Description: Connects to a TCP server for a novel chat system
Written by: Alexander Nead-Work (neadwora)
Course: CS 372-400 (Fall 2019, Redfield)
Last Modified: 2019 Oct 23
*/

#ifndef __CHATCLIENT_H
#define __CHATCLIENT_H

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
#include <netinet/in.h>

#define MAX_UNAME_BUF_LEN 12
#define MAX_BUF_LEN 513

int main(int, char **);
int get_username(char *);
int connect_to_server(char *, int);

#endif
