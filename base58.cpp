#include "base58.h"

/*
int base58_decode(const unsigned char* input, int length, unsigned char* output)
{
    output[0] = 0;
    int resultlen = 1;
    for (int i = 0; i < length; i++)
    {
        unsigned int carry = BASE58_MAP[input[i]];
        if (carry == -1) return 0;
        for (int j = 0; j < resultlen; j++)
        {
            carry += (unsigned int)(output[j]) * 58;
            output[j] = (unsigned char)(carry & 0xff);
            carry >>= 8;
        }
        while (carry > 0)
        {
            output[resultlen++] = carry & 0xff;
            carry >>= 8;
        }
    }

    for (int i = 0; i < length && input[i] == '1'; i++)
        output[resultlen++] = 0;
    for (int i = resultlen - 1, z = (resultlen >> 1) + (resultlen & 1); i >= z; i--)
    {
        int k = output[i];
        output[i] = output[resultlen - i - 1];
        output[resultlen - i - 1] = k;
    }
    return resultlen;
}
*/

#include <algorithm>
#include <string.h>
#include <cstdint>
#include <iostream>

/** All alphanumeric characters except for "0", "I", "O", and "l" */
static const char* pszBase58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

static const unsigned b58digits_map[] = {
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1, 0, 1, 2, 3, 4, 5, 6,  7, 8,-1,-1,-1,-1,-1,-1,
    -1, 9,10,11,12,13,14,15, 16,-1,17,18,19,20,21,-1,
    22,23,24,25,26,27,28,29, 30,31,32,-1,-1,-1,-1,-1,
    -1,33,34,35,36,37,38,39, 40,41,42,43,-1,44,45,46,
    47,48,49,50,51,52,53,54, 55,56,57,-1,-1,-1,-1,-1,
};

void base58_decode(const unsigned char* input, unsigned char* output)
{
    int digitslen = 1;
    output[37] = 0;
    for (int i = 0; i < 52; i++) 
    {
        unsigned carry = b58digits_map[input[i]];
        for (int j = 0; j < digitslen; j++) 
        {
            carry += output[37 - j] * 58;
            output[37 - j] = carry % 256;
            carry /= 256;
        }
        while (carry > 0)
        {
            output[37 - digitslen++] = carry % 256;
            carry /= 256;
        }
    }
}
