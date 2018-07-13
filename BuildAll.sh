sudo add-apt-repository -y ppa:bitcoin/bitcoin
sudo apt-get update
sudo apt-get install -y build-essential libtool autotools-dev autoconf pkg-config libssl-dev automake
sudo apt-get install -y libboost-all-dev libdb4.8-dev libdb4.8++-dev libprotobuf-dev protobuf-compiler libevent-dev miniupnpc-dev libqrencode-dev
sudo apt-get install -y libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools
./autogen.sh
./configure -enable-upnp-default -with-miniupnpc -disable-tests
sudo make install
digitalcoind
