#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void quicksort(void *lineptr[], int left, int right,
	  int (*comp)(void *, void *));

void mergesort(void *lineptr[], int left, int right,
	  int (*comp)(void *, void *));

int numcmp(char *s1, char *s2) {
	int v1, v2;
	v1 = atoi(s1);
	v2 = atoi(s2);
	if(v1 < v2) {
		return -1;
	} else if(v1 > v2) {
		return 1;
	} else {
		return 0;
	}
}

int main(int argc, char **argv)
{
	if(argc < 3 || argc > 6) {
		printf("Incorrect usage\n");
		printf("%s [-n] <input file> -<algorithm> [-o <output file>]\n", argv[0]);
		printf("Example 1:%s -n numbers.txt -quicksort -o qs_sorted_numbers.txt\n", argv[0]);
		printf("Example 2:%s strings.txt -mergesort -o ms_sorted_strings.txt\n", argv[0]);
		return 0;
	}

	int numeric = 0;
	if(strcmp(argv[1], "-n") == 0) {
		numeric = 1;
	}

	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	char *inFileName;
	char *algorithm;
	char outFileName[255];
	if(numeric == 1) {
		inFileName = argv[2];
		fp = fopen(inFileName, "r");
		algorithm = argv[3];
		if(argc > 4) {
			strcpy(outFileName, argv[5]);
		} else {
			strcpy(outFileName, "sorted_");
			strcat(outFileName, inFileName);
		}
	} else {
		inFileName = argv[1];
		fp = fopen(inFileName, "r");
		algorithm = argv[2];
		if(argc > 3) {
			strcpy(outFileName, argv[4]);
		} else {
			strcpy(outFileName, "sorted_");
			strcat(outFileName, inFileName);
		}
	}

	if(fp == NULL) {
		printf("File %s doesn't exist\n", inFileName);
		return 0;
	}

	
	int lineNum = 0;
	while((read = getline(&line, &len, fp)) != -1) {
		lineNum++;
	}

	fseek(fp, 0, SEEK_SET);
	char **inputArray = (char **)malloc(lineNum*sizeof(char*));
	int index = 0;
	while((read = getline(&inputArray[index], &len, fp)) != -1) {
		index++;
	}
	fclose(fp);

	if(strcmp(algorithm, "-quicksort") == 0) {
		printf("... sorting strings.txt file with quicksort\n");
		if(numeric) {
			quicksort((void **) inputArray, 0, lineNum-1, (int(*)(void*, void*))(numcmp));
		} else {
			quicksort((void **) inputArray, 0, lineNum-1, (int(*)(void*, void*))(strcmp));
		}
	} else if(strcmp(algorithm, "-mergesort") == 0) {
		printf("... sorting strings.txt file with mergesort\n");
		if(numeric) {
			mergesort((void **) inputArray, 0, lineNum-1, (int(*)(void*, void*))(numcmp));
		} else {
			mergesort((void **) inputArray, 0, lineNum-1, (int(*)(void*, void*))(strcmp));
		}
	} else {
		printf("Error: The only available algorithms are: -quicksort and -mergsort\n");
		return 0;
	}

	fp = fopen(outFileName, "w");

	for(int i = 0; i < lineNum; i++) {
		fprintf(fp, "%s", inputArray[i]);
	}

	printf("... Results file can be found at %s\n", outFileName);
	fclose(fp);

	return 0;
}
