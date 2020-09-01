#!/bin/bash
# use testnet settings,  if you need mainnet,  use ~/.digitalcoincore/digitalcoind.pid file instead
digitalcoin_pid=$(<~/.digitalcoincore/testnet3/digitalcoind.pid)
sudo gdb -batch -ex "source debug.gdb" digitalcoind ${digitalcoin_pid}
