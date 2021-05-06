#ifndef DGC_MULTIALGO_H
#define DGC_MULTIALGO_H

#include <string>

enum {
    ALGO_SHA256D = 0,
    ALGO_SCRYPT  = 1,
    ALGO_X11     = 2,
    NUM_ALGOS 
};

enum
{
    BLOCK_VERSION_ALGO           = (7 << 9),
    BLOCK_VERSION_SHA256D        = (1 << 9),
    BLOCK_VERSION_X11            = (2 << 9),
    BLOCK_VERSION_SCRYPT         = 1,
};

inline int GetAlgo(int nVersion)
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

inline std::string GetAlgoName(int Algo)
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


#endif