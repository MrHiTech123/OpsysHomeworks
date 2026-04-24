#include <stdlib.h>

typedef char* str;


#define linkedlistNodeTypeName(type) LinkedListNode_##type
#define linkedlistTypeName(type) LinkedList_##type

#define UNIQUE(name) name ## __LINE__
#define LinkedList__foreach(type, varname, list) type varname = list->value; for (linkedlistTypeName(type) UNIQUE(currentList) = list; UNIQUE(currentList); ({UNIQUE(currentList) = UNIQUE(currentList)->next; varname = (UNIQUE(currentList))? UNIQUE(currentList)->value : varname;}))
#define LinkedList_create NULL
#define LinkedList_createNodePtr(type) calloc(1, sizeof(linkedlistNodeTypeName(type)))
#define LinkedList_append(type, list, v) if (list) {linkedlistTypeName(type) UNIQUE(current) = list; while (UNIQUE(current)->next) {UNIQUE(current) = UNIQUE(current)->next;}; linkedlistTypeName(type) next = LinkedList_createNodePtr(type); next->value = v; UNIQUE(current)->next = next;} else {list = LinkedList_createNodePtr(type); list->value = v;}
#define LinkedList_free(type, list) {linkedlistTypeName(type) UNIQUE(current) = list; while (UNIQUE(current)) {linkedlistTypeName(type) next = UNIQUE(current)->next; free(UNIQUE(current)); UNIQUE(current) = next;}}
#define LinkedList_length(type, list) {linkedlistTypeName(type) UNIQUE(current) = list; int toReturn = 0; }

#define DECLARE_LINKEDLIST_TYPE(type) typedef struct linkedlistNodeTypeName(type) {struct linkedlistNodeTypeName(type)* next; type value;} linkedlistNodeTypeName(type); typedef linkedlistNodeTypeName(type)* linkedlistTypeName(type)


DECLARE_LINKEDLIST_TYPE(int);
DECLARE_LINKEDLIST_TYPE(str);


