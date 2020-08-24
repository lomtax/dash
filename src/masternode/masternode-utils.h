// Copyright (c) 2014-2019 The Dash Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MASTERNODE_UTILS_H
#define MASTERNODE_UTILS_H

#include "evo/deterministicmns.h"
#include "wallet/wallet.h"

class CConnman;

class CMasternodeUtils
{
public:
    static void ProcessMasternodeConnections(CConnman& connman);
    static void DoMaintenance(CConnman &connman);
    static std::string makeGenkey();
    static void writeDigitalcoinConfFile(std::string _line);
    static std::string getConfParam(std::string _arg);
    static CBitcoinAddress GetAccountAddressForMasternode( std::string  strAccount, bool bForceNew=false);
    static void writeMasternodeConfFile(std::string  _alias, std::string  _ipport,std::string  mnprivkey,std::string  _output,std::string  _index);
    static void writeDigitalcoinMasternodeConfInfo(std::string  mnGenkey, std::string  strIpPort);
    static std::vector<std::pair<std::string ,std::string >> checkMasternodeOutputs();
    static void cleanDigitalcoinConf();
    static void RemoveMasternodeConfigs();
};

#endif//MASTERNODE_UTILS_H

