#ifndef MAIN_H
#define MAIN_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "list.h"

#define BUFSIZE 40
#define WORDMAXNUM 10

#define CMD_NULL    0
#define CMD_ADD     1
#define CMD_DEL     2
#define CMD_SHOW    3
#define CMD_STOP    4

char *p_cmd_add = "add";
char *p_cmd_del = "del";
char *p_cmd_show = "show";
char *p_cmd_stop = "stop";

struct data_node
{
    struct list_head list;
    unsigned int data;
};

int main(int argc, char const *argv[]);


int parse_cmd(char *cmd, int *cmd_type, unsigned int *data);
int is_number(char *str);

int add_data(struct list_head *data_list, unsigned int data);
int del_data(struct list_head *data_list, unsigned int data);
void show_data(struct list_head *data_list);
void clear_data(struct list_head *data_list);


#endif