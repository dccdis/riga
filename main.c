#include <starter.h>
#include <init.h>
#include <unistd.h>

void usage(char *executable){
    printf ("Usage: %s hashfile [-t threads] [-m maxlen] [-c charsetfile]\n\n", executable);
    printf ("\t hashfile:        file of with 1...n entries of format username|salt|md5hash\n");
    printf ("\t -t threads:      number of SPE threads to run on, defaults to %d\n", defaults.num_threads);
    printf ("\t -m maxlen:       the maximum length of passwords to search through, defaults to %d\n", defaults.maxlen);
    printf ("\t -c charsetfile:  file with charsets to use for different password lengths (see example)\n");
    printf ("\n");
}

int main (int argc, char **argv){
    
    int option;
    char *chrset_filename = NULL;
    char *hash_filename = NULL;
    CONFIG cfg = defaults;

    if (argc < 2) {
        usage (argv[0]);
        exit (EXIT_FAILURE);
    }

    while ((option = getopt (argc, argv, "t:m:c:")) != -1){
        switch (option) {
            case 't': cfg.num_threads = atoi (optarg); break;
            case 'm': cfg.maxlen = atoi (optarg); break;
            case 'c': chrset_filename = optarg; break;
            default: usage (argv[0]);
        }
    }

    hash_filename = argv[argc - 1];

    int input_nok = 0;
    if (hash_filename == NULL) input_nok = 1;
    if (cfg.maxlen > 12 || cfg.maxlen < 1) input_nok = 1;
    if (cfg.num_threads < 1) input_nok = 1;
    if (input_nok) {
        usage (argv[0]);
        exit (EXIT_FAILURE);
    }


    USER **usermap;
    usermap = parse_hashfile (hash_filename);

    if (chrset_filename != NULL){
        parse_chrsets (chrset_filename, &cfg);
    }

    cfg.entrycount = count_entries (usermap);
    
    starter (usermap, cfg);

    int i;
    for (i = 0; i < cfg.entrycount; i++){
        free (usermap[i]);
    }

    return 0;
}
