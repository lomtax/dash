#include "dgc_multi_algo.h"

int GetAlgo(int nVersion)
{
    int algo = ALGO_SCRYPT;
    int masked = nVersion & BLOCK_VERSION_ALGO;

    switch (masked)
    {
        case 1:
            algo = ALGO_SCRYPT; 
            break;
        case BLOCK_VERSION_SHA256D:
            algo = ALGO_SHA256D; 
            break;
        case BLOCK_VERSION_X11:
            algo = ALGO_X11; 
            break;
            default : 
            break;
    }
    return algo;
}

std::string GetAlgoName(int Algo)
{
    switch (Algo)
    {
        case ALGO_SHA256D:
            return std::string("sha256d");
        case ALGO_SCRYPT:
            return std::string("scrypt");
        case ALGO_X11:
            return std::string("x11");
    }
    return std::string("unknown");       
}