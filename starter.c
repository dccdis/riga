#include <starter.h>

void *brute (void *data) {
    CRACK_PARAMS *wrapper = (CRACK_PARAMS *) data;
    CRACK_PARAMS params __attribute__((aligned(16)));

    memcpy (params.usermap, wrapper->usermap, sizeof(wrapper->usermap));
    memcpy (params.charsets, wrapper->charsets, sizeof(wrapper->charsets));
    memcpy (params.charsetlengths, wrapper->charsetlengths, sizeof(wrapper->charsetlengths));
    params.entrycount = wrapper->entrycount;
    params.maxlen = wrapper->maxlen;
    params.offset = wrapper->offset;
    params.step = wrapper->step;

    unsigned int entry_point = SPE_DEFAULT_ENTRY;
    spe_context_ptr_t context;

    context = spe_context_create (SPE_EVENTS_ENABLE|SPE_MAP_PS, NULL);
    spe_program_load (context, &engine);

    spe_context_run (context, &entry_point, 0, &params, NULL, NULL);

    pthread_exit (NULL);
}

void starter(const USER **usermap, const CONFIG cfg){

    pthread_t threads[cfg.num_threads];
    long t;

    int i;
    CRACK_PARAMS params[cfg.num_threads] __attribute__((aligned(16)));
    for (t = 0; t < cfg.num_threads; t++){
        for (i = 0; i < cfg.entrycount; i++) {
            memcpy (params[t].usermap[i].username, usermap[i]->username, sizeof(usermap[i]->username));
            memcpy (params[t].usermap[i].salt, usermap[i]->salt, sizeof(usermap[i]->salt));
            memcpy (params[t].usermap[i].hash, usermap[i]->hash, sizeof(usermap[i]->hash));
        }
        for (i = 0; i < 12; i++){
            memcpy (params[t].charsets[i], cfg.charsets[i], sizeof(cfg.charsets[i]));
            params[t].charsetlengths[i] = cfg.charsetlengths[i];
        }
        params[t].entrycount = cfg.entrycount;
        params[t].maxlen = cfg.maxlen;
        params[t].offset = t;
        params[t].step = cfg.num_threads;
    }
    
    for (t = 0; t < cfg.num_threads; t++){
        pthread_create (&threads[t], NULL, brute, (void *) &params[t]);
    }

    for (t = 0; t < cfg.num_threads; t++){
        pthread_join (threads[t], NULL);
    }

}
