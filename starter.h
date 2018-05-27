#ifndef _STARTER_H_
#define _STARTER_H_

#include <libspe2.h>
#include <init.h>
#include <pthread.h>

extern spe_program_handle_t engine;

void *brute (void *data);

void starter(const USER **usermap, const CONFIG cfg);

#endif

