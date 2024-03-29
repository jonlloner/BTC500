#include "test.h"

unsigned long long Timer::time()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

Timer::Timer()
{
    timer = time();
}

unsigned long long Timer::stop(int iterations)
{
    unsigned long long newTime = time();
    unsigned long long returnTime = (newTime - timer) * (1000000000 / iterations) / 1000;
    timer = newTime;
    return returnTime;
}

double Timer::stop()
{
    unsigned long long newTime = time();
    double returnTime = (double)(newTime - timer) / 1000;
    timer = newTime;
    return returnTime;
}

bool check(const unsigned char* array1, const unsigned char* array2, int length)
{
    for (int c = 0; c < length; c++)
        if (array1[c] != array2[c])
            return false;
    return true;
}

void test()
{
    std::cout << "[I] TESTING:" << std::endl;
    {
        unsigned char input[] = "KxFC1jmwwCoACiCAWZ3eXa96mBM6tb3TYzGmf6YwgdGWZgawvrtJ";
        unsigned char output[] = 
        { 
            0x80,
            0x1E, 0x99, 0x42, 0x3A, 0x4E, 0xD2, 0x76, 0x08, 0xA1, 0x5A, 0x26, 0x16, 0xA2, 0xB0, 0xE9, 0xE5, 
            0x2C, 0xED, 0x33, 0x0A, 0xC5, 0x30, 0xED, 0xCC, 0x32, 0xC8, 0xFF, 0xC6, 0xA5, 0x26, 0xAE, 0xDD, 
        };
        unsigned char output_[33];
        output_[0] = 0x80;
        base58_decode(input, output_);
        TEST("base58_decode", check(output, output_, 33), base58_decode(input, output_))
    }
    {
        unsigned char input[] = 
        { 
            0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x6D, 0x79, 0x20, 0x6E, 0x61, 0x6D, 0x65, 0x20, 0x69, 
            0x73, 0x20, 0x56, 0x69, 0x6B, 0x74, 0x6F, 0x72, 0x20, 0x59, 0x75, 0x73, 0x6B, 0x6F, 0x76, 0x79
        };
        unsigned char output[] = 
        { 
            0x88, 0x3C, 0x7C, 0x1A, 0x88, 0x1B, 0x45, 0xE1, 0xAE, 0x84, 0xF5, 0x82, 0x2C, 0xA2, 0x7F, 0xE5, 0x22, 0xE3, 0x29, 0xDE
        };
        unsigned char output_[20];
        ripemd160(input, output_);
        TEST("ripemd160", check(output, output_, 20), ripemd160(input, output_))
    }
    {
        bool result = true;
        unsigned char input32[] =
        {
            0x81, 0x47, 0x78, 0x6C, 0x4D, 0x15, 0x10, 0x63, 0x33, 0xBF, 0x27, 0x8D, 0x71, 0xDA, 0xDA, 0xF1,
            0x07, 0x9E, 0xF2, 0xD2, 0x44, 0x0A, 0x4D, 0xDE, 0x37, 0xD7, 0x47, 0xDE, 0xD5, 0x40, 0x35, 0x92,
            0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00
        };
        unsigned char output32[] =
        {
            0x50, 0x7A, 0x5B, 0x8D, 0xFE, 0xD0, 0xFC, 0x6F, 0xE8, 0x80, 0x17, 0x43, 0x72, 0x0C, 0xED, 0xEC,
            0x06, 0xAA, 0x5C, 0x6F, 0xCA, 0x72, 0xB0, 0x7C, 0x49, 0x96, 0x44, 0x92, 0xFB, 0x98, 0xA7, 0x14
        };
        unsigned char output32_[32];
        sha256(input32, output32_);
        result &= check(output32, output32_, 32);
        unsigned char input34[] =
        {
            0x80, 0x0C, 0x28, 0xFC, 0xA3, 0x86, 0xC7, 0xA2, 0x27, 0x60, 0x0B, 0x2F, 0xE5, 0x0B, 0x7C, 0xAE, 
            0x11, 0xEC, 0x86, 0xD3, 0xBF, 0x1F, 0xBE, 0x47, 0x1B, 0xE8, 0x98, 0x27, 0xE1, 0x9D, 0x72, 0xAA, 
            0x1D, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10
        };
        unsigned char output34[] =
        {
            0xB5, 0x04, 0xA8, 0x1B, 0x66, 0x92, 0x44, 0x82, 0xA2, 0x89, 0xCE, 0x57, 0x1A, 0x61, 0x4C, 0x37,
            0xB1, 0x22, 0xF4, 0x0C, 0xEF, 0xB0, 0xAB, 0xBA, 0x02, 0x27, 0xE1, 0x43, 0x15, 0x4C, 0x21, 0x53
        };
        unsigned char output34_[32];
        sha256(input34, output34_);
        result &= check(output34, output34_, 32);
        TEST("sha256", result, sha256(input32, output32_))
    }
    {
        unsigned char wif[] = "L5EZftvrYaSudiozVRzTqLcHLNDoVn7H5HSfM9BAN6tMJX8oTWz6";
        unsigned char extended_key[64] = { 0 };
        extended_key[0] = 0x80;
        extended_key[33] = 0x01;
        extended_key[34] = 0x80;
        extended_key[62] = 0x01;
        extended_key[63] = 0x10;
        unsigned char digest1[64] = { 0 };
        digest1[32] = 0x80;
        digest1[62] = 0x01;
        unsigned char digest2[32];
        base58_decode(wif, extended_key);
        sha256(extended_key, digest1);
        sha256(digest1, digest2);
        unsigned char checksum[] = { 0x66, 0x55, 0x7E, 0x53 };
        TEST("checksum", check(digest2, checksum, 4), ;)
    }
    std::cout << "[I] TESTING FINISHED SUCCESSFULLY" << std::endl;
}
