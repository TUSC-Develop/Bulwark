// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017-2018 The Tusc developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"
void MineGenesis(CBlock genesis){
    // This will figure out a valid hash and Nonce if you're creating a different genesis block:
    uint256 newhash = genesis.GetHash();
    uint256 besthash;
    int64_t nStart = GetTime();
    uint256 hashTarget = ~uint256(0) >> 20;
    printf("Target: %s\n", hashTarget.GetHex().c_str());
    memset(&besthash,0xFF,32);
    while (newhash > hashTarget) {
        ++genesis.nNonce;
        if (genesis.nNonce == 0){
            printf("NONCE WRAPPED, incrementing time");
            ++genesis.nTime;
        }
        newhash = genesis.GetHash();
        if(newhash < besthash){
            besthash=newhash;
            printf("New best: %s\n", newhash.GetHex().c_str());
        }
    }
    printf("Found Genesis, Nonce: %ld, Hash: %s\n", genesis.nNonce, genesis.GetHash().GetHex().c_str());
    printf("Gensis Hash Merkle: %s\n", genesis.hashMerkleRoot.ToString().c_str());
}

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}
//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions

static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
        (0, uint256("0x00000c2432502e1fa448858527c3e62a16282e72e84f6bdb0a9175ada51a0b48"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1531182600,// * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
	boost::assign::map_list_of(0, uint256("0x00000f75e835201379d8f8522ca034c50ca6770938b6c44d3b858a74dc7d370a"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1531182601,
    0,
    250};
static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1531182631,
    0,
    100};
class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x08;
        pchMessageStart[1] = 0x02;
        pchMessageStart[2] = 0x01;
        pchMessageStart[3] = 0x17;
	    vAlertPubKey = ParseHex("042dfbb26101a2b4c3966db428edd7a109298cf5427a2a5a1e364197975d13bb81a85a42d9cacd09fc70e79cf09ef589c01169eb62c4d922a7d1792766107b40ae");
        nDefaultPort = 20235;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // Tusc starting difficulty is 1 / 2^12
        nMaxReorganizationDepth = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Tusc: 1 minute
        nTargetSpacingSlowLaunch = 5 * 90;  // Tusc: 7.5 minutes (Slow launch - Block 300)
	    nTargetSpacing = 1 * 60; // Tusc: 1min after block 100
        nLastPOWBlock = 1500;
        nLastPOWBlockOld = 345600; // 1 year
		nLastSeeSawBlock = 200000; // last block for seesaw rewards
	    nRampToBlock = 250; // Slow start, ramp linearly to this block
        nMaturity = 25; // 25 Minutes
	    nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 1;
	    nMaxMoneyOut = 24525000 * COIN; // Year 2

        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;

        const char* pszTimestamp = "THE GUARDIAN 07/10/2018 Thai cave rescue: eight boys now freed as mission to save others continues – live";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("0409fa4710f9c26124da396736b5b4edfb4d26767e0e09e32ad88ae0da5b92e36f2478fe39900fa26a4ebae241c89208a5af29dd84683bb091ca2232d30be4d990") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1531182600;
        genesis.nBits = bnProofOfWorkLimit.GetCompact();;
        genesis.nNonce = 1199343;

	hashGenesisBlock = genesis.GetHash();
		assert(hashGenesisBlock == uint256("0x00000c2432502e1fa448858527c3e62a16282e72e84f6bdb0a9175ada51a0b48"));
		assert(genesis.hashMerkleRoot == uint256("0xd91b44998fa9b6274476549ef78e451c93894de038fb77e272fc26cd9d010a15"));

        vSeeds.push_back(CDNSSeedData("master.thatcoin.tech", "master.thatcoin.tech"));      // Single node address
        vSeeds.push_back(CDNSSeedData("slave.thatcoin.teach", "slave.thatcoin.tech"));      // Single node address


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 128); // t
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 18);
	base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
	base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
//        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
//        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
//        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
//BIP44 as defined by https://github.com/bitcoin/bips/blob/master/bip-0044.mediawiki
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

	    convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

	    fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

	    nPoolMaxTransactions = 3;

        strSporkKey = "04017e4c47f70a434d6e204d5817277162a37b36fa896105a0ad298113079480e070aaef7ed8d1472f7690fd3eb46b4c627d157b7d32a6bf70f129e8a54e072458";
        strObfuscationPoolDummyAddress = "ti1VQna6by2AvdT38L5JkKg82XPhBbzyLR";
        nStartMasternodePayments = 1531186200;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;
/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xb5;
        pchMessageStart[1] = 0xd9;
        pchMessageStart[2] = 0xf4;
        pchMessageStart[3] = 0xa0;

	    vAlertPubKey = ParseHex("04f078362a716c402e976b279e7a2e033a5774d17e27de100a6013074af9b7d96dff3cd29b59c5133d011d7656096cd80b56943cb0d527f708a6ad8f1456a9e8d1");
        nDefaultPort = 42133;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 30; // 30 Seconds
        nTargetSpacing = 1 * 30;  // 30 Seconds
	    nTargetSpacingSlowLaunch = 1 * 30; // Kludgy but don't want to check for testnet each time in GetNextWorkRequired

        nLastPOWBlock = 1000;
        nLastPOWBlockOld = 1100;
		nLastSeeSawBlock = 1200;
        nMaturity = 15;
        nMaxMoneyOut = 33284220 * COIN; // 2032 Maximum
        nRampToBlock = 100;

        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1531146601;
        genesis.nNonce = 1968143;
        genesis.nBits = bnProofOfWorkLimit.GetCompact();

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000f75e835201379d8f8522ca034c50ca6770938b6c44d3b858a74dc7d370a"));
        assert(genesis.hashMerkleRoot == uint256("0xd91b44998fa9b6274476549ef78e451c93894de038fb77e272fc26cd9d010a15"));

        vFixedSeeds.clear();
        vSeeds.clear();


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 65); // Testnet tusc addresses start with 'T'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 12);  // Testnet tusc script addresses start with '5' or '6'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet tusc BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet tusc BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet tusc BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x01)(0x00)(0x00)(0x80).convert_to_container<std::vector<unsigned char> >();
        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));
        fRequireRPCPassword = true;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;
        nPoolMaxTransactions = 2;
        strSporkKey = "04eb42fd898f381b2a8df8cc9ec3cd41ce09f6e5bd8c20e6a3752866fae69b19de838769c373916ea107da4577d71f13d26bcaf511d9e9bbd607e82f19cea1e182";
        strObfuscationPoolDummyAddress = "ti1VQna6by2AvdT38L5JkKg82XPhBbzyLR";
        nStartMasternodePayments = 1531157401; //Fri, 09 Jan 2015 21:05:58 GMT
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;
/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Tusc: 1 day
        nTargetSpacing = 1 * 60;        // Tusc: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1531182631;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 0;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 51476;
//        assert(hashGenesisBlock == uint256("0x"));
        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.
        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;
/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.
        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }
    //! Published setters to allow changing values in unit test cases
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
};
static CUnitTestParams unitTestParams;
static CChainParams* pCurrentParams = 0;
CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}
const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}
CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}
void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;
    SelectParams(network);
    return true;
}
