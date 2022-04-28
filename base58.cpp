#include "base58.h"



void base58_decode(const unsigned char* input, unsigned char* output)
{
    unsigned char output_[38];
    int digitslen = 1;
    output_[0] = 0;
    for (int i = 0; i < 52; i++) 
    {
        unsigned carry = BASE58_MAP[input[i]];
        for (int j = 0; j < digitslen; j++) 
        {
            carry += output_[j] * 58;
            output_[j] = carry % 256;
            carry /= 256;
        }
        while (carry > 0)
        {
            output_[digitslen++] = carry % 256;
            carry /= 256;
        }
    }
    for (int i = 1; i < 33; i++)
        output[i] = output_[37 - i];
}
