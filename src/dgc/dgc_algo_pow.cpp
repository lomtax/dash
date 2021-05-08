#include "dgc_algo_pow.h"
#include "chain.h"

arith_uint256 GetPrevWorkForAlgo(const CBlockIndex& blockIndex, int algo)
{
    arith_uint256 nWork;
    CBlockIndex* pindex = blockIndex.pprev;
    while (pindex)
    {
        if (pindex->GetAlgo() == algo)
        {
            return GetBlockProof(*pindex);
        }
        pindex = pindex->pprev;
    }

    return UintToArith256(Params().ProofOfWorkLimit(algo));
}

int GetAlgoWorkFactor(const CBlockIndex& blockIndex)
{
    if (!TestNet() && (blockIndex.nHeight < multiAlgoDiffChangeTarget))
    {
        return 1;
    }
    if (TestNet() && (blockIndex.nHeight < 100))
    {
        return 1;
    }
    switch (GetAlgo(blockIndex.nVersion))
    {
        case ALGO_SHA256D:
            return 1;
        case ALGO_SCRYPT:
            return 1024 * 4;
        case ALGO_X11:
            return 128 * 8;
        default:
            return 1;
    }
}

arith_uint256 GetBlockWorkAdjusted(const CBlockIndex& blockIndex)
{
    arith_uint256 bnRes;
    if ((TestNet() && (blockIndex.nHeight >= 1)) || (!TestNet() && blockIndex.nHeight >= V3_FORK))
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
    {
        bnRes = GetBlockProof(blockIndex) * GetAlgoWorkFactor(blockIndex);
    }
    return bnRes;
}

uint256 GetHash_Sha(int32_t nVersion, uint32_t nNonce)
{
    return Hash(BEGIN(nVersion), END(nNonce));
}

uint256 GetHash_X11(int32_t nVersion, uint32_t nNonce)
{
    return HashX11(BEGIN(nVersion), END(nNonce));
}

uint256 GetHash_Scrypt(int32_t nVersion)
{
    uint256 result;
    scrypt_1024_1_1_256(BEGIN(nVersion), BEGIN(result));
    return result;
}

uint256 GetPowHash(int32_t nVersion, uint32_t nNonce, int algo)
{
    uint256 result = GetHash_Sha(nVersion, nNonce);

    switch (algo)
    {
        case ALGO_SHA256D : result = GetHash_Sha(nVersion, nNonce); break;
        case ALGO_SCRYPT: result = GetHash_Scrypt(nVersion); break;
        case ALGO_X11: result = GetHash_X11(nVersion, nNonce); break;
    }

    return result;
}