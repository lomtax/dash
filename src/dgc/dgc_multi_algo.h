#pragma once

#include <string>

enum 
{
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

int GetAlgo(int nVersion);
std::string GetAlgoName(int Algo);

