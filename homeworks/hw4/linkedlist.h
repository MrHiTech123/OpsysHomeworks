#include <stdlib.h>

typedef char* str;


#define linkedlistNodeTypeName(type) LinkedListNode_##type
#define linkedlistTypeName(type) LinkedList_##type

#define UNIQUE_NAME l ## __LINE__
#define LinkedList__foreach(type, varname, list) type varname = list->value; for (linkedlistTypeName(type) UNIQUE_NAME = list; UNIQUE_NAME; ({UNIQUE_NAME = UNIQUE_NAME->next; varname = (UNIQUE_NAME)? UNIQUE_NAME->value : varname;}))
#define LinkedList_create(type) calloc(1, sizeof(linkedlistNodeTypeName(type)))
#define LinkedList_append(type, list, v) {linkedlistTypeName(type) current##__LINE__ = list; while (current##__LINE__->next) {current##__LINE__ = current##__LINE__->next;}; linkedlistTypeName(type) next = LinkedList_create(type); next->value = v; current##__LINE__->next = next;}
#define LinkedList_free(type, list) {linkedlistTypeName(type) UNIQUE_NAME = list; while (UNIQUE_NAME) {linkedlistTypeName(type) next = UNIQUE_NAME->next; free(UNIQUE_NAME); UNIQUE_NAME = next;}}


#define DECLARE_LINKEDLIST_TYPE(type) typedef struct linkedlistNodeTypeName(type) {struct linkedlistNodeTypeName(type)* next; type value;} linkedlistNodeTypeName(type); typedef linkedlistNodeTypeName(type)* linkedlistTypeName(type)


DECLARE_LINKEDLIST_TYPE(int);
DECLARE_LINKEDLIST_TYPE(str);


