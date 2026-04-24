#include <stdio.h>


#include "linkedlist.h"



int main(int argc, char const *argv[])
{
	LinkedList_int myList = LinkedList_create;
	
	LinkedList_append(int, myList, 1);
	LinkedList_append(int, myList, 3);
	LinkedList_append(int, myList, 5);
	
	// printf("%d\n", __LINE__);
	LinkedList__foreach(int, i, myList) {
		printf("%d\n", i);
	}
	
	LinkedList_free(int, myList);
	
	LinkedList_str strings = LinkedList_create;
	
	LinkedList_append(str, strings, "Abcd");
	LinkedList_append(str, strings, "Hello");
	LinkedList_append(str, strings, "Jason");
	
	LinkedList__foreach(str, currentWord, strings) {
		printf("%s\n", currentWord);
	}
	
	LinkedList_free(str, strings);
	
	LinkedList_str strings2 = LinkedList_create;
	
	// LinkedList_append(str, strings2, "I LIED");
	
	LinkedList__foreach(str, thing, strings2) {
		printf("SHOULD NOT PRINT %s\n", thing);
	}
	
	LinkedList_free(str, strings2);
	
	return 0;
}








