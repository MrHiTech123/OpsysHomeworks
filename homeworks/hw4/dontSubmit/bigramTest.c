#include "../bigram.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
	
	LinkedList_str words = LinkedList__create;
	
	LinkedList__append(str, words, "the");
	LinkedList__append(str, words, "princess");
	LinkedList__append(str, words, "the");
	LinkedList__append(str, words, "princess");
	LinkedList__append(str, words, "the");
	LinkedList__append(str, words, "princess");
	LinkedList__append(str, words, "the");
	LinkedList__append(str, words, "dragon");
	LinkedList__append(str, words, "the");
	LinkedList__append(str, words, "dragon");
	LinkedList__append(str, words, "the");
	LinkedList__append(str, words, "peacock");
	
	
	
	
	BiGramHolder holder = BiGramHolder__create(words);
	
	for (int i = 1; i <= 5; ++i) {
		LinkedList_str nextWords = BiGramHolder__rankTopNextWords(holder, "the", i);
		
		printf("Number %d\n");
		
		LinkedList__foreach(str, word, nextWords) {
			printf("\t%s\n", word);
		}
		
		LinkedList__free(str, nextWords);
		
	}
	
	
	
	str sentence = joinAll(words);
	
	printf("%s\n", sentence);
	
	
	
	
	LinkedList_LinkedList_str sentences = BiGramHolder__generateSentences(holder, "the", 6, 1);
	
	printf("%d\n", LinkedList__length(LinkedList_str, sentences));
	
	LinkedList__foreach(LinkedList_str, currentList, sentences) {
		printf("%s\n", joinAll(currentList));
	}
	
	
	BiGramHolder__free(holder);
	free(sentence);
	
	
	LinkedList__free(str, words);
	
	
	return 0;
}













