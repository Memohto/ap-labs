#include <stdio.h>
#include <stdlib.h>

int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);
int mystrcmp(char *a, char *b);

int main(int argc, char **argv) {
    if(argc != 4) {
        printf("Incorrect usage\n");
        printf("Option 1: %s -add <string 1> <string 2>\n", argv[0]);
        printf("Option 2: %s -find <string> <substring>\n", argv[0]);
        return 0;
    }

    if(mystrcmp(argv[1], "-add") == 0) {
        printf("Initial Lenght: %d\n", mystrlen(argv[2]));
        char *result = mystradd(argv[2], argv[3]);
        printf("New String: %s\n", result);
        printf("New length: %d\n", mystrlen(result));
        free(result);
    } else if(mystrcmp(argv[1], "-find") == 0) {
        int index = mystrfind(argv[2], argv[3]);
        if(index >= 0) {
            printf("['%s'] string was found at [%d] position\n", argv[3], index);
        } else {
            printf("['%s'] string was not found\n", argv[3]);
        }
    } else {
        printf("Incorrect usage\n");
        printf("Option 1: %s -add <string 1> <string 2>\n", argv[0]);
        printf("Option 2: %s -find <string> <substring>\n", argv[0]);
        return 0;
    }
    return 0;
}
