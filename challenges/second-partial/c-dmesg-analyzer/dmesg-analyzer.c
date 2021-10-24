#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define REPORT_FILE "dmesg_report.txt"

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage:./dmesg-analizer logfile.txt\n");
        return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

#define HASHSIZE 50000
#define ARRSIZE 3000
#define LOGSIZE 1000
#define KEYSIZE 1000
static struct nlist *hashtab[HASHSIZE];

struct nlist {
    struct nlist *next;
    char *name;
    char defn[ARRSIZE][LOGSIZE];
    int lasti;
};

unsigned hash(char *s) {
    unsigned hashval;
    for(hashval = 0; *s != '\0'; s++) {
        hashval = *s + 31 * hashval;
    }
    return hashval % HASHSIZE;
}

struct nlist *lookup(char *s) {
    struct nlist *np;
    for(np = hashtab[hash(s)]; np != NULL; np = np->next) {
        if(strcmp(s, np->name) == 0) {
            return np;
        }
    }
    return NULL;
}

struct nlist *install(char *name, char *defn) {
    struct nlist *np;
    unsigned hashval;
    if((np = lookup(name)) == NULL) {
        np = (struct nlist *) malloc(sizeof(*np));
        if(np == NULL || (np->name = strdup(name)) == NULL) {
            return NULL;
        }
        hashval = hash(name);
        np->next = hashtab[hashval];
        np->lasti = 0;
        strcpy(np->defn[np->lasti], defn);
        hashtab[hashval] = np;
    } else {
        np->lasti++;
        strcpy(np->defn[np->lasti], defn);
    }
    return np;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    // Implement your solution here.
    FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

    fp = fopen(logFile, "r");

    if(fp == NULL) {
		printf("File %s doesn't exist\n", logFile);
		return;
	}

	while((read = getline(&line, &len, fp)) != -1) {
        char log[LOGSIZE] = "";
        char key[KEYSIZE] = "";
        int concatLog = 1;
        int doneKey = 0;
        char *current = line;
		for(;*current != '\n'; current++) {
            if(concatLog == -1 && *current != ' ' && doneKey == 0) {
                concatLog = 0;
            } else if(concatLog == 0 && *current == ' ' && doneKey == 1) {
                concatLog = 1;
            }

            if(concatLog == 1) {
                strncat(log, &(*current), 1);
            } else if(concatLog == 0) {
                strncat(key, &(*current), 1);
            }

            if(concatLog == 1 && *current == ']' && doneKey == 0) {
                concatLog = -1;
            } else if(concatLog == 0 && *current == ':') {
                doneKey = 1;
            }
        }
        if(doneKey == 0) {
            strcat(log, " ");
            strcat(log, key);
            strcpy(key, "General:");
        }
        if(strcmp(log, " ") != 0)
            install(key, log);
	}

    fclose(fp);

    int wfp = open(report, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (wfp < 0) {
        printf("Couldn't write on file %s", report);
        return;
    }

    char message[LOGSIZE];
    for(int i = 0; i < HASHSIZE; i++) {
        struct nlist *np;
        if((np = hashtab[i]) != NULL) {
            strcpy(message, "");
            strcat(message, np->name);
            strcat(message, "\n");
            write(wfp, message, strlen(message));
            for(int j = 0; j < ARRSIZE; j++) {
                if(strcmp(np->defn[j], "") != 0) {
                    strcpy(message, "  ");
                    strcat(message, np->defn[j]);
                    strcat(message, "\n");
                    write(wfp, message, strlen(message));
                } else {
                    break;
                }
            }
        }
    }

    close(wfp);

    printf("Report is generated at: [%s]\n", report);
}