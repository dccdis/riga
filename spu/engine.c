#include <engine.h>

char **setup_generator (const char *charset, const int length){
    int i;
    char **generator = malloc (length * sizeof (char*));
    
    for (i = 0; i < length; i++){
        generator[i] = (char *) charset;
    }

    return generator;
}

int generate_next_combination (
        char *combination, 
        const int combinationlen,
        char **generator, 
        const char *charset, 
        const int charsetlen,
        const int step){

    generator[0] += step;
    combination[0] = generator[0][0];

    int i = 0;
    while (__builtin_expect (generator[i] - charset >= charsetlen, UNLIKELY)){
        if (__builtin_expect (i >= (combinationlen-1), UNLIKELY)){
            return 0;
        }
        generator[i] -= charsetlen;
        generator[i+1]++;
        combination[i] = generator[i][0];
        combination[i+1] = generator[i+1][0];
        i++;
    }
    return 1;
}

void preprocess (const int pwlen, const unsigned char *salt, const int saltlen, CHUNK *output){
    
    unsigned char temp[64] = { 0x00 };
    int totallen = pwlen + saltlen;
    char lastbyte = 0x80;

    int i;
    for (i = pwlen; i < totallen; i++) temp[i] = salt[i-pwlen];
    temp[totallen] = lastbyte;
    
    vector signed char vec_temp[16];
    for (i = 0; i < 14; i++){
        vec_temp[i] = spu_insert (temp[(4*i)+3], vec_temp[i], 0);
        vec_temp[i] = spu_insert (temp[(4*i)+2], vec_temp[i], 1);
        vec_temp[i] = spu_insert (temp[(4*i)+1], vec_temp[i], 2);
        vec_temp[i] = spu_insert (temp[(4*i)],   vec_temp[i], 3);

        vec_temp[i] = spu_insert (temp[(4*i)+3], vec_temp[i], 4);
        vec_temp[i] = spu_insert (temp[(4*i)+2], vec_temp[i], 5);
        vec_temp[i] = spu_insert (temp[(4*i)+1], vec_temp[i], 6);
        vec_temp[i] = spu_insert (temp[(4*i)],   vec_temp[i], 7);

        vec_temp[i] = spu_insert (temp[(4*i)+3], vec_temp[i], 8);
        vec_temp[i] = spu_insert (temp[(4*i)+2], vec_temp[i], 9);
        vec_temp[i] = spu_insert (temp[(4*i)+1], vec_temp[i], 10);
        vec_temp[i] = spu_insert (temp[(4*i)],   vec_temp[i], 11);

        vec_temp[i] = spu_insert (temp[(4*i)+3], vec_temp[i], 12);
        vec_temp[i] = spu_insert (temp[(4*i)+2], vec_temp[i], 13);
        vec_temp[i] = spu_insert (temp[(4*i)+1], vec_temp[i], 14);
        vec_temp[i] = spu_insert (temp[(4*i)],   vec_temp[i], 15);
    }

    for (i = 0; i < 14; i++) output->md5data[i] = (vec_uint4) vec_temp[i];
   
    int bitlen = totallen * 8; 
    output->md5data[14] = (vec_uint4) { bitlen, bitlen, bitlen, bitlen };
    output->md5data[15] = (vec_uint4) { 0x00000000 };
}

int main (unsigned long long spe_id, unsigned long long argp, unsigned long long envp){

    int dma_tag = 1;
    CRACK_PARAMS params __attribute__((aligned(16)));
    spu_mfcdma64 (&params, mfc_ea2h(argp), mfc_ea2l(argp), sizeof(CRACK_PARAMS), dma_tag, MFC_GET_CMD);
    spu_writech (MFC_WrTagMask, 1 << dma_tag);
    spu_mfcstat (MFC_TAG_UPDATE_ALL);

    USER usermap[MAX_ENTRIES];
    char charsets[MAX_PWLEN][128];
    int charsetlengths[MAX_PWLEN];
    memcpy (charsets, params.charsets, sizeof(params.charsets));
    memcpy (charsetlengths, params.charsetlengths, sizeof(params.charsetlengths));
    memcpy (usermap, params.usermap, sizeof(params.usermap));
    int entrycount = params.entrycount;
    int maxlen = params.maxlen;
    int offset = params.offset;
    int step = params.step;
    
    int i;
    vec_uint4 *results = malloc (4 * sizeof(vec_uint4));
    vec_uint4 temphash;
    int len;
    char *charset;
    int charsetlen;
    int confirmation_mask;
    char *combinations[4];
    combinations[0] = malloc ((MAX_PWLEN+1) * sizeof (char));
    combinations[1] = malloc ((MAX_PWLEN+1) * sizeof (char));
    combinations[2] = malloc ((MAX_PWLEN+1) * sizeof (char));
    combinations[3] = malloc ((MAX_PWLEN+1) * sizeof (char));

    CHUNK *md5chunk = (CHUNK *) malloc (entrycount * sizeof(CHUNK));

    len = 1;
    while (len <= maxlen){
        printf ("Thread proceeding to next password length (%u)\n", len);
        char *combination = malloc ((MAX_PWLEN + 1) * sizeof(char));
        charset = charsets[len-1];
        charsetlen = charsetlengths[len-1];
        
        memset (combinations[0], 0, MAX_PWLEN+1);
        memset (combinations[1], 0, MAX_PWLEN+1);
        memset (combinations[2], 0, MAX_PWLEN+1);
        memset (combinations[3], 0, MAX_PWLEN+1);

        memset (combination, 0, MAX_PWLEN + 1);
        memset (combination, charset[0], len);

        char **generator = setup_generator (charset, len);

        /* clock generator with offset for this thread */
        generate_next_combination (combination, len, generator, charset, charsetlen, offset);

        for (i = 0; i < entrycount; i++) {
            preprocess (len, usermap[i].salt, strlen(usermap[i].salt), &md5chunk[i]);
        }

        int combinations_remaining = 1;
        int k;
        while (__builtin_expect (combinations_remaining, LIKELY)) {    
            if (__builtin_expect (generate_next_combination
                        (combination, len, generator, charset, charsetlen, step) == 0, UNLIKELY)){
                combinations_remaining = 0;
            }
            strcpy(combinations[0], combination);
                
            if (__builtin_expect (generate_next_combination    
                        (combination, len, generator, charset, charsetlen, step) == 0, UNLIKELY)){
                combinations_remaining = 0;
            }
            strcpy(combinations[1], combination);

            if (__builtin_expect (generate_next_combination
                        (combination, len, generator, charset, charsetlen, step) == 0, UNLIKELY)){
                combinations_remaining = 0;
            }
            strcpy(combinations[2], combination);

            if (__builtin_expect (generate_next_combination
                        (combination, len, generator, charset, charsetlen, step) == 0, UNLIKELY)){
                combinations_remaining = 0;
            }
            strcpy(combinations[3], combination);

            for (i = 0; i < entrycount; i++){
                for (k = 0; k < len; k++) {
                    md5chunk[i].md5data[k/4] = (vec_uint4) spu_insert 
                        (combinations[0][k],(vec_uchar16) md5chunk[i].md5data[k/4], 3-k);
                
                    md5chunk[i].md5data[k/4] = (vec_uint4) spu_insert 
                        (combinations[1][k],(vec_uchar16) md5chunk[i].md5data[k/4], 7-k);

                    md5chunk[i].md5data[k/4] = (vec_uint4) spu_insert 
                        (combinations[2][k],(vec_uchar16) md5chunk[i].md5data[k/4], 11-k);

                    md5chunk[i].md5data[k/4] = (vec_uint4) spu_insert 
                        (combinations[3][k],(vec_uchar16) md5chunk[i].md5data[k/4], 15-k);
                }
            }
                
            for (i = 0; i < entrycount; i++){
   
                results = md5hash(&md5chunk[i], results);
   
                temphash = spu_cmpeq (results[0], usermap[i].hash);
                if (__builtin_expect (spu_extract (temphash,0) > 0, UNLIKELY)){
                    confirmation_mask = 0; 
                    confirmation_mask = spu_extract (temphash,1) & spu_extract (temphash,2) & spu_extract(temphash,3);
                    if (__builtin_expect (confirmation_mask != 0, UNLIKELY)) {
                        printf("Collision found: %s/%s\n", usermap[i].username, combinations[0]);
                    }
                }
                temphash = spu_cmpeq (results[1], usermap[i].hash);
                if (__builtin_expect (spu_extract (temphash,0) > 0, UNLIKELY)){
                    confirmation_mask = 0; 
                    confirmation_mask = spu_extract (temphash,1) & spu_extract (temphash,2) & spu_extract(temphash,3);
                    if (__builtin_expect (confirmation_mask != 0, UNLIKELY)) {
                        printf("Collision found: %s/%s\n", usermap[i].username, combinations[1]);
                    }
                }
                temphash = spu_cmpeq (results[2], usermap[i].hash);
                if (__builtin_expect (spu_extract (temphash,0) > 0, UNLIKELY)){
                    confirmation_mask = 0; 
                    confirmation_mask = spu_extract (temphash,1) & spu_extract (temphash,2) & spu_extract(temphash,3);
                    if (__builtin_expect (confirmation_mask != 0, UNLIKELY)) {
                        printf("Collision found: %s/%s\n", usermap[i].username, combinations[2]);
                    }
                }
                temphash = spu_cmpeq (results[3], usermap[i].hash);
                if (__builtin_expect (spu_extract (temphash,0) > 0, UNLIKELY)){
                    confirmation_mask = 0; 
                    confirmation_mask = spu_extract (temphash,1) & spu_extract (temphash,2) & spu_extract(temphash,3);
                    if (__builtin_expect (confirmation_mask != 0, UNLIKELY)) {
                        printf("Collision found: %s/%s\n", usermap[i].username, combinations[3]);
                    }
                }
            }
        }
        len++;
        free (generator);
        free (combination);

    }
    return 0;
}
