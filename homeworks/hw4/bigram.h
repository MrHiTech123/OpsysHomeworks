#ifndef _LINKEDLIST
#define _LINKEDLIST

#include <stdlib.h>
#include <stdbool.h>

#define LINKEDLIST_NODETYPE_NAME(type) LinkedListNode_##type
#define LINKEDLIST_TYPE_NAME(type) LinkedList_##type

#define MACRO_VAR(name) macroVar_ ## name ## __LINE__
#define LinkedList__foreach(type, varname, list) type varname; if (list.first) {varname = list.first->value;}; for (LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(currentList) = list.first; MACRO_VAR(currentList); ({MACRO_VAR(currentList) = MACRO_VAR(currentList)->next; if (MACRO_VAR(currentList)) {varname = MACRO_VAR(currentList)->value;}}))
#define LinkedList__create {.first = NULL}
#define LinkedList__createNodePtr(type, v) ({LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(toReturn) = (LINKEDLIST_NODETYPE_NAME(type)*)calloc(1, sizeof(LINKEDLIST_NODETYPE_NAME(type))); MACRO_VAR(toReturn)->value = v; MACRO_VAR(toReturn);})
#define LinkedList__append(type, list, v) {LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(current) = list.first; while (MACRO_VAR(current) && MACRO_VAR(current)->next) {MACRO_VAR(current) = MACRO_VAR(current)->next;}; LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(next) = LinkedList__createNodePtr(type, v); if (list.first) {MACRO_VAR(current)->next = MACRO_VAR(next);} else {list.first = MACRO_VAR(next);};}
#define __LINKED_LIST_FREE(type, list, freeFunction) {LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(current) = list.first; while (MACRO_VAR(current)) {LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(next) = MACRO_VAR(current)->next; freeFunction(MACRO_VAR(current)); MACRO_VAR(current) = MACRO_VAR(next);}}
#define LinkedList__length(type, list) ({int MACRO_VAR(toReturn) = 0; LinkedList__foreach(type, MACRO_VAR(unused), list) {MACRO_VAR(unused) = MACRO_VAR(unused); ++MACRO_VAR(toReturn);}; MACRO_VAR(toReturn);})
#define LinkedList__contains(type, haystack, needle, equalityFunction) ({bool MACRO_VAR(toReturn) = false; LinkedList__foreach(type, MACRO_VAR(current), haystack) {if (equalityFunction(MACRO_VAR(current), needle)) {MACRO_VAR(toReturn) = true; break;}}; MACRO_VAR(toReturn);})
#define LinkedList__copy(type, list) ({LINKEDLIST_TYPE_NAME(type) MACRO_VAR(toReturn__copy) = LinkedList__create; LinkedList__foreach(type, MACRO_VAR(current__copy), list) {LinkedList__append(type, MACRO_VAR(toReturn__copy), MACRO_VAR(current__copy));}; MACRO_VAR(toReturn__copy);})
#define LinkedList__getLast(type, list) ({type MACRO_VAR(toReturn); LinkedList__foreach(type, MACRO_VAR(current), list) {MACRO_VAR(toReturn) = MACRO_VAR(current);}; MACRO_VAR(toReturn);})

#define LinkedList__free(type, list) __LINKED_LIST_FREE(type, list, free)
#define LinkedList__freeFree(type, list) __LINKED_LIST_FREE(type, list, LinkedList__free)
#define LinkedList__freeFreeFree(type, list) __LINKED_LIST_FREE(type, list LinkedList__freeFre ## e)

#define DECLARE_LINKEDLIST_TYPE(type) typedef struct LINKEDLIST_NODETYPE_NAME(type) {struct LINKEDLIST_NODETYPE_NAME(type)* next; type value;} LINKEDLIST_NODETYPE_NAME(type); typedef struct {LINKEDLIST_NODETYPE_NAME(type)* first;} LINKEDLIST_TYPE_NAME(type)

#endif

#ifndef _LINKEDMAP
#define _LINKEDMAP

#include <stdbool.h>





#define LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType) LinkedMapDataStruct_ ## keyType ## _ ## valueType
#define LINKEDMAP_DATA_STRUCT_LINKED_LIST_TYPE_NAME(keyType, valueType) LINKEDLIST_TYPE_NAME(LinkedMapDataStruct_ ## keyType ## _ ## valueType)
#define LINKEDMAP_NODE_NAME(keyType, valueType) LINKEDLIST_NODETYPE_NAME(LinkedMapDataStruct_ ## keyType ## _ ## valueType)
#define LINKEDMAP_TYPE_NAME(keyType, valueType) LinkedMap_ ## keyType ## _ ## valueType
#define DECLARE_LINKEDMAP_TYPE(keyType, valueType) typedef struct {keyType key; valueType value;} LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType); DECLARE_LINKEDLIST_TYPE(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType)); typedef LINKEDMAP_DATA_STRUCT_LINKED_LIST_TYPE_NAME(keyType, valueType) LINKEDMAP_TYPE_NAME(keyType, valueType)
// typedef LinkedList__ ## LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType) LINKEDMAP_TYPE_NAME(keyType, valueType)

// LinkedList_LinkedMapDataStruct_int_str
#define LinkedMap__create LinkedList__create

#define equals(x, y) x == y


#define LinkedMap__containsKey(keyType, valueType, map, k, equalityFunction) ({bool toReturn = false; LinkedList__foreach(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType), MACRO_VAR(current), map) {if (equalityFunction(MACRO_VAR(current).key, k)) {toReturn = true;}}; toReturn;})
#define LinkedMap__insert(keyType, valueType, map, k, v) {LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType) MACRO_VAR(toAppend) = {.key = k, .value = v}; LinkedList__append(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType), map, MACRO_VAR(toAppend))}
#define LinkedMap__free(keyType, valueType, map) LinkedList__free(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType), map)
#define LINKEDMAP_FIND_NODE(keyType, valueType, map, k, equalityFunction) ({LINKEDMAP_NODE_NAME(keyType, valueType)* toReturn; LinkedList__foreach(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType), MACRO_VAR(current), map) {if (equalityFunction(MACRO_VAR(current).key, k)) {toReturn = MACRO_VAR(current); break;}}; toReturn;})
#define LinkedMap__find(keyType, valueType, map, k, equalityFunction) ({valueType toReturn; LinkedList__foreach(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType), MACRO_VAR(current), map) {if (equalityFunction(MACRO_VAR(current).key, k)) {toReturn = MACRO_VAR(current).value; break;}}; toReturn;})
#define LinkedMap__set(keyType, valueType, map, k, v, equalityFunction) if (LinkedMap__containsKey(keyType, valueType, map, k, equalityFunction)) {for (LINKEDMAP_NODE_NAME(keyType, valueType)* MACRO_VAR(current) = map.first; MACRO_VAR(current); MACRO_VAR(current) = MACRO_VAR(current)->next) {if (equalityFunction(MACRO_VAR(current)->value.key, k)) {MACRO_VAR(current)->value.value = v; break;}}} else {LinkedMap__insert(keyType, valueType, map, k, v)}

#endif