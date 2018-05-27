#include <md5.h>

/* (b and c) or ((not b) and d) */
void set_round_1 (vec_uint4 *a, 
        const vec_uint4 *b,
        const vec_uint4 *c,
        const vec_uint4 *d,
        const vec_uint4 *data,
        unsigned int s,
        const vec_uint4 *t) {
    *a = spu_add (*a, *t);
    *a = spu_add (*a, *data);
    *a = spu_add (*a, spu_sel(*d,*c,*b));
    *a = spu_rl  (*a, s);
    *a = spu_add (*a, *b);
} 

/* (d and b) or ((not d) and c) */
void set_round_2 (vec_uint4 *a, 
        const vec_uint4 *b,
        const vec_uint4 *c,
        const vec_uint4 *d,
        const vec_uint4 *data,
        unsigned int s,
        const vec_uint4 *t) {
    *a = spu_add (*a, *t);
    *a = spu_add (*a, *data);
    *a = spu_add (*a, spu_sel(*c,*b,*d));
    *a = spu_rl  (*a, s);
    *a = spu_add (*a, *b);
} 

/* b xor c xor d */
void set_round_3 (vec_uint4 *a, 
        const vec_uint4 *b,
        const vec_uint4 *c,
        const vec_uint4 *d,
        const vec_uint4 *data,
        unsigned int s,
        const vec_uint4 *t) {
    *a = spu_add (*a, *t);
    *a = spu_add (*a, *data);
    *a = spu_add (*a, spu_xor(*b, spu_xor(*c,*d)));
    *a = spu_rl  (*a, s);
    *a = spu_add (*a, *b);
} 

/* c xor (b or (not d)) */
void set_round_4 (vec_uint4 *a, 
        const vec_uint4 *b,
        const vec_uint4 *c,
        const vec_uint4 *d,
        const vec_uint4 *data,
        unsigned int s,
        const vec_uint4 *t) {
    *a = spu_add (*a, *t);
    *a = spu_add (*a, *data);
    *a = spu_add (*a, spu_xor(*c, spu_orc(*b,*d)));
    *a = spu_rl  (*a, s);
    *a = spu_add (*a, *b);
} 

inline vec_uint4 *md5hash (const CHUNK *input, vec_uint4 *output) {
   
    vec_uint4 A = { 0x67452301, 0x67452301, 0x67452301, 0x67452301 };
    vec_uint4 B = { 0xefcdab89, 0xefcdab89, 0xefcdab89, 0xefcdab89 };
    vec_uint4 C = { 0x98badcfe, 0x98badcfe, 0x98badcfe, 0x98badcfe };
    vec_uint4 D = { 0x10325476, 0x10325476, 0x10325476, 0x10325476 };

    vec_uint4 orig_a = A;
    vec_uint4 orig_b = B;
    vec_uint4 orig_c = C;
    vec_uint4 orig_d = D;

    vec_uint4 T1 =  { 0xd76aa478, 0xd76aa478, 0xd76aa478, 0xd76aa478 };
    vec_uint4 T2 =  { 0xe8c7b756, 0xe8c7b756, 0xe8c7b756, 0xe8c7b756 };
    vec_uint4 T3 =  { 0x242070db, 0x242070db, 0x242070db, 0x242070db };
    vec_uint4 T4 =  { 0xc1bdceee, 0xc1bdceee, 0xc1bdceee, 0xc1bdceee };
    vec_uint4 T5 =  { 0xf57c0faf, 0xf57c0faf, 0xf57c0faf, 0xf57c0faf };
    vec_uint4 T6 =  { 0x4787c62a, 0x4787c62a, 0x4787c62a, 0x4787c62a };
    vec_uint4 T7 =  { 0xa8304613, 0xa8304613, 0xa8304613, 0xa8304613 };
    vec_uint4 T8 =  { 0xfd469501, 0xfd469501, 0xfd469501, 0xfd469501 };
    vec_uint4 T9 =  { 0x698098d8, 0x698098d8, 0x698098d8, 0x698098d8 };
    vec_uint4 T10 = { 0x8b44f7af, 0x8b44f7af, 0x8b44f7af, 0x8b44f7af };
    vec_uint4 T11 = { 0xffff5bb1, 0xffff5bb1, 0xffff5bb1, 0xffff5bb1 };
    vec_uint4 T12 = { 0x895cd7be, 0x895cd7be, 0x895cd7be, 0x895cd7be };
    vec_uint4 T13 = { 0x6b901122, 0x6b901122, 0x6b901122, 0x6b901122 };
    vec_uint4 T14 = { 0xfd987193, 0xfd987193, 0xfd987193, 0xfd987193 };
    vec_uint4 T15 = { 0xa679438e, 0xa679438e, 0xa679438e, 0xa679438e };
    vec_uint4 T16 = { 0x49b40821, 0x49b40821, 0x49b40821, 0x49b40821 };
    vec_uint4 T17 = { 0xf61e2562, 0xf61e2562, 0xf61e2562, 0xf61e2562 };
    vec_uint4 T18 = { 0xc040b340, 0xc040b340, 0xc040b340, 0xc040b340 };
    vec_uint4 T19 = { 0x265e5a51, 0x265e5a51, 0x265e5a51, 0x265e5a51 };
    vec_uint4 T20 = { 0xe9b6c7aa, 0xe9b6c7aa, 0xe9b6c7aa, 0xe9b6c7aa };
    vec_uint4 T21 = { 0xd62f105d, 0xd62f105d, 0xd62f105d, 0xd62f105d };
    vec_uint4 T22 = { 0x02441453, 0x02441453, 0x02441453, 0x02441453 };
    vec_uint4 T23 = { 0xd8a1e681, 0xd8a1e681, 0xd8a1e681, 0xd8a1e681 };
    vec_uint4 T24 = { 0xe7d3fbc8, 0xe7d3fbc8, 0xe7d3fbc8, 0xe7d3fbc8 };
    vec_uint4 T25 = { 0x21e1cde6, 0x21e1cde6, 0x21e1cde6, 0x21e1cde6 };
    vec_uint4 T26 = { 0xc33707d6, 0xc33707d6, 0xc33707d6, 0xc33707d6 };
    vec_uint4 T27 = { 0xf4d50d87, 0xf4d50d87, 0xf4d50d87, 0xf4d50d87 };
    vec_uint4 T28 = { 0x455a14ed, 0x455a14ed, 0x455a14ed, 0x455a14ed };
    vec_uint4 T29 = { 0xa9e3e905, 0xa9e3e905, 0xa9e3e905, 0xa9e3e905 };
    vec_uint4 T30 = { 0xfcefa3f8, 0xfcefa3f8, 0xfcefa3f8, 0xfcefa3f8 };
    vec_uint4 T31 = { 0x676f02d9, 0x676f02d9, 0x676f02d9, 0x676f02d9 };
    vec_uint4 T32 = { 0x8d2a4c8a, 0x8d2a4c8a, 0x8d2a4c8a, 0x8d2a4c8a };
    vec_uint4 T33 = { 0xfffa3942, 0xfffa3942, 0xfffa3942, 0xfffa3942 };
    vec_uint4 T34 = { 0x8771f681, 0x8771f681, 0x8771f681, 0x8771f681 };
    vec_uint4 T35 = { 0x6d9d6122, 0x6d9d6122, 0x6d9d6122, 0x6d9d6122 };
    vec_uint4 T36 = { 0xfde5380c, 0xfde5380c, 0xfde5380c, 0xfde5380c };
    vec_uint4 T37 = { 0xa4beea44, 0xa4beea44, 0xa4beea44, 0xa4beea44 };
    vec_uint4 T38 = { 0x4bdecfa9, 0x4bdecfa9, 0x4bdecfa9, 0x4bdecfa9 };
    vec_uint4 T39 = { 0xf6bb4b60, 0xf6bb4b60, 0xf6bb4b60, 0xf6bb4b60 };
    vec_uint4 T40 = { 0xbebfbc70, 0xbebfbc70, 0xbebfbc70, 0xbebfbc70 };
    vec_uint4 T41 = { 0x289b7ec6, 0x289b7ec6, 0x289b7ec6, 0x289b7ec6 };
    vec_uint4 T42 = { 0xeaa127fa, 0xeaa127fa, 0xeaa127fa, 0xeaa127fa };
    vec_uint4 T43 = { 0xd4ef3085, 0xd4ef3085, 0xd4ef3085, 0xd4ef3085 };
    vec_uint4 T44 = { 0x04881d05, 0x04881d05, 0x04881d05, 0x04881d05 };
    vec_uint4 T45 = { 0xd9d4d039, 0xd9d4d039, 0xd9d4d039, 0xd9d4d039 };
    vec_uint4 T46 = { 0xe6db99e5, 0xe6db99e5, 0xe6db99e5, 0xe6db99e5 };
    vec_uint4 T47 = { 0x1fa27cf8, 0x1fa27cf8, 0x1fa27cf8, 0x1fa27cf8 };
    vec_uint4 T48 = { 0xc4ac5665, 0xc4ac5665, 0xc4ac5665, 0xc4ac5665 };
    vec_uint4 T49 = { 0xf4292244, 0xf4292244, 0xf4292244, 0xf4292244 };
    vec_uint4 T50 = { 0x432aff97, 0x432aff97, 0x432aff97, 0x432aff97 };
    vec_uint4 T51 = { 0xab9423a7, 0xab9423a7, 0xab9423a7, 0xab9423a7 };
    vec_uint4 T52 = { 0xfc93a039, 0xfc93a039, 0xfc93a039, 0xfc93a039 };
    vec_uint4 T53 = { 0x655b59c3, 0x655b59c3, 0x655b59c3, 0x655b59c3 };
    vec_uint4 T54 = { 0x8f0ccc92, 0x8f0ccc92, 0x8f0ccc92, 0x8f0ccc92 };
    vec_uint4 T55 = { 0xffeff47d, 0xffeff47d, 0xffeff47d, 0xffeff47d };
    vec_uint4 T56 = { 0x85845dd1, 0x85845dd1, 0x85845dd1, 0x85845dd1 };
    vec_uint4 T57 = { 0x6fa87e4f, 0x6fa87e4f, 0x6fa87e4f, 0x6fa87e4f };
    vec_uint4 T58 = { 0xfe2ce6e0, 0xfe2ce6e0, 0xfe2ce6e0, 0xfe2ce6e0 };
    vec_uint4 T59 = { 0xa3014314, 0xa3014314, 0xa3014314, 0xa3014314 };
    vec_uint4 T60 = { 0x4e0811a1, 0x4e0811a1, 0x4e0811a1, 0x4e0811a1 };
    vec_uint4 T61 = { 0xf7537e82, 0xf7537e82, 0xf7537e82, 0xf7537e82 };
    vec_uint4 T62 = { 0xbd3af235, 0xbd3af235, 0xbd3af235, 0xbd3af235 };
    vec_uint4 T63 = { 0x2ad7d2bb, 0x2ad7d2bb, 0x2ad7d2bb, 0x2ad7d2bb };
    vec_uint4 T64 = { 0xeb86d391, 0xeb86d391, 0xeb86d391, 0xeb86d391 };
 
    set_round_1 (&A, &B, &C, &D,  &input->md5data[0],   7,  &T1);
    set_round_1 (&D, &A, &B, &C,  &input->md5data[1],  12,  &T2);
    set_round_1 (&C, &D, &A, &B,  &input->md5data[2],  17,  &T3);
    set_round_1 (&B, &C, &D, &A,  &input->md5data[3],  22,  &T4);
    set_round_1 (&A, &B, &C, &D,  &input->md5data[4],   7,  &T5);
    set_round_1 (&D, &A, &B, &C,  &input->md5data[5],  12,  &T6);
    set_round_1 (&C, &D, &A, &B,  &input->md5data[6],  17,  &T7);
    set_round_1 (&B, &C, &D, &A,  &input->md5data[7],  22,  &T8);
    set_round_1 (&A, &B, &C, &D,  &input->md5data[8],   7,  &T9);
    set_round_1 (&D, &A, &B, &C,  &input->md5data[9],  12, &T10);
    set_round_1 (&C, &D, &A, &B,  &input->md5data[10], 17, &T11);
    set_round_1 (&B, &C, &D, &A,  &input->md5data[11], 22, &T12);
    set_round_1 (&A, &B, &C, &D,  &input->md5data[12],  7, &T13);
    set_round_1 (&D, &A, &B, &C,  &input->md5data[13], 12, &T14);
    set_round_1 (&C, &D, &A, &B,  &input->md5data[14], 17, &T15);
    set_round_1 (&B, &C, &D, &A,  &input->md5data[15], 22, &T16);

    set_round_2 (&A, &B, &C, &D,  &input->md5data[1],  5, &T17);
    set_round_2 (&D, &A, &B, &C,  &input->md5data[6],  9, &T18);
    set_round_2 (&C, &D, &A, &B, &input->md5data[11], 14, &T19);
    set_round_2 (&B, &C, &D, &A,  &input->md5data[0], 20, &T20);
    set_round_2 (&A, &B, &C, &D,  &input->md5data[5],  5, &T21);
    set_round_2 (&D, &A, &B, &C, &input->md5data[10],  9, &T22);
    set_round_2 (&C, &D, &A, &B, &input->md5data[15], 14, &T23);
    set_round_2 (&B, &C, &D, &A,  &input->md5data[4], 20, &T24);
    set_round_2 (&A, &B, &C, &D,  &input->md5data[9],  5, &T25);
    set_round_2 (&D, &A, &B, &C, &input->md5data[14],  9, &T26);
    set_round_2 (&C, &D, &A, &B,  &input->md5data[3], 14, &T27);
    set_round_2 (&B, &C, &D, &A,  &input->md5data[8], 20, &T28);
    set_round_2 (&A, &B, &C, &D, &input->md5data[13],  5, &T29);
    set_round_2 (&D, &A, &B, &C,  &input->md5data[2],  9, &T30);
    set_round_2 (&C, &D, &A, &B,  &input->md5data[7], 14, &T31);
    set_round_2 (&B, &C, &D, &A, &input->md5data[12], 20, &T32);
 
    set_round_3 (&A, &B, &C, &D,  &input->md5data[5],  4, &T33);
    set_round_3 (&D, &A, &B, &C,  &input->md5data[8], 11, &T34);
    set_round_3 (&C, &D, &A, &B, &input->md5data[11], 16, &T35);
    set_round_3 (&B, &C, &D, &A, &input->md5data[14], 23, &T36);
    set_round_3 (&A, &B, &C, &D,  &input->md5data[1],  4, &T37);
    set_round_3 (&D, &A, &B, &C,  &input->md5data[4], 11, &T38);
    set_round_3 (&C, &D, &A, &B,  &input->md5data[7], 16, &T39);
    set_round_3 (&B, &C, &D, &A, &input->md5data[10], 23, &T40);
    set_round_3 (&A, &B, &C, &D, &input->md5data[13],  4, &T41);
    set_round_3 (&D, &A, &B, &C,  &input->md5data[0], 11, &T42);
    set_round_3 (&C, &D, &A, &B,  &input->md5data[3], 16, &T43);
    set_round_3 (&B, &C, &D, &A,  &input->md5data[6], 23, &T44);
    set_round_3 (&A, &B, &C, &D,  &input->md5data[9],  4, &T45);
    set_round_3 (&D, &A, &B, &C, &input->md5data[12], 11, &T46);
    set_round_3 (&C, &D, &A, &B, &input->md5data[15], 16, &T47);
    set_round_3 (&B, &C, &D, &A,  &input->md5data[2], 23, &T48);

    set_round_4 (&A, &B, &C, &D,  &input->md5data[0],  6, &T49);
    set_round_4 (&D, &A, &B, &C,  &input->md5data[7], 10, &T50);
    set_round_4 (&C, &D, &A, &B, &input->md5data[14], 15, &T51);
    set_round_4 (&B, &C, &D, &A,  &input->md5data[5], 21, &T52);
    set_round_4 (&A, &B, &C, &D, &input->md5data[12],  6, &T53);
    set_round_4 (&D, &A, &B, &C,  &input->md5data[3], 10, &T54);
    set_round_4 (&C, &D, &A, &B, &input->md5data[10], 15, &T55);
    set_round_4 (&B, &C, &D, &A,  &input->md5data[1], 21, &T56);
    set_round_4 (&A, &B, &C, &D,  &input->md5data[8],  6, &T57);
    set_round_4 (&D, &A, &B, &C, &input->md5data[15], 10, &T58);
    set_round_4 (&C, &D, &A, &B,  &input->md5data[6], 15, &T59);
    set_round_4 (&B, &C, &D, &A, &input->md5data[13], 21, &T60);
    set_round_4 (&A, &B, &C, &D,  &input->md5data[4],  6, &T61);
    set_round_4 (&D, &A, &B, &C, &input->md5data[11], 10, &T62);
    set_round_4 (&C, &D, &A, &B,  &input->md5data[2], 15, &T63);
    set_round_4 (&B, &C, &D, &A,  &input->md5data[9], 21, &T64);
       
    A = spu_add (A, orig_a);
    B = spu_add (B, orig_b);
    C = spu_add (C, orig_c);
    D = spu_add (D, orig_d);

    output[0] = (vec_uint4) { spu_extract(A,0), spu_extract(B,0), spu_extract(C,0), spu_extract(D,0) };
    output[1] = (vec_uint4) { spu_extract(A,1), spu_extract(B,1), spu_extract(C,1), spu_extract(D,1) };
    output[2] = (vec_uint4) { spu_extract(A,2), spu_extract(B,2), spu_extract(C,2), spu_extract(D,2) };
    output[3] = (vec_uint4) { spu_extract(A,3), spu_extract(B,3), spu_extract(C,3), spu_extract(D,3) };

    return output;
}

