#ifndef _BIGRAM
#define _BIGRAM

#include "mystring.h"
#include "linkedmap.h"

DECLARE_LINKEDLIST_TYPE(str);
DECLARE_LINKEDMAP_TYPE(str, int);
DECLARE_LINKEDMAP_TYPE(str, LinkedMap_str_int);
typedef LinkedMap_str_LinkedMap_str_int BiGramHolder;

#define BIGRAMHOLDER_CONTAINS(holder, key) LinkedMap__containsKey(str, LinkedMap_str_int, holder, key, equals_str)
#define BIGRAMHOLDER_FIND(holder, key) LinkedMap__find(str, LinkedMap_str_int, holder, key, equals_str)


void BiGramHolder__add(BiGramHolder holder, str key, str value) {
	if (!BIGRAMHOLDER_CONTAINS(holder, key)) {
		LinkedMap__insert(str, LinkedMap_str_int, holder, key, LinkedMap__create);
	}
	
	LinkedMap_str_int locationToAdd = BIGRAMHOLDER_FIND(holder, key);
	
	int valueToAdd = 1;
	
	if (LinkedMap__containsKey(str, int, locationToAdd, value, equals_str)) {
		valueToAdd = LinkedMap__find(str, int, locationToAdd, value, equals_str) + 1;
	}
	
	LinkedMap__set(str, int, locationToAdd, value, valueToAdd, equals_str);
	printf("%s %s %d %d\n", key, value, LinkedMap__find(str, int, locationToAdd, value, equals_str), LinkedMap__find(str, int, BIGRAMHOLDER_FIND(holder, key), value, equals_str));
}

int BiGramHolder__getTimes(BiGramHolder holder, str firstWord, str secondWord) {
	LinkedMap_str_int location = BIGRAMHOLDER_FIND(holder, firstWord);
	printf("d %d\n", location.first->value.value);
	return LinkedMap__find(str, int, location, secondWord, equals_str);
}

BiGramHolder BiGramHolder__create(LinkedList_str words) {
	BiGramHolder toReturn = LinkedMap__create;
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


#endif