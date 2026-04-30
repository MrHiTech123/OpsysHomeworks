int largestTokenLengthInFile(const char* fileName) {
	int fileHandle = open(fileName, 0);
	if (fileHandle < 0) {
		return -1;
	}
	
	int current = 0;
	int longest = 0;
	
	char currentChar = 0;
	int charactersRead = 100;
	while (charactersRead) {
		charactersRead = read(fileHandle, &currentChar, 1);
		if (isspace(currentChar)) {
			if (current > longest) {
				longest = current;
			}
			current = 0;
		}
		else {
			++current;
		}
	}
	
	return longest;
	
	
	
}
