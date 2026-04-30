#ifndef _LINKEDMAP
#define _LINKEDMAP

#include "linkedlist.h"
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
#define LINKEDMAP_FIND_NODE(keyType, valueType, map, k, equalityFunction) ({LINKEDMAP_NODE_NAME(keyType, valueType) toReturn; LinkedList__foreach(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType), MACRO_VAR(current), map) {if (equalityFunction(MACRO_VAR(current).key, k)) {toReturn = MACRO_VAR(current); break;}}; toReturn;})
#define LinkedMap__find(keyType, valueType, map, k, equalityFunction) ({valueType toReturn; LinkedList__foreach(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType), MACRO_VAR(current), map) {if (equalityFunction(MACRO_VAR(current).key, k)) {toReturn = MACRO_VAR(current).value; break;}}; toReturn;})
#define LinkedMap__set(keyType, valueType, map, k, v, equalityFunction) if (LinkedMap__containsKey(keyType, valueType, map, k, equalityFunction)) {LinkedList__foreach(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType), MACRO_VAR(current), map) {if (equalityFunction(MACRO_VAR(current).key, k)) {MACRO_VAR(current).value = v; break;}};} else {LinkedMap__insert(keyType, valueType, map, k, v)}

#endif