#ifndef _BIGRAM
#define _BIGRAM
#include <stdio.h>

#include "mystring.h"
#include "linkedmap.h"

DECLARE_LINKEDLIST_TYPE(str);
DECLARE_LINKEDLIST_TYPE(LinkedList_str);
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

str joinAll(LinkedList_str toJoin) {
	int listLen = LinkedList__length(str, toJoin);
	int stringLen = listLen;
	LinkedList__foreach(str, word, toJoin) {
		stringLen += strlen(word);
	}
	
	str toReturn = calloc(stringLen, sizeof(char));
	int currentIndex = 0;
	
	LinkedList__foreach(str, word2, toJoin) {
		strcpy(toReturn + currentIndex, word2);
		currentIndex += strlen(word2);
		*(toReturn + currentIndex) = ' ';
		++currentIndex;
	}
		
	return toReturn;
}

LinkedList_str BiGramHolder__rankTopNextWords(BiGramHolder holder, str currentWord, int wordsToRank) {
	printf("Find words %s, %d\n", currentWord, wordsToRank);
	
	LinkedList_str toReturn = LinkedList__create;
	
	if (!BIGRAMHOLDER_CONTAINS(holder, currentWord)) {
		return toReturn;
	}
	
	LinkedMap_str_int location = BIGRAMHOLDER_FIND(holder, currentWord);
	int locationLen = LinkedList__length(LinkedMapDataStruct_str_int, location);
	
	
	
	for (int i = 0; i < wordsToRank && i < locationLen; ++i) {
		str toAdd = NULL;
		int mostAmounts = 0;
		
		LinkedList__foreach(LinkedMapDataStruct_str_int, wordAndFrequency, location) {
			if (wordAndFrequency.value > mostAmounts && !LinkedList__contains(str, toReturn, wordAndFrequency.key, equals_str)) {
				toAdd = wordAndFrequency.key;
				mostAmounts = wordAndFrequency.value;
			}
		}
		
		LinkedList__append(str, toReturn, toAdd);
	}
	
	return toReturn;
}



LinkedList_LinkedList_str BiGramHolder__generateSentences(BiGramHolder holder, str rootWord, int breadth, int depth) {
	LinkedList_LinkedList_str toReturn = LinkedList__create;
	LinkedList_str rootList = LinkedList__create;
	LinkedList__append(str, rootList, rootWord);
	LinkedList__append(LinkedList_str, toReturn, rootList);
	
	for (int currentDepth = 0; currentDepth < depth; ++currentDepth) {
		printf("Outer loop\n");
		
		LinkedList_LinkedList_str toReplaceToReturn = LinkedList__create;
		LinkedList__foreach(LinkedList_str, subList, toReturn) {
			printf("Sublist loop\n");
			
			str lastWord = LinkedList__getLast(str, subList);
			LinkedList_str possibleNextWords = BiGramHolder__rankTopNextWords(holder, lastWord, breadth);
			int lastNextWordForListCopy = LinkedList__length(str, possibleNextWords) - 1;
			
			if (lastNextWordForListCopy < 0) {
				continue;
			}
			
			
			int currentBreadth = 0;
			LinkedList__foreach(str, currentNextWord, possibleNextWords) {
				printf("Next word loop\n");
				if (currentBreadth < lastNextWordForListCopy) {
					LinkedList_str currentCopy = LinkedList__copy(str, subList);
										
					LinkedList__append(str, currentCopy, currentNextWord);
					LinkedList__append(LinkedList_str, toReplaceToReturn, currentCopy);
				}
				++currentBreadth;
			}
			
			LinkedList__append(str, subList, LinkedList__getLast(str, possibleNextWords));
			LinkedList__append(LinkedList_str, toReplaceToReturn, subList);
		}
		
		LinkedList__free(LinkedList_str, toReturn);
		toReturn = toReplaceToReturn;
		
	}
	
	
	return toReturn;
	
}


void BiGramHolder__addAll(BiGramHolder holder, LinkedList_str words) {
	str prev = NULL;
	LinkedList__foreach(str, word, words) {
		if (prev != NULL) {
			BiGramHolder__add(holder, prev, word);
		}
		prev = word;
		continue;
	}
}

BiGramHolder BiGramHolder__create(LinkedList_str words) {
	BiGramHolder toReturn = calloc(1, sizeof(LinkedMap_str_LinkedMap_str_int));
	toReturn->first = NULL;
	BiGramHolder__addAll(toReturn, words);
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