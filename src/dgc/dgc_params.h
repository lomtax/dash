#pragma once

#include "chain.h"

/** DGC V3 Hard Fork Block */
static const int V3_FORK = 1028000;
static const int MAX_BLOCK_ALGO_COUNT = 3;

/* Other DGC Fork Blocks */
static const int DIFF_SWITCH_HEIGHT = 476280;
static const int INFLATION_FIX_HEIGHT = 523800;
static const int DIFF2_SWITCH_HEIGHT = 625800;

static const int64_t multiAlgoDiffChangeTarget = 10; // block where multi-algo work weighting starts 145000

static const int64_t nTargetTimespan = 108 * 40; // digitalcoin: 108 blocks (72 mins) [OLD WAS 6*60*3*20]
static const int64_t nTargetSpacing = 1 * 40; // digitalcoin: 40 seconds
static const int64_t nInterval = nTargetTimespan / nTargetSpacing;

//MultiAlgo Target updates
static const int64_t multiAlgoTargetTimespan = 120; // 2 minutes (NUM_ALGOS(3) * 40 seconds)
static const int64_t multiAlgoTargetSpacing = 120; // 2 minutes (NUM_ALGOS * 30 seconds)
static const int64_t multiAlgoInterval = 1; // retargets every blocks

static const int64_t nAveragingInterval = 10; // 10 blocks
static const int64_t nAveragingTargetTimespan = nAveragingInterval * multiAlgoTargetSpacing; // 20 minutes

static const int64_t nMaxAdjustDown = 40; // 40% adjustment down
static const int64_t nMaxAdjustUp = 20; // 20% adjustment up

static const int64_t nTargetTimespanAdjDown = multiAlgoTargetTimespan * (100 + nMaxAdjustDown) / 100;
static const int64_t nLocalDifficultyAdjustment = 40; // 40% down, 20% up

static const int64_t nMinActualTimespan = nAveragingTargetTimespan * (100 - nMaxAdjustUp) / 100;
static const int64_t nMaxActualTimespan = nAveragingTargetTimespan * (100 + nMaxAdjustDown) / 100;