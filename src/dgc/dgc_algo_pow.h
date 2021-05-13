#pragma once

#include "arith_uint256.h"
#include "chain.h"
#include "chainparams.h"


arith_uint256 GetPrevWorkForAlgo(const CBlockIndex& blockIndex, int algo);
int GetAlgoWorkFactor(const CBlockIndex& blockIndex);
arith_uint256 GetBlockWorkAdjusted(const CBlockIndex& blockIndex);
