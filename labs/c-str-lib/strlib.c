 #include <stdlib.h>

int mystrlen(char *str) {
    int lenght = 0;
    for(int i = 0; str[i] != '\0'; i++) {
        lenght++;
    }
    return lenght;
}

char *mystradd(char *origin, char *addition) {
    char *result = (char *)malloc(mystrlen(origin) + mystrlen(addition) + 1);
    int i;
    for(i = 0; origin[i] != '\0'; i++) {
        result[i] = origin[i];
    }
    for(int j = 0; addition[j] != '\0'; j++) {
        result[i] = addition[j];
        i++;
    }
    return result;
}

int mystrfind(char *origin, char *substr) {
    int i = 0, j = 0;
    int matchFound = 0;
    while(origin[i] != '\0' && !matchFound) {
        j = 0;
        while(substr[j] != '\0' && origin[i+j] == substr[j]) {
            j++;
        }
        if(substr[j] == '\0') {
            matchFound = 1;
        } else {
            i++;
        }
    }
    return matchFound ? i : -1;
}

int mystrcmp(char *a, char *b) {
    int equal = 0;
    for(int i = 0; a[i] != '\0' && equal == 0; i++) {
        if(a[i] != b[i]) equal = -1;
    }
    return equal;
}
