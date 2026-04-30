#include "../linkedmap.h"

DECLARE_LINKEDLIST_TYPE(str);
DECLARE_LINKEDMAP_TYPE(str, int);

int main(int argc, char const *argv[])
{
	LinkedMap_str_int map = LinkedMap__create;
	
	// LinkedMapDataStruct_int_str x = {.key = 1, .value = "Hi"};
	
	// LinkedList__append(LINKEDMAP_DATA_STRUCT_NAME(int, str), map, {.key = 1; .value = "Hi"});
	
	LinkedMap__insert(str, int, map, "HI", 1);
	// {++map.length; LinkedListNode_LinkedMapDataStruct_int_str* macroVar_current__LINE__ = map.first; while (macroVar_current__LINE__ && macroVar_current__LINE__->next) {macroVar_current__LINE__ = macroVar_current__LINE__->next;}; LinkedListNode_LinkedMapDataStruct_int_str* macroVar_next__LINE__ = ({LinkedListNode_LinkedMapDataStruct_int_str* macroVar_toReturn__LINE__ = calloc(1, sizeof(LinkedListNode_LinkedMapDataStruct_int_str)); macroVar_toReturn__LINE__->value = (LinkedMapDataStruct_int_str){.key = 1, .value = "HI"}; macroVar_toReturn__LINE__;}); if (map.first) {macroVar_current__LINE__->next = macroVar_next__LINE__;} else {map.first = macroVar_next__LINE__;};}
	
	LinkedMap__insert(str, int, map, "Bye", 2);
	
	printf("%d\n", LinkedMap__find(str, int, map, "HI", equals_str));
	printf("%d \n", LinkedMap__containsKey(str, int, map, "Bye", equals_str));
	
	// ({str toReturn; LinkedMapDataStruct_int_str macroVar_current__LINE__; if (map.first) {macroVar_current__LINE__ = map.first->value;}; for (LinkedListNode_LinkedMapDataStruct_int_str* macroVar_currentList__LINE__ = map.first; macroVar_currentList__LINE__; ({macroVar_currentList__LINE__ = macroVar_currentList__LINE__->next; macroVar_current__LINE__ = (macroVar_currentList__LINE__)? macroVar_currentList__LINE__->value : macroVar_current__LINE__;})) {if (macroVar_current__LINE__.key == 2) {toReturn = macroVar_current__LINE__.value;}}; toReturn});
	
	LinkedMap__free(str, int, map);
	
	
	
	
	
	return 0;
}
