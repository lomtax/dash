#pragma once

#include <string>
#include "uint256.h"

#define BEGIN(a)            ((char*)&(a))
#define END(a)              ((char*)&((&(a))[1]))

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
int GetAlgo(std::string name);
std::string GetAlgoName(int Algo);
uint256 GetHash_Sha(const int32_t& nVersion, const uint32_t& nNonce);
uint256 GetHash_X11(const int32_t& nVersion, const uint32_t& nNonce);
uint256 GetHash_Scrypt(const int32_t& nVersion);
uint256 GetPowHash(const int32_t& nVersion, const uint32_t& nNonce, int algo);
