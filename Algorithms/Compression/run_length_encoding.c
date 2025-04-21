//  Given a string(containing lowercase and uppercase English letters) of length N, 
//  the string should be encoded according to the following rule : each maximal sequence
//  consisting of the same character will be replaced by the character followed by the 
//  length of the sequence.The length of the string is unknown, so it is necessary to read 
//  and write in blocks.The maximum complexity should be O(N).

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 100

typedef struct {
	char* v;
	int lastIndex;
	int dim;
} Stack;

void initStack(Stack* s) {
	s->dim = 10;

	s->v = (char*)malloc((s->dim) * sizeof(char));
	if (s->v == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	s->lastIndex = -1;
}

void Push(Stack* s, char val) {
	if (s->lastIndex == s->dim - 1){
		s->dim *= 2;
		char* temp = (char*)realloc(s->v, s->dim * sizeof(char));
		if (temp == NULL) {
			free(s->v); 
			exit(1); 
		}
		s->v = temp; 
	}

	s->v[++s->lastIndex] = val;
}

void PushNum(Stack* s, int num) {
	char* stringNum = (char *)malloc(25 * sizeof(char));
	if (stringNum == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	sprintf(stringNum, "%d", num); // converting num to a string

	for (int i = 0; i < strlen(stringNum); i++) {
		Push(s, stringNum[i]);
	}

	free(stringNum);
}

void freeStack(Stack* s) {
	free(s->v);
	s->v = NULL;
	s->dim = 0;
	s->lastIndex = -1;
}

char* encode(const char* text) {
	if (text == NULL) return NULL;

	size_t len = strlen(text);
	if (len == 0) {
		return strdup("");
	}

	char currentChar = text[0];
	int counter = 1;

	Stack encoded;
	initStack(&encoded);


	for (size_t i = 1; i <= len; i++) {
		if (currentChar != text[i]) {
			Push(&encoded, currentChar);
			PushNum(&encoded, counter);

			currentChar = text[i];
			counter = 1;
		}
		else {
			counter++;
		}
	}

	Push(&encoded, '\0');

	char* result = strdup(encoded.v); // copying the resulting string to a new memory location
	freeStack(&encoded);

	return result;
}

int main() {

	char* inputStr = (char*)malloc(MAX_INPUT_SIZE * sizeof(char));
	if (inputStr == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}

	printf("Introduceti stringul: ");
	fgets(inputStr, MAX_INPUT_SIZE, stdin);

	char* encoded = encode(inputStr);
	printf("Original: %s \n Encoded: %s", inputStr, encoded);

	free(encoded);
	free(inputStr);

	return 0;
}