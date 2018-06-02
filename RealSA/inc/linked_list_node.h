/*
 * linked_list_node.h: file to define the linked list node structure
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LINKED_LIST_NODE_H
#define LINKED_LIST_NODE_H

typedef struct linked_list_node_tag
{
    void *element;
    struct linked_list_node_tag *next;
    struct linked_list_node_tag *prev;
}linked_list_node;

#endif

#ifdef __cplusplus
}
#endif