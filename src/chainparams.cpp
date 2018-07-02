// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"
#include "uint256.h"
static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Digitalcoin, A Currency for a Digital Age";
    const CScript genesisOutputScript = CScript() << ParseHex("04a5814813115273a109cff99907ba4a05d951873dae7acb6c973d0c9e7c88911a3dbc9aa600deac241b91707e7b4ffb30ad91c8e56e695a1ddf318592988afe0a") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 4730400; // Note: actual number of blocks per calendar year with DGW v3 is ~200700 (for example 449750 - 249050)
        consensus.nMasternodePaymentsStartBlock = 3000000; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 0;//DGC : NEVER USED ANYMORE // actual historical value
        consensus.nMasternodePaymentsIncreasePeriod = 0;//DGC : NEVER USED ANYMORE  // 17280 - actual historical value
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 3000000; // 328008; // actual historical value
        consensus.nBudgetPaymentsCycleBlocks = 50000;//16616; // ~(60*24*30)/2.6, actual number of blocks per month is 200700 / 12 = 16725
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 60*60*24;//NOT USED
        consensus.nSuperblockCycle = 50000;//16616; // ~(60*24*30)/2.6, actual number of blocks per month is 200700 / 12 = 16725
        consensus.nSuperblockStartBlock = 3250000; // 614820; // The block at which 12.1 goes live (end of final 12.0 budget cycle)
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 10;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0x000007d91d1254d60e2dd1ae580383070a4ddffa4c64c2eeb4a2f9ecc0414343");
        consensus.powLimit = uint256S("0000100000000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 108*40;//24 * 60 * 60; // Digitalcoin: 1 day
        consensus.nPowTargetSpacing = 40;//2.5 * 60; // Digitalcoin: 2.5 minutes
        consensus.bnProofOfWorkLimit[ALGO_SHA256D] = CBigNum(uint256S("0000100000000000000000000000000000000000000000000000000000000000"));
        consensus.bnProofOfWorkLimit[ALGO_SCRYPT]  = CBigNum(uint256S("0000100000000000000000000000000000000000000000000000000000000000"));
        consensus.bnProofOfWorkLimit[ALGO_X11] = CBigNum(uint256S("0000100000000000000000000000000000000000000000000000000000000000"));
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 15200;
        consensus.nPowDGWHeight = 34140;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1486252800; // Feb 5th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517788800; // Feb 5th, 2018

        // Deployment of DIP0001 
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 3;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1508025600; // Oct 15th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1539561600; // Oct 15th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 4032;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 3226; // 80% of 4032

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000100000000000000000000000000000000000000001117564a5d286356cb6"); // 0

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000000000000a246455a317fe838094c0aff7f43f0d7e5a9d8110ba73680d7"); // 0

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0xb6;
        pchMessageStart[3] = 0xdb;
        vAlertPubKey = ParseHex("04016c44069c3152982413d3ba3bf262a3a4d3ddad859ba78e0d744f5c67c2205d2aa2122e6c62b6310dad2d1e2f7e39028455ff1dbb26511c60fc96c8b4560c43");
        nDefaultPort = 7999;//9999 replace HERE
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDelayGetHeadersTime = 24 * 60 * 60;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1367867384, 672176, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
  
        assert(consensus.hashGenesisBlock == uint256S("0x5e039e1ca1dbf128973bf6cff98169e40a1b194c3b91463ab74956f413b2f9c8"));
        assert(genesis.hashMerkleRoot == uint256S("0xecb2c595fff9f2364152c32027737007c5a4c60ec960cf93754b0211bc2a1501"));

        vSeeds.push_back(CDNSSeedData("bitnode baritus", "212.24.97.214")); 
        vSeeds.push_back(CDNSSeedData("brekki node1", "dgcnode1.brekki.fr")); 
        vSeeds.push_back(CDNSSeedData("brekki node2", "dgcnode2.brekki.fr")); 
        vSeeds.push_back(CDNSSeedData("MN_1" , "149.28.67.135"));
        vSeeds.push_back(CDNSSeedData("MN_2" , "136.144.202.230"));
        vSeeds.push_back(CDNSSeedData("MN_3" , "136.144.202.234"));
        vSeeds.push_back(CDNSSeedData("MN_4" , "45.32.243.212"));
        vSeeds.push_back(CDNSSeedData("MN_5" , "45.76.255.143"));
        vSeeds.push_back(CDNSSeedData("MN_6" , "104.156.254.26"));
        vSeeds.push_back(CDNSSeedData("MN_7" , "82.165.30.169"));
        vSeeds.push_back(CDNSSeedData("MN_8" , "149.28.199.96"));
        vSeeds.push_back(CDNSSeedData("MN_9" , "142.134.238.189"));
        vSeeds.push_back(CDNSSeedData("MN_10" , "136.144.202.232"));
        vSeeds.push_back(CDNSSeedData("MN_11" , "136.144.202.237"));
        vSeeds.push_back(CDNSSeedData("MN_12" , "195.201.174.153"));
        vSeeds.push_back(CDNSSeedData("MN_13" , "94.130.70.219"));
        vSeeds.push_back(CDNSSeedData("MN_14" , "23.95.0.21"));
        vSeeds.push_back(CDNSSeedData("MN_15" , "23.226.138.218"));
        vSeeds.push_back(CDNSSeedData("MN_16" , "209.250.243.147"));
        vSeeds.push_back(CDNSSeedData("MN_17" , "45.76.247.104"));
        vSeeds.push_back(CDNSSeedData("MN_18" , "138.201.205.233"));
        vSeeds.push_back(CDNSSeedData("MN_19" , "136.144.202.235"));
        vSeeds.push_back(CDNSSeedData("MN_20" , "144.202.68.185"));
        vSeeds.push_back(CDNSSeedData("MN_21" , "45.76.236.92"));
        vSeeds.push_back(CDNSSeedData("MN_22" , "46.150.48.119"));
        vSeeds.push_back(CDNSSeedData("MN_23" , "149.28.72.163"));
        vSeeds.push_back(CDNSSeedData("MN_24" , "45.63.86.239"));
        vSeeds.push_back(CDNSSeedData("MN_25" , "104.207.156.207"));
        vSeeds.push_back(CDNSSeedData("MN_26" , "81.167.109.85"));
        vSeeds.push_back(CDNSSeedData("MN_27" , "80.240.21.233"));
        vSeeds.push_back(CDNSSeedData("MN_28" , "136.144.202.236"));
        vSeeds.push_back(CDNSSeedData("MN_29" , "89.40.4.49"));
        vSeeds.push_back(CDNSSeedData("MN_30" , "149.28.114.92"));

        // Digitalcoin addresses start with 'X'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,30);
        // Digitalcoin script addresses start with '7'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        // Digitalcoin private keys start with '7' or 'X'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        // Digitalcoin BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Digitalcoin BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        // Digitalcoin BIP44 coin type is '5'
        nExtCoinType = 5;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "0419fb74e341a19c66e27a3ef642f6ecaa8eaad487d069a322a5b0c848d339ed6aa2f5d976c12efa934b53bb2daf5999dd2bfd08fb7788f9a4cd4c09e28573bb0d";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
        ( 0,       uint256S("0x5e039e1ca1dbf128973bf6cff98169e40a1b194c3b91463ab74956f413b2f9c8"))
        ( 1,       uint256S("0x45b2559dbe5e5772498e4170f3f1561448179fa90dd349e60e891766878dea2e"))
        ( 20,      uint256S("0x59436aad777d285d52a3fb61b4176c7ca30a1254b7fc1480b2c7320913953fe3"))
        ( 500,     uint256S("0xad48f7f697107bc2235779cd3efb56d522ac557c5f7824b81cba610ed2bb90d4"))
        ( 1000,    uint256S("0x6970173f5fe0686a168e67e37d858c2fb090e67dfd26d9bf44c356aa2cda100c"))
        ( 3500,    uint256S("0x6e92c6cf634c39149d07f022cf13e87b91713d1e7a5d9abc2b5f3646a4027838"))
        ( 5000,    uint256S("0x45c1a2e86b6fbeb0e0ee3d4662e62cba0677571b8956c1846b19ee7f10648028"))
        ( 10000,   uint256S("0x73e21da5458d9e3b321228f1b319e217fd4c82da629bc293981a79077e28b869"))
        ( 15000,   uint256S("0x5c456fa7873afec904d965194d27fc7127295d08a6910d9ac255bacfc19ab013"))
        ( 22222,   uint256S("0x7a58919a24c189f8c286413381e6ed7224c90a4181a7f7cd098825cc75ddec27"))
        ( 30000,   uint256S("0xcf2f900a91d869974e309451c28ae368690f42b773618f6500bac04588f3bb44"))
        ( 50000,   uint256S("0xa691f38abcea1434b10dd64714b12d9654aae718d2a5d6d8b45720108edc3d5d"))
        ( 75000,   uint256S("0x461190bfda4470466ad456291de9094664ef56f85a73b2f3811d06eed444bbd8"))
        ( 100000,  uint256S("0x06b2abf25bd4d7d1d9866e166e90c389480c58aaacf6545d2277c14c8cbb94d3"))
        ( 200000,  uint256S("0x79cb7dba6456048b9af2ffaae309f9c4d80b436ad92e01e8ae11ec1127ee5b18"))
        ( 300000,  uint256S("0x6b77bcab02c162b9a7a4424db80bd4241564d2c9d783e3c9f0e02cd0f25b0025"))
        ( 480000,  uint256S("0xa11759fa9ed9c11769dc7ec3c279f523c886ea0ca0b9e1d1a49441c32b701f0d"))
        ( 600308,  uint256S("0x0cd7f68e0e79a4595abb871fb71fd2db803b34b15da182d23c1568f56611af91"))
        ( 778389,  uint256S("0x5d1c20eda68cf4221885f2e9e0ad47817cca44da24f770f1334ca8cc2b07eb49"))
        ( 800000,  uint256S("0x609f6259e199a0a60705bf4c02d6aee84dc38d7d741f8044b1eec0c636567ebb"))
        ( 850000,  uint256S("0xa52a30db508a3bc3ab71f9bd83573e4ab4289ed8e5b66dc92f862baf6eb80eba"))
        ( 900000,  uint256S("0x254c90e2a47d0f28292bc7d6f5cd8f856eb782d8692a1d923abd9c43749935bc"))
        ( 950000,  uint256S("0x706c8ce10f2c9ffbf90fc10fe683ccabeeafa4e6cf5c8ec320f50319c9ba40a9"))
        ( 1000000, uint256S("0x564e27dc7bf17488c4bf66ba0955cbf075c975f8386ede157be9578da0476356"))
        ( 1010000, uint256S("0xbbe9adc561be01cb37acd71abf42a17e566d05b9cdfed95793db931540805bcf"))
        ( 1020000, uint256S("0xdc51bcc193a2e84bcbdd0448e6e0f5396b4e57c2f43e239d110d1145b147d4c9"))
        ( 1023000, uint256S("0xeaae71b7dae28ab3abfcfa959ae3db50eb4ed93204e36731a54063a4ea8e7218"))
        ( 1023013, uint256S("0xc328d2a8f8b976769a6b0488cbf6dc641902b6eb7db0995befd58e69679af4f8"))
        ( 3301439, uint256S("0x91e893e7d89086c092d634ad7075f8b6373588d6001cc44435710bea0c6e7512")),
        1528399290, // * UNIX timestamp of last checkpoint block
        4476765,   // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        3840.0     // * estimated number of transactions per day after checkpoint        
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 4730400;
        consensus.nMasternodePaymentsStartBlock = 200; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 220;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 240;
        consensus.nBudgetPaymentsCycleBlocks = 100;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 250; // NOTE: Should satisfy nSuperblockStartBlock > nBudgetPeymentsStartBlock
        consensus.nSuperblockCycle = 100; // Superblocks can be issued hourly on testnet
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0x0000047d24635e347be3aaaeb66c26be94901a2f962feccd4f95090191f208c1");
        consensus.powLimit = uint256S("0000100000000000000000000000000000000000000000000000000000000000");
        consensus.bnProofOfWorkLimit[ALGO_SHA256D] = CBigNum(uint256S("0000100000000000000000000000000000000000000000000000000000000000"));
        consensus.bnProofOfWorkLimit[ALGO_SCRYPT]  = CBigNum(uint256S("0000100000000000000000000000000000000000000000000000000000000000"));
        consensus.bnProofOfWorkLimit[ALGO_X11] = CBigNum(uint256S("0000100000000000000000000000000000000000000000000000000000000000"));
        consensus.nPowTargetTimespan = 108*40; // Digitalcoin: 1 day
        consensus.nPowTargetSpacing = 40; // Digitalcoin: 2.5 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 4001; // nPowKGWHeight >= nPowDGWHeight means "no KGW"
        consensus.nPowDGWHeight = 4001;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1506556800; // September 28th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1538092800; // September 28th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 3;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1505692800; // Sep 18th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1537228800; // Sep 18th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 50; // 50% of 100

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00"); // 37900

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00"); // 37900
        /*
        pchMessageStart[0] = 0xce;
        pchMessageStart[1] = 0xe2;
        pchMessageStart[2] = 0xca;
        pchMessageStart[3] = 0xff;
        */
        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0xb6;
        pchMessageStart[3] = 0xdb;
        vAlertPubKey = ParseHex("04016c44069c3152982413d3ba3bf262a3a4d3ddad859ba78e0d744f5c67c2205d2aa2122e6c62b6310dad2d1e2f7e39028455ff1dbb26511c60fc96c8b4560c43");
        nDefaultPort = 17999;
        nMaxTipAge = 0x7fffffff; // allow mining on top of old blocks for testnet
        nDelayGetHeadersTime = 30 * 24 * 60 * 60;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1524863865, 1363366, 0x1e0ffff0, 1, 50 * COIN);
        //genesis = CreateGenesisBlock(1390666206UL, 3861367235UL, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x9dc0f1255caf75497a73dd0183d06b45dbe23bdfc90f67a2fb124a19be1a4cde"));
        assert(genesis.hashMerkleRoot == uint256S("0xecb2c595fff9f2364152c32027737007c5a4c60ec960cf93754b0211bc2a1501"));


//        assert(consensus.hashGenesisBlock == uint256S("0x00000bafbc94add76cb75e2ec92894837288a481e5c005f6563d91623bf8bc2c"));
//        assert(genesis.hashMerkleRoot == uint256S("0xe0028eb9648db56b1ac77cf090b99048a8007e2bb64b68f092c03c7f56a662c7"));

        vFixedSeeds.clear();
        vSeeds.clear();

//       vSeeds.push_back(CDNSSeedData("digitalcoindot.io",  "testnet-seed.digitalcoindot.io"));
 //       vSeeds.push_back(CDNSSeedData("masternode.io", "test.dnsseed.masternode.io"));

        // Digitalcoin addresses start with 'X'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,30);
        // Digitalcoin script addresses start with '7'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        // Digitalcoin private keys start with '7' or 'X'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        // Digitalcoin BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Digitalcoin BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        // Testnet Digitalcoin BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "0419fb74e341a19c66e27a3ef642f6ecaa8eaad487d069a322a5b0c848d339ed6aa2f5d976c12efa934b53bb2daf5999dd2bfd08fb7788f9a4cd4c09e28573bb0d";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
        	(     0, uint256S("0x9dc0f1255caf75497a73dd0183d06b45dbe23bdfc90f67a2fb124a19be1a4cde")),

            1524863865, // * UNIX timestamp of last checkpoint block
            0,       // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            1000         // * estimated number of transactions per day after checkpoint
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 24 * 60 * 60; // Digitalcoin: 1 day
        consensus.nPowTargetSpacing = 2.5 * 60; // Digitalcoin: 2.5 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nPowKGWHeight = 15200; // same as mainnet
        consensus.nPowDGWHeight = 34140; // same as mainnet
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 3;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xb3;
        pchMessageStart[2] = 0xb2;
        pchMessageStart[3] = 0xdb;
        vAlertPubKey = ParseHex("04016c44069c3152982413d3ba3bf262a3a4d3ddad859ba78e0d744f5c67c2205d2aa2122e6c62b6310dad2d1e2f7e39028455ff1dbb26511c60fc96c8b4560c43");
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDelayGetHeadersTime = 0; // never delay GETHEADERS in regtests
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1392796564, 961533, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));
        //assert(genesis.hashMerkleRoot == uint256S("0xe0028eb9648db56b1ac77cf090b99048a8007e2bb64b68f092c03c7f56a662c7"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            (  0, uint256S("0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206")),
            0,
            0,
            0
        };
        // Digitalcoin addresses start with 'X'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,30);
        // Digitalcoin script addresses start with '7'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        // Digitalcoin private keys start with '7' or 'X'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // Digitalcoin BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Digitalcoin BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        // Regtest Digitalcoin BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;
   }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
