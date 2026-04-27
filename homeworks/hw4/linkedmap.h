#include "linkedlist.h"





#define LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType) LinkedMapDataStruct_ ## keyType ## _ ## valueType
#define LINKEDMAP_DATA_STRUCT_LINKED_LIST_TYPE_NAME(keyType, valueType) LINKEDLIST_TYPE_NAME(LinkedMapDataStruct_ ## keyType ## _ ## valueType)
#define LINKEDMAP_NODE_NAME(keyType, valueType) LINKEDLIST_NODETYPE_NAME(LinkedMapDataStruct_ ## keyType ## _ ## valueType)
#define LINKEDMAP_TYPE_NAME(keyType, valueType) Map_ ## keyType ## _ ## valueType
#define DECLARE_LINKEDMAP_TYPE(keyType, valueType) typedef struct {keyType key; valueType value;} LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType); DECLARE_LINKEDLIST_TYPE(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType)); typedef LINKEDMAP_DATA_STRUCT_LINKED_LIST_TYPE_NAME(keyType, valueType) LINKEDMAP_TYPE_NAME(keyType, valueType)
// typedef LinkedList__ ## LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType) LINKEDMAP_TYPE_NAME(keyType, valueType)

#define MAP_FIND_ELEMENT(key, value)

DECLARE_LINKEDMAP_TYPE(int, str);

// LinkedList_LinkedMapDataStruct_int_str
#define LinkedMap__create LinkedList__create

