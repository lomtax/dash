#pragma once

#include "dgc_params.h"
#include "dgc_multi_algo.h"
#include "arith_uint256.h"
#include "chain.h"

arith_uint256 GetPrevWorkForAlgo(const CBlockIndex& blockIndex, int algo);
int GetAlgoWorkFactor(const CBlockIndex& blockIndex);
arith_uint256 GetBlockWorkAdjusted(const CBlockIndex& blockIndex);
uint256 ProofOfWorkLimit(int algo);