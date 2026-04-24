#include <stdlib.h>

typedef char* str;


#define linkedlistNodeTypeName(type) LinkedListNode_##type
#define linkedlistTypeName(type) LinkedList_##type

#define UNIQUE(name) name ## __LINE__
#define LinkedList__foreach(type, varname, list) type varname; if (list.first) {varname = list.first->value;}; for (linkedlistNodeTypeName(type)* UNIQUE(currentList) = list.first; UNIQUE(currentList); ({UNIQUE(currentList) = UNIQUE(currentList)->next; varname = (UNIQUE(currentList))? UNIQUE(currentList)->value : varname;}))
#define LinkedList_create {.first = NULL, .length = 0}
#define LinkedList_createNodePtr(type) calloc(1, sizeof(linkedlistNodeTypeName(type)))
#define LinkedList_append(type, list, v) {linkedlistNodeTypeName(type)* UNIQUE(current) = list.first; while (UNIQUE(current) && UNIQUE(current)->next) {printf("here\n"); UNIQUE(current) = UNIQUE(current)->next;}; linkedlistNodeTypeName(type)* UNIQUE(next) = LinkedList_createNodePtr(type); UNIQUE(next)->value = v; if (list.first) {UNIQUE(current)->next = UNIQUE(next);} else {list.first = UNIQUE(next);};}
#define LinkedList_free(type, list) {linkedlistNodeTypeName(type)* UNIQUE(current) = list.first; while (UNIQUE(current)) {linkedlistNodeTypeName(type)* UNIQUE(next) = UNIQUE(current)->next; free(UNIQUE(current)); UNIQUE(current) = UNIQUE(next);}}
#define LinkedList_length(type, list) {linkedlistNodeTypeName(type)* UNIQUE(current) = list.first; int toReturn = 0; }

#define DECLARE_LINKEDLIST_TYPE(type) typedef struct linkedlistNodeTypeName(type) {struct linkedlistNodeTypeName(type)* next; type value;} linkedlistNodeTypeName(type); typedef struct {linkedlistNodeTypeName(type)* first; int length;} linkedlistTypeName(type)


DECLARE_LINKEDLIST_TYPE(int);
DECLARE_LINKEDLIST_TYPE(str);


