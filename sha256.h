#pragma once

#include <immintrin.h>
#include <string>
#include <stdint.h>

#define ROR(x,n) _rotr(x, n)
#define S0(x) (ROR(x,2) ^ ROR(x,13) ^ ROR(x,22))
#define S1(x) (ROR(x,6) ^ ROR(x,11) ^ ROR(x,25))
#define s0(x) (ROR(x,7) ^ ROR(x,18) ^ (x >> 3))
#define s1(x) (ROR(x,17) ^ ROR(x,19) ^ (x >> 10))

#define Maj(x,y,z) ((x&y)^(x&z)^(y&z))
#define Ch(x,y,z) (z ^ (x & (y ^ z)))

#define Round(a, b, c, d, e, f, g, h, k, w) \
    t1 = h + S1(e) + Ch(e,f,g) + k + (w); \
    t2 = S0(a) + Maj(a,b,c); \
    d += t1; \
    h = t1 + t2;

#define WRITEBE32(ptr,x) *((unsigned *)(ptr)) = _byteswap_ulong(x)
#define READBE32(ptr) (unsigned)_byteswap_ulong(*(unsigned *)(ptr))

void sha256(unsigned char* input, unsigned char* output);
