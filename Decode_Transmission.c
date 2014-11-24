#include <stdio.h>
#include <stdlib.h>

#define SIZE 256

// TODO: check if calloc works w/ MSP

typedef struct {
	int size;
	int id;
	int pulses[SIZE];
} IRCode;

void parseCode(IRCode *code, char *codeStr);
void stringCopy(int start, int end, char* str, char* res);

int main() {
	char *codeStr = "37-4-500,1000,500,1000|";
	IRCode Code;
	
	parseCode(&Code, codeStr);
	printf("id: %d\n", Code.id);
	printf("size: %d\n", Code.size);
	int i = 0;
	for (i; i < Code.size; i++) {
		printf("pulse %d: %d\n", i, Code.pulses[i]);
	}
	return 0;
}

void parseCode(IRCode *code, char *codeStr) {
	int i = 0;
	char c = codeStr[0];
	int start = 0;
	int end = 0;
	int code_index = 0;
	int len = 0;
	int result = 0;
	int id_set = 0;
	int size_set = 0;
	for (i; c != '|'; i++) {
		c = codeStr[i];
		switch (c) {
			case '-':
				len = end-start;
				char *res1 = calloc(sizeof(char),len);
				stringCopy(start, end-1,codeStr,res1);
				result = atoi(res1);
				start = i+1;
				end = i+1;
				if (!id_set) {
					id_set = 1;
					code->id = result;
				} else {
					size_set = 1;
					code->size = result;
				}
				break;
			case ',':
				len = end-start;
				char *res2 = calloc(sizeof(char),len);
				stringCopy(start, end-1,codeStr,res2);
				result = atoi(res2);
				start = i+1;
				end = i+1;
				code->pulses[code_index++] = result;
				break;
			case '|':
				len = end-start;
				char *res3 = calloc(sizeof(char),len);
				stringCopy(start, end-1,codeStr,res3);
				result = atoi(res3);
				start = i+1;
				end = i+1;
				code->pulses[code_index++] = result;
				break;
			default:
				end++;
				break;
		}
	}
	code->pulses[code_index] = '\0';
}

void stringCopy(int start, int end, char* str, char* res) {
	int i = start;
	for (i;i<= end; i++){
		res[i-start] = str[i];
	}
}
