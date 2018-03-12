// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2018-2018 The Ultima Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"
#include "arith_uint256.h"
#include "chain.h"
#include "primitives/block.h"
#include "uint256.h"
#include <math.h>
#include <stdio.h>
#include "chainparams.h"
#include "consensus/merkle.h"
#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include <assert.h>
#include <inttypes.h>
#include <boost/assign/list_of.hpp>
#include "chainparamsseeds.h"


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


static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "3/2/2018 Coindesk - Liechtenstein Bank Opens Up Cryptocurrency Investment for Clients";
    const CScript genesisOutputScript = CScript() << ParseHex("040825d9629587373fae82ecab935a8d9ac09b743234366740c6f8aea263c5a7f914e630f9733264426a21c27cea00bc7427fc7397773aa102f526649d818aaf8e") << OP_CHECKSIG;
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
        consensus.nSubsidyHalvingInterval = 1051200; // one year
        consensus.nMasternodePaymentsStartBlock = 10000; // not true, it has to be lower than nMasternodePaymentsIncreaseBlock though
        consensus.nMasternodePaymentsIncreaseBlock = 40320; // approximately 14 days from start
        consensus.nMasternodePaymentsIncreasePeriod = 20160; // approximately 7 days after first increase
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 1576800; // approx 1.5 years
        consensus.nBudgetPaymentsCycleBlocks = 166160;
        consensus.nBudgetPaymentsWindowBlocks = 1000;
        consensus.nBudgetProposalEstablishingTime = 60*60*24;
        consensus.nSuperblockStartBlock = 1000000000; // not used for now
        consensus.nSuperblockCycle = 16616;
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 0; // from the genesis block
        consensus.BIP34Hash = uint256S("0x00000b817c072f8b5ab4c01377945436412b02d65075ac3efd58f3fde4cc8b3a"); // genesis hash
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 2 * 60; // Ultima - 120 seconds, 4 blocks
        consensus.nPowTargetSpacing = 30; // Ultima - 30 seconds
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1502280000; // Aug 9th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1533816000; // Aug 9th, 2018

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xa2;
        pchMessageStart[1] = 0xc4;
        pchMessageStart[2] = 0xcb;
        pchMessageStart[3] = 0x4a;
        vAlertPubKey = ParseHex("04aad2ee49301a274c5908baecab6f37e80114478b2d19a3d6d1e3686eb77e819065454bdd03ada42163bb8ce757c5d79770459951f10de318180435b0cbf5f8db");
        nDefaultPort = 8420;
        nMaxTipAge = 6 * 60 * 60; // ~720 blocks
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1520845200, 1884578, 0x1e0ffff0, 1, 3 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000b817c072f8b5ab4c01377945436412b02d65075ac3efd58f3fde4cc8b3a"));
        assert(genesis.hashMerkleRoot == uint256S("0x2fd91ac3873f4ba6aaed16c32a4630acefe6b78e290b768e285032feb36625c3"));


//        vFixedSeeds.clear();
//        vSeeds.clear();

        vSeeds.push_back(CDNSSeedData("ultima.qa", "seed1.ultima.qa"));
        vSeeds.push_back(CDNSSeedData("ultima.qa", "seed2.ultima.qa"));
        vSeeds.push_back(CDNSSeedData("ultima.qa", "seed3.ultima.qa"));

        // Ultima addresses start with 'U'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,68);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,30);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,224);
        // Ultima BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Ultima BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // Ultima BIP44 coin type is '5'
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x05).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "041e084dd43dea7a5bc9d8ed200073fc6bd9f35d7216f6f73fbbd819db16f72f370bc72878fb72919879522a730420039e69758845a3e69a722dd484c1fcc58ada";
        strMasternodePaymentsPubKey = "041e084dd43dea7a5bc9d8ed200073fc6bd9f35d7216f6f73fbbd819db16f72f370bc72878fb72919879522a730420039e69758845a3e69a722dd484c1fcc58ada";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (    0, uint256S("0x00000b817c072f8b5ab4c01377945436412b02d65075ac3efd58f3fde4cc8b3a")),
            1520845200, // * UNIX timestamp of last checkpoint block
            0,    //  total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            2800        // * estimated number of transactions per day after checkpoint
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
        consensus.nSubsidyHalvingInterval = 1051200;
        consensus.nMasternodePaymentsStartBlock = 10000; // not true
        consensus.nMasternodePaymentsIncreaseBlock = 40320;
        consensus.nMasternodePaymentsIncreasePeriod = 20160;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 2100000000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 1000000000; // not used for now
        consensus.nSuperblockCycle = 24; // Superblocks can be issued hourly on testnet
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 0; // genesis
        consensus.BIP34Hash = uint256S("0x000004fd9b314f4b222e3949bf29df66e8edd98e32de691d895db1e43c470aaf"); // genesis
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 2 * 60; // Ultima - 120 seconds
        consensus.nPowTargetSpacing = 30; // Ultima - 30 seconds
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1502280000; // Aug 9th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1533816000; // Aug 9th, 2018

        pchMessageStart[0] = 0x2a;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0x6c;
        vAlertPubKey = ParseHex("04efd62bdb1677a56bcf165830c6b0e653e0d25f1d0df25e190ecfa6d088849b08de3389303f85e39b3d563633b77735b44c36d6b13f2f99f15d31a2862d28d9bc");
        nDefaultPort = 8422;
        nMaxTipAge = 0x7fffffff; // allow mining on top of old blocks for testnet
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1520845100, 1605382, 0x1e0ffff0, 1, 3 * COIN);
        
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000004fd9b314f4b222e3949bf29df66e8edd98e32de691d895db1e43c470aaf"));
        assert(genesis.hashMerkleRoot == uint256S("0x2fd91ac3873f4ba6aaed16c32a4630acefe6b78e290b768e285032feb36625c3"));

        vFixedSeeds.clear();
        vSeeds.clear();
//      vSeeds.push_back(CDNSSeedData("ultima.qa",  "seedtest1.ultima.qa"));
//      vSeeds.push_back(CDNSSeedData("ultima.qa",  "seedtest2.ultima.qa"));

        // Testnet Ultima addresses start with 'n'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,112);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,20);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,240);
        // Testnet Ultima BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Testnet Ultima BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Testnet Ultima BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "04cc93dc4cc3bd2438cc94971c8382d6a3e353f66b8f4eb1f1fc0c5b7508c7cb84aff96079eb5c75904d402771f37129012f95984d5dd7ad9e7751a772e8ed6cfb";
        strMasternodePaymentsPubKey = "04cc93dc4cc3bd2438cc94971c8382d6a3e353f66b8f4eb1f1fc0c5b7508c7cb84aff96079eb5c75904d402771f37129012f95984d5dd7ad9e7751a772e8ed6cfb";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x000004fd9b314f4b222e3949bf29df66e8edd98e32de691d895db1e43c470aaf")),
	     1520845100, // * UNIX timestamp of last checkpoint block
            0,     // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            500         // * estimated number of transactions per day after checkpoint
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
        consensus.nPowTargetTimespan = 2 * 60; // Ultima - 120 seconds, 4 blocks
        consensus.nPowTargetSpacing = 30; // Ultima: 30 seconds
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        pchMessageStart[0] = 0xc1;
        pchMessageStart[1] = 0x2a;
        pchMessageStart[2] = 0xd1;
        pchMessageStart[3] = 0xab;
        nMaxTipAge = 30 * 24 * 60 * 60; // one month behind
        nDefaultPort = 8424;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1520845000, 1, 0x207fffff, 1, 3 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x6d4ba4a073dc5667b1d63099310b62b505d1c2224a66bc31d4b4489e714adc8c"));
        assert(genesis.hashMerkleRoot == uint256S("0x2fd91ac3873f4ba6aaed16c32a4630acefe6b78e290b768e285032feb36625c3"));

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
            ( 0, uint256S("0x6d4ba4a073dc5667b1d63099310b62b505d1c2224a66bc31d4b4489e714adc8c")),
            0,
            0,
            0
        };
        // Regtest Ultima addresses start with 'n'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,112);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,20);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,240);
        // Regtest Ultima BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Regtest Ultima BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Regtest Ultima BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();
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
