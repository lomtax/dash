#pragma once

#include "arith_uint256.h"
#include "scrypt.h"
#include "chain.h"

#define BEGIN(a)            ((char*)&(a))
#define END(a)              ((char*)&((&(a))[1]))

arith_uint256 GetPrevWorkForAlgo(const CBlockIndex& blockIndex, int algo);
int GetAlgoWorkFactor(const CBlockIndex& blockIndex);
arith_uint256 GetBlockWorkAdjusted(const CBlockIndex& blockIndex);
uint256 GetHash_Sha(int32_t nVersion, uint32_t nNonce);
uint256 GetHash_X11(int32_t nVersion, uint32_t nNonce);
uint256 GetHash_Scrypt(int32_t nVersion);
uint256 GetPowHash(int32_t nVersion, uint32_t nNonce, int algo);