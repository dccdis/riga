#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <spu_intrinsics.h>
#include <spu_mfcio.h>
#include <md5.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LINE_MAX 64
#define MAX_ENTRIES 64
#define SALTSTRINGLEN 32
#define MAX_PWLEN 12
#define HASHLEN 16
#define LIKELY 1
#define UNLIKELY 0

typedef struct user {
    char username[LINE_MAX];
    char salt[SALTSTRINGLEN];
    vec_uint4 hash;
} USER;

typedef struct crack_params { 
    USER usermap[MAX_ENTRIES];
    int entrycount; 
    int maxlen;
    int offset;
    int step;
    int charsetlengths[MAX_PWLEN];
    char charsets[MAX_PWLEN][128];
    char padding[192]; 
} CRACK_PARAMS;

/* Sets up and returns generator for charset and length */
char **setup_generator (const char *charset, const int length);

/* Generates all strings up to length, returns 1 while there are combinations available
 * returns 0 once the value range runs out */
int generate_next_combination (
        char *combination,
        const int combinationlen,
        char **generator,
        const char *charset,
        const int charsetlen,
        const int step);

void preprocess (const int pwlen, const unsigned char *salt, const int saltlen, CHUNK *output);

#endif

