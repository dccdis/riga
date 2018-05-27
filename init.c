#include <init.h>

USER *parse_line(const char *line){
    USER *returnvalue;
    unsigned char *temphash;
    returnvalue = malloc (sizeof (USER));
    temphash = malloc (32 * sizeof(char));
    bzero (returnvalue->username, USERNAME_MAX);
    bzero (returnvalue->salt, SALTSTRINGLEN);
    bzero (returnvalue->hash, HASHLEN);
    bzero (temphash, HASHLEN);
   
    unsigned int tempsalt;
    sscanf (line, "%[^|]|%08x|%[^\n]", returnvalue->username, &tempsalt, temphash);
    sprintf (returnvalue->salt, "%u", tempsalt);

    unsigned char little_endian[16];

    sscanf (temphash,    "%08x", (unsigned int *) &little_endian[0]);
    sscanf (temphash+8,  "%08x", (unsigned int *) &little_endian[4]);
    sscanf (temphash+16, "%08x", (unsigned int *) &little_endian[8]);
    sscanf (temphash+24, "%08x", (unsigned int *) &little_endian[12]);

    returnvalue->hash[0] = little_endian[3];
    returnvalue->hash[1] = little_endian[2];
    returnvalue->hash[2] = little_endian[1];
    returnvalue->hash[3] = little_endian[0];
    returnvalue->hash[4] = little_endian[7];
    returnvalue->hash[5] = little_endian[6];
    returnvalue->hash[6] = little_endian[5];
    returnvalue->hash[7] = little_endian[4];
    returnvalue->hash[8] = little_endian[11];
    returnvalue->hash[9] = little_endian[10];
    returnvalue->hash[10] = little_endian[9];
    returnvalue->hash[11] = little_endian[8];
    returnvalue->hash[12] = little_endian[15];
    returnvalue->hash[13] = little_endian[14];
    returnvalue->hash[14] = little_endian[13];
    returnvalue->hash[15] = little_endian[12];

    free(temphash);
    return returnvalue;
}

USER **parse_hashfile (char *filename) {
    FILE *fp;
    fp = fopen (filename, "r");
    if (fp == NULL) {
        fputs ("opening hashfile failed\n",stderr);
        exit (EXIT_FAILURE);
    }
    
    char line [USERNAME_MAX];
    char *user;
    USER **usermap;
    usermap = malloc (MAX_ENTRIES * sizeof (USER));
    bzero (usermap, MAX_ENTRIES * sizeof(USER));
    int entrycount = 0;

    while (fgets (line, USERNAME_MAX, fp) != NULL){
        user = parse_line (line);
        if (user != NULL){
            usermap[entrycount] = user;
            entrycount++;
        }
    }
    fclose (fp);
    return usermap;
}

int count_entries (USER **usermap){
    int entrycount = 0;
    while (usermap[++entrycount]->username != 0);
    return entrycount;
}

void parse_chrsets (char *filename, CONFIG *cfg) {
    FILE *fp;
    fp = fopen (filename, "r");
    if (fp == NULL) {
        fputs ("opening charsetfile failed\n",stderr);
        exit (EXIT_FAILURE);
    }

    char line[MAX_CHARSET];
    char temp[MAX_CHARSET];
    int i;
    while (fgets (line, MAX_CHARSET, fp) != NULL) {
        bzero (temp, MAX_CHARSET);
        sscanf (line, "%d: %[^\n]", &i, temp);
        if (i < 1 || i > 12) continue;
        strncpy (cfg->charsets[i-1], temp, MAX_CHARSET-1);
        cfg->charsetlengths[i-1] = strlen (cfg->charsets[i-1]);
    }

    fclose(fp);
}
