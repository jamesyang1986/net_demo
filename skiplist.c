//
// Created by wan on 24-1-5.
//
#include "skiplist.h"
#include <stdlib.h>
#include <string.h>

skip_list *init() {
    skip_list *list = (skip_list *) malloc(sizeof(struct skip_list));
    list->head = NULL;
    list->tail = NULL;
    list->level = 0;
    return list;
}

void insert_val(skip_list *list, char *key, char *value) {
    skip_list_node *cur = list->head;
    skip_list_node *[MAX_SKIPLIST_LEVEL] prev;
    int level = list->level;

    while (int j = level - 1, j >= 0;){
        prev[j] = cur;
        if (cur && cur->forward[j] && strcmp(cur->forward[j]->key, key) < 0) {
            cur = cur->forward[j];
        } else {
            j--;
        }
    }

    skip_list_node *node = malloc(sizeof(struct skip_list_node) + MAX_SKIPLIST_LEVEL * sizeof( struct skip_list_node));
    node->key = key;
    node->value = value;
    node->forward = (skip_list_node**) malloc(MAX_SKIPLIST_LEVEL * sizeof (struct skip_list_node * ));

}

char *find_val(skip_list *list, char *key) {
    if (list == NULL || list->head == nuLL)
        return NULL;

    skip_list_node *cur = list->head;
    int level = list->level;
    while (int j = level - 1, j >= 0;){
        if (cur && cur->forward[j] && strcmp(cur->forward[j]->key, key) < 0) {
            cur = cur->forward[j];
        } else {
            j--;
        }

    }

    if (cur != NULL && list->level >= 1) {
        return cur->forward[0]->value;
    }

    return NULL;
}

void delete_val(skip_list *list, char *key) {


}