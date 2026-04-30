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
	
	for (int i = 1; i <= 3; ++i) {
		LinkedList_str nextWords = BiGramHolder__rankTopNextWords(holder, "the", i);
		
		printf("Number %d\n");
		
		LinkedList__foreach(str, word, nextWords) {
			printf("\t%s\n", word);
		}
		
	}
	
	return 0;
}













