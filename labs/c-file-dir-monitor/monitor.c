#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include <ftw.h>
#include "logger.h"

#define MAXLEVEL 1

static int inotifyFd;
char* directories[250];

static int watchsubdir(const char *pathname, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    if (ftwbuf->level > MAXLEVEL) {
        return 0; 
    }
    if(tflag == FTW_D) {
        int wd = inotify_add_watch(inotifyFd, pathname, IN_ISDIR|IN_CREATE|IN_DELETE|IN_MOVED_FROM|IN_MOVED_TO);
        if (wd == -1) {
            printf("Error @ inotify_add_watch\n");
            return -1;
        }
        char *path = strdup(pathname);
        directories[wd] = path;
    }
    return 0;
}

static char *displayInotifyEvent(struct inotify_event *i) {
    char *fullname = malloc(sizeof(char) * (strlen(directories[i->wd]) + strlen(i->name)) + 2);
    strcpy(fullname, directories[i->wd]);
    strcat(fullname, "/");
    strcat(fullname, i->name);
    if(i->mask & IN_ISDIR) {
        if (i->mask & IN_CREATE) {
            printf("- [Directory - Create] - %s\n", fullname);
            return i->name;
        } 
        if (i->mask & IN_DELETE) printf("- [Directory - Removal] - %s\n", fullname);
        if (i->mask & IN_MOVED_FROM) printf("- [Directory - Rename] - %s", fullname);
        if (i->mask & IN_MOVED_TO) printf(" -> %s\n", fullname);
    } else {
        if (i->mask & IN_CREATE) printf("- [File - Create] - %s\n", fullname);
        if (i->mask & IN_DELETE) printf("- [File - Removal] - %s\n", fullname);
        if (i->mask & IN_MOVED_FROM) printf("- [File - Rename] - %s", fullname);
        if (i->mask & IN_MOVED_TO) printf(" -> %s\n", fullname);
    }
    return "";
}

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Incorrect usage\n");
		printf("%s <directory>\n", argv[0]);
        return -1;
    }

    int wd, j;
    char buf[BUF_LEN] __attribute__ ((aligned(8)));
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

    inotifyFd = inotify_init();
    if (inotifyFd == -1) {
        printf("Error @ inotify_init\n");
        return -1;
    }

    int flags = FTW_SKIP_SUBTREE;
    int ret = nftw(argv[1], watchsubdir, 5, flags);   

    for (;;) {
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0) {
            printf("Fatal: read() from inotify fd returned 0!\n");
            return -1;
        }

        if (numRead == -1) {
            printf("Error @ read\n");
            return -1;
        }

        for (p = buf; p < buf + numRead; ) {
            event = (struct inotify_event *) p;
            char *dirname = displayInotifyEvent(event);
            if(strlen(dirname) != 0 && event->wd == 1) {
                char *subdir = malloc(sizeof(char) * (strlen(argv[1]) + strlen(dirname)) + 2);
                strcpy(subdir, argv[1]);
                strcat(subdir, "/");
                strcat(subdir, dirname);
                wd = inotify_add_watch(inotifyFd, subdir, IN_ISDIR|IN_CREATE|IN_DELETE|IN_MOVED_FROM|IN_MOVED_TO);
                if (wd == -1) {
                    printf("Error @ inotify_add_watch\n");
                    return -1;
                }
                directories[wd] = subdir;
            }

            p += sizeof(struct inotify_event) + event->len;
        }
    }

    return 0;
}
