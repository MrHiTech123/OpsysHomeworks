#ifndef _LINKEDLIST
#define _LINKEDLIST

#include <stdlib.h>
#include <stdbool.h>
#include "mystring.h"

#define LINKEDLIST_NODETYPE_NAME(type) LinkedListNode_##type
#define LINKEDLIST_TYPE_NAME(type) LinkedList_##type

#define MACRO_VAR(name) macroVar_ ## name ## __LINE__
#define LinkedList__foreach(type, varname, list) type varname; if (list.first) {varname = list.first->value;}; for (LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(currentList) = list.first; MACRO_VAR(currentList); ({MACRO_VAR(currentList) = MACRO_VAR(currentList)->next; if (MACRO_VAR(currentList)) {varname = MACRO_VAR(currentList)->value;}}))
#define LinkedList__create {.first = NULL}
#define LinkedList__createNodePtr(type, v) ({LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(toReturn) = (LINKEDLIST_NODETYPE_NAME(type)*)calloc(1, sizeof(LINKEDLIST_NODETYPE_NAME(type))); MACRO_VAR(toReturn)->value = v; MACRO_VAR(toReturn);})
#define LinkedList__append(type, list, v) {LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(current) = list.first; while (MACRO_VAR(current) && MACRO_VAR(current)->next) {MACRO_VAR(current) = MACRO_VAR(current)->next;}; LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(next) = LinkedList__createNodePtr(type, v); if (list.first) {MACRO_VAR(current)->next = MACRO_VAR(next);} else {list.first = MACRO_VAR(next);};}
#define __LINKED_LIST_FREE(type, list, freeFunction) {LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(current) = list.first; while (MACRO_VAR(current)) {LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(next) = MACRO_VAR(current)->next; freeFunction(MACRO_VAR(current)); MACRO_VAR(current) = MACRO_VAR(next);}}
#define LinkedList__length(type, list) ({int MACRO_VAR(toReturn) = 0; LinkedList__foreach(type, MACRO_VAR(unused), list) {MACRO_VAR(unused) = MACRO_VAR(unused); ++MACRO_VAR(toReturn);}; MACRO_VAR(toReturn);})

#define LinkedList__free(type, list) __LINKED_LIST_FREE(type, list, free)
#define LinkedList__freeFree(type, list) __LINKED_LIST_FREE(type, list, LinkedList__free)
#define LinkedList__freeFreeFree(type, list) __LINKED_LIST_FREE(type, list LinkedList__freeFre ## e)

#define DECLARE_LINKEDLIST_TYPE(type) typedef struct LINKEDLIST_NODETYPE_NAME(type) {struct LINKEDLIST_NODETYPE_NAME(type)* next; type value;} LINKEDLIST_NODETYPE_NAME(type); typedef struct {LINKEDLIST_NODETYPE_NAME(type)* first;} LINKEDLIST_TYPE_NAME(type)

#endif