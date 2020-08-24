// Copyright (c) 2014-2020 The Dash Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "compat.h"
#include "tinyformat.h"
#include "utiltime.h"
#include "amount.h"

#include "base58.h"
#include "netbase.h"
#include "privatesend-server.h"
#include "util.h"
#include "wallet/wallet.h"

#include <univalue.h>
#include <boost/filesystem/path.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread/exceptions.hpp>
#include <exception>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>
#include <fstream>

#include "activemasternode.h"
#include "masternode-payments.h"
#include "masternode-sync.h"
#include "masternodeconfig.h"
#include "masternodeman.h"
#include "rpc/server.h"
#include "utilmoneystr.h"

#include "masternode/masternode-utils.h"

#include "init.h"
#include "masternode/masternode-sync.h"
#ifdef ENABLE_WALLET
#include "privatesend/privatesend-client.h"
#endif
#include "validation.h"

struct CompareScoreMN
{
    bool operator()(const std::pair<arith_uint256, const CDeterministicMNCPtr&>& t1,
                    const std::pair<arith_uint256, const CDeterministicMNCPtr&>& t2) const
    {
        return (t1.first != t2.first) ? (t1.first < t2.first) : (t1.second->collateralOutpoint < t2.second->collateralOutpoint);
    }
};

void CMasternodeUtils::ProcessMasternodeConnections(CConnman& connman)
{
    std::vector<CDeterministicMNCPtr> vecDmns; // will be empty when no wallet
#ifdef ENABLE_WALLET
    privateSendClient.GetMixingMasternodesInfo(vecDmns);
#endif // ENABLE_WALLET

    // Don't disconnect masternode connections when we have less then the desired amount of outbound nodes
    int nonMasternodeCount = 0;
    connman.ForEachNode(CConnman::AllNodes, [&](CNode* pnode) {
        if (!pnode->fInbound && !pnode->fFeeler && !pnode->m_manual_connection && !pnode->fMasternode) {
            nonMasternodeCount++;
        }
    });
    if (nonMasternodeCount < connman.GetMaxOutboundNodeCount()) {
        return;
    }

    connman.ForEachNode(CConnman::AllNodes, [&](CNode* pnode) {
        if (pnode->fMasternode && !connman.IsMasternodeQuorumNode(pnode)) {
#ifdef ENABLE_WALLET
            bool fFound = false;
            for (const auto& dmn : vecDmns) {
                if (pnode->addr == dmn->pdmnState->addr) {
                    fFound = true;
                    break;
                }
            }
            if (fFound) return; // do NOT disconnect mixing masternodes
#endif // ENABLE_WALLET
            if (fLogIPs) {
                LogPrintf("Closing Masternode connection: peer=%d, addr=%s\n", pnode->GetId(), pnode->addr.ToString());
            } else {
                LogPrintf("Closing Masternode connection: peer=%d\n", pnode->GetId());
            }
            pnode->fDisconnect = true;
        }
    });
}

void CMasternodeUtils::DoMaintenance(CConnman& connman)
{
    if(!masternodeSync.IsBlockchainSynced() || ShutdownRequested())
        return;

    static unsigned int nTick = 0;

    nTick++;

    if(nTick % 60 == 0) {
        ProcessMasternodeConnections(connman);
    }
}

std::string MasternodeUtils::makeGenkey()
{
    //genkey
    CKey secret;
    secret.MakeNewKey(false);

    auto mnGenkey = CBitcoinSecret(secret).ToString();

    return mnGenkey;
}

void MasternodeUtils::writeDigitalcoinConfFile(std::string  _line)
{
    FILE *  fileout=NULL;
    
    boost::filesystem::path pathDebug = GetDataDir() / "digitalcoin.conf";

    fileout = fopen (pathDebug.string().c_str(),"aw");
    std::string  s =std::string ("\n")+_line;

    fprintf(fileout,"%s",s.c_str());

    fclose (fileout);
}

std::string MasternodeUtils::getConfParam(std::string _arg)
{
    BOOST_FOREACH(auto ar, mapArgs) 
    {
        if(ar.first==_arg)
            return ar.second;    
    }
    return std::string ("");
}

//CBitcoinAddress GetAccountAddress( std::string  strAccount, bool bForceNew=false);

void MasternodeUtils::writeMasternodeConfFile(std::string  _alias, std::string  _ipport,std::string  mnprivkey,std::string  _output,std::string  _index)
{
    FILE *  fileout=NULL;
    boost::filesystem::path pathDebug2 = GetDataDir() / "masternode.conf";

    fileout = fopen (pathDebug2.string().c_str(),"w");// use "a" for append, "w" to overwrite, previous content will be deleted

    std::string  s =std::string ("\n")+_alias+std::string (" ")+_ipport+std::string (" ")+mnprivkey+std::string (" ")+_output+std::string (" ")+_index;

    fprintf(fileout,"%s",s.c_str());
    fclose (fileout); // must close after opening
}

void MasternodeUtils::writeDigitalcoinMasternodeConfInfo(std::string  mnGenkey, std::string  strIpPort)
{
    writeDigitalcoinConfFile("masternode=1");
    writeDigitalcoinConfFile("masternodeprivkey="+mnGenkey);
    writeDigitalcoinConfFile("externalip="+strIpPort);
    writeDigitalcoinConfFile("masternodeaddr="+strIpPort);
}

std::vector<std::pair<std::string ,std::string >> MasternodeUtils::checkMasternodeOutputs()
{
    std::vector<std::pair<std::string ,std::string >> result;

    std::vector<COutput> vPossibleCoins;
    pwalletMain->AvailableCoins(vPossibleCoins, true, NULL, false, ONLY_1000);

    UniValue obj(UniValue::VOBJ);
    BOOST_FOREACH(COutput& out, vPossibleCoins) 
    {
        result.push_back(std::pair<std::string ,std::string >{out.tx->GetHash().ToString(),std::to_string(out.i)});
    }

    return result;
}

void MasternodeUtils::cleanDigitalcoinConf()
{
    boost::filesystem::path pathDebug = GetDataDir() / "digitalcoin.conf";

    std::vector<std::string> lines;

    std::string item_name;
    std::ifstream nameFileout;

    nameFileout.open(pathDebug.string().c_str());

    while (nameFileout >> item_name)
    {
        if(item_name.find("masternode") == std::string::npos
        && item_name.find("externalip") == std::string::npos)
        {
           lines.push_back(item_name);
        }
    }

    nameFileout.close();
    
    remove(pathDebug.string().c_str());
    
    for(std::string& key : lines)
        writeDigitalcoinConfFile(key);
}

void MasternodeUtils::RemoveMasternodeConfigs()
{
    cleanDigitalcoinConf();
    boost::filesystem::path masternodeConfPath = GetDataDir() / "masternode.conf";

    if (boost::filesystem::exists(masternodeConfPath.string().c_str()))        
        remove(masternodeConfPath.string().c_str());
}


