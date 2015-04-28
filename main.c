
#include "main.h"

int main(int argc, char const *argv[])
{
    /* code */
    char input_buf[BUFSIZE] = {0};
    char *p_input_buf = NULL;
    char *p = NULL;
    int cmd_type = CMD_NULL;
    unsigned int data = 0;
    int stop_flag = 0;

    LIST_HEAD(data_list);

    while (!stop_flag) {
        p_input_buf = fgets(input_buf, BUFSIZE, stdin);
        if (p_input_buf == NULL){
            printf("Error @ Get input\n");
            continue;
        }

        if (*(p_input_buf + BUFSIZE - 1) != 0) {
            printf("Error @ Get input: too many chars \n");
            continue;
        }
        p = strchr(p_input_buf, '\n');
        *p = '\0';

        if (parse_cmd(p_input_buf, &cmd_type, &data) != 0) {
            printf("Error @ Command parsing.\n");
            continue;
        }

        int ret;
        switch (cmd_type) {
            case CMD_ADD:
            if ((ret = add_data(&data_list, data) != 0) != 0) {
                if (ret == -1)
                    printf("Error @ Add data: malloc error.\n");
            }
            break;
            case CMD_DEL:
            if ((ret = del_data(&data_list, data)) != 0) {
                if (ret == -1)
                    printf("Error @ Delete data: malloc error.\n");
                if (ret == -2)
                    printf("Error @ Delete data: no data %d in list.\n", data);
            }
            break;
            case CMD_SHOW:
            show_data(&data_list);
            break;
            case CMD_STOP:
            clear_data(&data_list);
            stop_flag = 1;
            break;
            default:
            break;
        }
    }
    return 0;
}

/**
 * parsing the command input from console
 * @param  cmd      : the input command string that need to be parsed.
 * @param  cmd_type : output, indicating the command type of the input command string
 *                    see the defination of CMD_* in main.h
 * @param  data     : when a command need a operand(such as add/del), data return the
 *                    value of the operand. default 0.
 * @return          : 0 - success
 *                    1 - parsing failed
 */
int parse_cmd(char *cmd, int *cmd_type, unsigned int *data)
{
    int word_num = 0;
    char *word_arr[WORDMAXNUM] = {NULL};
    char *p;
    while ((p = strsep(&cmd, " ")) != NULL) {
        if (*p != '\0')
            word_arr[word_num++] = p;
        if (word_num >= WORDMAXNUM)
            return -1;
    }
    if (word_num == 0)
        return -1;

    if (!strcmp(word_arr[0], p_cmd_add)) {
        if (word_num != 2)
            return -1;
        if (!is_number(word_arr[1]))
            return -1;
        long int temp;
        if ((temp = strtol(word_arr[1], NULL, 10)) >= UINT_MAX)
            return -1;
        *data = (unsigned int) temp;
        *cmd_type = CMD_ADD;
        return 0;
    } else if (!strcmp(word_arr[0], p_cmd_del)) {
        if (word_num != 2)
            return -1;
        if (!is_number(word_arr[1]))
            return -1;
        long int temp;
        if ((temp = strtol(word_arr[1], NULL, 10)) >= UINT_MAX)
            return -1;
        *data = (unsigned int) temp;
        *cmd_type = CMD_DEL;
        return 0;
    } else if (!strcmp(word_arr[0], p_cmd_show)) {
        if (word_num != 1)
            return -1;
        *cmd_type = CMD_SHOW;
        *data = 0;
        return 0;
    } else if (!strcmp(word_arr[0], p_cmd_stop)) {
        if (word_num != 1)
            return -1;
        *cmd_type = CMD_STOP;
        *data = 0;
        return 0;
    } else {
        *cmd_type = CMD_NULL;
        *data = 0;
        return -1;
    }
}

/**
 * determine whether a string is a number string (containing only 0-9)
 * @param  str : the input string
 * @return     : 0 - the string isn't a number string
 *               1 - the string is a number string
 */
int is_number(char *str)
{
    char *p = str;
    while (*p != '\0') {
        if (*p > '9' || *(p++) < '0')
            return 0;
    }
    return 1;
}

/**
 * add a data to the specified list
 * @param  data_list : the head of the specified list
 * @param  data      : the data needed to be added
 * @return           :  0 - operated successfully;
 *                     -1 - error when malloc()
 */
int add_data(struct list_head *data_list, unsigned int data)
{
    struct data_node *node;
    if (list_empty(data_list)) {
        if ((node = (struct data_node *)malloc(sizeof(struct data_node))) == NULL)
            return -1;
        node->data = data;
        list_add(&node->list, data_list);
    } else {
        struct list_head *p;
        struct data_node *entry;
        list_for_each(p, data_list) {
            entry = list_entry(p, struct data_node, list);
            if (entry->data == data)
                return 0;
            if (entry->data > data) {
                if ((node = (struct data_node *)malloc(sizeof(struct data_node))) == NULL)
                    return -1;
                node->data = data;
                list_add_tail(&node->list, p);
                return 0;
            }
        }
        if ((node = (struct data_node *)malloc(sizeof(struct data_node))) == NULL)
            return -1;
        node->data = data;
        list_add_tail(&node->list, data_list);
    }
    return 0;
}

/**
 * delete a data node from the input list
 * @param  data_list : the input data list.
 * @param  data      [the data need to be deleted]
 * @return           [return 0 when operate seccess.]
 */
/**
 * delete a data node from the specified list
 * @param  data_list : the head of the specified list
 * @param  data      : the data need to be deleted.
 * @return           :  0 - operated successfully;
 *                     -1 - error when malloc()
 *                     -2 - there is no such data in the list
 */
int del_data(struct list_head *data_list, unsigned int data)
{
    struct data_node *node;
    if ((node = (struct data_node *)malloc(sizeof(struct data_node))) == NULL)
        return -1;
    node->data = data;

    if (list_empty(data_list)) {
        return -2;
    } else {
        struct list_head *p;
        struct data_node *entry;
        list_for_each(p, data_list) {
            entry = list_entry(p, struct data_node, list);
            if (entry->data == data) {
                list_del(&entry->list);
                free(entry);
                return 0;
            }
        }
        return -2;
    }
    return 0;
}

/**
 * show all the data from the specified list
 * @param  data_list : the specified list.
 */
void show_data(struct list_head *data_list)
{
    struct list_head *p;
    struct data_node *entry;
    list_for_each(p, data_list) {
        entry = list_entry(p, struct data_node, list);
        printf("%d\n", entry->data);
    }
}

/**
 * clear the specified list
 * @param data_list : list input
 */
void clear_data(struct list_head *data_list)
{
    struct list_head *p, *n;
    struct data_node *entry;
    list_for_each_safe(p, n, data_list) {
        entry = list_entry(p, struct data_node, list);
        list_del(&entry->list);
        free(entry);
    }
}
