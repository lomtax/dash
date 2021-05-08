#pragma once

#include "dgc_params.h"
#include "arith_uint256.h"
#include <chain.h>

unsigned int GetNextWorkRequiredV1(const CBlockIndex* pindexLast, const CBlockHeader *pblock, int algo);
const CBlockIndex* GetLastBlockIndexForAlgo(const CBlockIndex* pindex, int algo);
unsigned int GetNextWorkRequiredV2(const CBlockIndex* pindexLast, const CBlockHeader *pblock, int algo);
