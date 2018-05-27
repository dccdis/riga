#ifndef _INIT_H_
#define _INIT_H_

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#define USERNAME_MAX 64
#define MAX_ENTRIES 64
#define MAX_CHARSET 128
#define SALTSTRINGLEN 32
#define HASHLEN 16
#define DEFAULTCHARSET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\0"

typedef struct user {
    char username[USERNAME_MAX];
    char salt[SALTSTRINGLEN];
    unsigned char hash[HASHLEN];
} USER;

typedef struct crack_params { 
    USER usermap[MAX_ENTRIES];
    int entrycount; 
    int maxlen;
    int offset;
    int step;
    int charsetlengths[12];
    char charsets[12][128];
    char padding[192]; 
} CRACK_PARAMS;

typedef struct config {
    int maxlen;
    int num_threads;
    int entrycount;
    int charsetlengths[12];
    char charsets[12][128];
} CONFIG;

static const CONFIG defaults = {7, 6, 0, { [0 ... 11] = 62 }, { [0 ... 11] = DEFAULTCHARSET }};

USER *parse_line (const char *line);

int count_entries (USER **usermap);

USER **parse_hashfile (char *filename);

void parse_chrsets (char *filename, CONFIG *cfg);

#endif
