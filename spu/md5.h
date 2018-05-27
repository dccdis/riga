#ifndef _MD5_H_
#define _MD5_H_

#include <spu_intrinsics.h>
#include <vec_types.h>

typedef struct chunk {
    vec_uint4 md5data[16];
} CHUNK;

vec_uint4 *md5hash (const CHUNK *input, vec_uint4 *output);

#endif
