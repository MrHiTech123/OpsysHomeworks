#ifndef _BIGRAM
#define _BIGRAM

#include "mystring.h"
#include "linkedmap.h"

DECLARE_LINKEDLIST_TYPE(str);
DECLARE_LINKEDMAP_TYPE(str, int);
DECLARE_LINKEDMAP_TYPE(str, LinkedMap_str_int);
typedef LinkedMap_str_LinkedMap_str_int* BiGramHolder;

#define BIGRAMHOLDER_CONTAINS(holder, key) LinkedMap__containsKey(str, LinkedMap_str_int, (*holder), key, equals_str)
#define BIGRAMHOLDER_FIND(holder, key) LinkedMap__find(str, LinkedMap_str_int, (*holder), key, equals_str)
#define BIGRAMHOLDER_FOREACH(varName, holder) LinkedList__foreach(LinkedMapDataStruct_str_LinkedMap_str_int, varName, (*holder))
#define BIGRAMHOLDER_SET(holder, firstWord, value) LinkedMap__set(str, LinkedMap_str_int, (*holder), firstWord, value, equals_str);


void BiGramHolder__display(BiGramHolder holder) {
	printf("Displaying bigram holder!\n");
	BIGRAMHOLDER_FOREACH(s, holder) {
		printf("%s\n", s.key);
		LinkedList__foreach(LinkedMapDataStruct_str_int, i, s.value) {
			printf("\t%s %d\n", i.key, i.value);
		}
	}
}

void BiGramHolder__add(BiGramHolder holder, str key, str value) {
	if (!BIGRAMHOLDER_CONTAINS(holder, key)) {
		LinkedMap_str_int toAdd = LinkedMap__create;
		BIGRAMHOLDER_SET(holder, key, toAdd);
	}
		
	LinkedMap_str_int locationToAdd = BIGRAMHOLDER_FIND(holder, key);
	
	int valueToAdd = 1;
	
	if (LinkedMap__containsKey(str, int, locationToAdd, value, equals_str)) {
		valueToAdd = LinkedMap__find(str, int, locationToAdd, value, equals_str) + 1;
	}
	
	LinkedMap__set(str, int, locationToAdd, value, valueToAdd, equals_str);
	BIGRAMHOLDER_SET(holder, key, locationToAdd);
	
}

int BiGramHolder__getTimes(BiGramHolder holder, str firstWord, str secondWord) {
	LinkedMap_str_int location = BIGRAMHOLDER_FIND(holder, firstWord);
	if (!LinkedMap__containsKey(str, int, location, secondWord, equals_str)) {
		return 0;
	}
	
	return LinkedMap__find(str, int, location, secondWord, equals_str);
}

BiGramHolder BiGramHolder__create(LinkedList_str words) {
	BiGramHolder toReturn = calloc(1, sizeof(LinkedMap_str_LinkedMap_str_int));
	toReturn->first = NULL;
	str prev = NULL;
	LinkedList__foreach(str, word, words) {
		if (prev != NULL) {
			BiGramHolder__add(toReturn, prev, word);
		}				
		prev = word;
		continue;
	}
	
	return toReturn;
}

// LinkedList_str BiGramHolder__(Get)

void BiGramHolder__free(BiGramHolder holder) {
	BIGRAMHOLDER_FOREACH(dataStruct, holder) {
		LinkedMap__free(str, int, dataStruct.value);
	}
	LinkedMap__free(str, LinkedMap_str_int, (*holder));
	free(holder);
}


#endif