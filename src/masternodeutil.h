
#ifndef MASTERNODE_UTIL_H
#define MASTERNODE_UTIL_H

#include "wallet/wallet.h"

std::string makeGenkey();

void writeDigitalcoinConfFile(std::string _line);

std::string getConfParam(std::string _arg);

CBitcoinAddress GetAccountAddressForMasternode( std::string  strAccount, bool bForceNew=false);

void writeMasternodeConfFile(std::string  _alias, std::string  _ipport,std::string  mnprivkey,std::string  _output,std::string  _index);

void writeDigitalcoinMasternodeConfInfo(std::string  mnGenkey, std::string  strIpPort);

std::vector<std::pair<std::string ,std::string >> checkMasternodeOutputs();

void cleanDigitalcoinConf();

void RemoveMasternodeConfigs();

#endif // MASTERNODE_UTIL_H
