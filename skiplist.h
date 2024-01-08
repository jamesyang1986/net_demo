//
// Created by wan on 24-1-5.
//

#ifndef NET_DEMO_SKIPLIST_H
#define NET_DEMO_SKIPLIST_H

#endif //NET_DEMO_SKIPLIST_H

#define MAX_SKIPLIST_LEVEL 6

typedef struct skip_list_node {
    char *key;
    char *value;
    struct skip_list_node* [MAX_SKIPLIST_LEVEL] forward;
} skip_list_node;

typedef struct skip_list {
    struct skip_list_node *head, *tail;
    int level;
} skip_list;

skip_list* init();

void insert_val(skip_list *list, char *key, char *value);

char* find_val(skip_list *list, char *key);

void delete_val(skip_list *list, char *key);