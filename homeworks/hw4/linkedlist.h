#include <stdlib.h>

typedef char* str;


#define linkedlistNodeTypeName(type) LinkedListNode_##type
#define linkedlistTypeName(type) LinkedList_##type

#define MACRO_VAR(name) macroVar_ ## name ## __LINE__
#define LinkedList__foreach(type, varname, list) type varname; if (list.first) {varname = list.first->value;}; for (linkedlistNodeTypeName(type)* MACRO_VAR(currentList) = list.first; MACRO_VAR(currentList); ({MACRO_VAR(currentList) = MACRO_VAR(currentList)->next; varname = (MACRO_VAR(currentList))? MACRO_VAR(currentList)->value : varname;}))
#define LinkedList__create {.first = NULL, .length = 0}
#define LinkedList__createNodePtr(type, v) ({linkedlistNodeTypeName(type)* MACRO_VAR(toReturn) = calloc(1, sizeof(linkedlistNodeTypeName(type))); MACRO_VAR(toReturn)->value = v; MACRO_VAR(toReturn);})
#define LinkedList__append(type, list, v) {++list.length; linkedlistNodeTypeName(type)* MACRO_VAR(current) = list.first; while (MACRO_VAR(current) && MACRO_VAR(current)->next) {MACRO_VAR(current) = MACRO_VAR(current)->next;}; linkedlistNodeTypeName(type)* MACRO_VAR(next) = LinkedList__createNodePtr(type, v); if (list.first) {MACRO_VAR(current)->next = MACRO_VAR(next);} else {list.first = MACRO_VAR(next);};}
#define LinkedList__free(type, list) {linkedlistNodeTypeName(type)* MACRO_VAR(current) = list.first; while (MACRO_VAR(current)) {linkedlistNodeTypeName(type)* MACRO_VAR(next) = MACRO_VAR(current)->next; free(MACRO_VAR(current)); MACRO_VAR(current) = MACRO_VAR(next);}}

#define DECLARE_LINKEDLIST_TYPE(type) typedef struct linkedlistNodeTypeName(type) {struct linkedlistNodeTypeName(type)* next; type value;} linkedlistNodeTypeName(type); typedef struct {linkedlistNodeTypeName(type)* first; int length;} linkedlistTypeName(type)


DECLARE_LINKEDLIST_TYPE(int);
DECLARE_LINKEDLIST_TYPE(str);


