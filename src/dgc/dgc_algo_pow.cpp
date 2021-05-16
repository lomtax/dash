#include "dgc_algo_pow.h"

arith_uint256 GetPrevWorkForAlgo(const CBlockIndex& blockIndex, int algo)
{
    CBlockIndex* pindex = blockIndex.pprev;
    while (pindex)
    {
        if (GetAlgo(pindex->nVersion) == algo)
        {
            return GetBlockProof(*pindex);
        }
        pindex = pindex->pprev;
    }

    return UintToArith256(ProofOfWorkLimit(algo));
}

int GetAlgoWorkFactor(const CBlockIndex& blockIndex)
{
    int factor = 0;

    if (blockIndex.nHeight < multiAlgoDiffChangeTarget)
    {
        factor = 1;
    }
    else
    {
        switch (GetAlgo(blockIndex.nVersion))
        {
            case ALGO_SHA256D : factor = 1; break;
            case ALGO_SCRYPT : factor = 1024 * 4; break;
            case ALGO_X11 : factor = 128 * 8; break;
            default : factor = 1;
        }
    }

    return factor;
}

arith_uint256 GetBlockWorkAdjusted(const CBlockIndex& blockIndex)
{
    arith_uint256 bnRes = arith_uint256();

    if (blockIndex.nHeight >= V3_FORK)
    {
        // Adjusted Block Work is the Sum of work of this block and the most recent work of one block of each algo
        arith_uint256 nBlockWork = GetBlockProof(blockIndex);
        int nAlgo = GetAlgo(blockIndex.nVersion);
        for (int algo = 0; algo < NUM_ALGOS; algo++)
        {
            if (algo != nAlgo)
            {
                nBlockWork += GetPrevWorkForAlgo(blockIndex, algo);
            }
        }
        bnRes = nBlockWork / NUM_ALGOS;
    }
    else
        bnRes = GetBlockProof(blockIndex) * GetAlgoWorkFactor(blockIndex);
    
    return bnRes;
}

uint256 ProofOfWorkLimit(int algo)
{
    uint256 pow = uint256();

    switch(algo)
    {
        case ALGO_SHA256D : pow = shaProofOfWorkLimit; break;
        case ALGO_SCRYPT : pow = scryptProofOfWorkLimit; break;
        case ALGO_X11 : pow = x11ProofOfWorkLimit; break;
    }

    return pow;
}