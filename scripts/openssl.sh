#!/bin/sh
sudo apt-get install build-essential libssl-dev -y
cd /usr/local/src
sudo wget https://www.openssl.org/source/openssl-1.1.1b.tar.gz -O openssl-1.1.0.tar.gz
sudo tar -zxvf openssl-1.1.0.tar.gz
cd openssl-1.1.1b
sudo ./config --prefix=/usr/local/ssl --openssldir=/usr/local/ssl -Wl,-rpath,/usr/local/ssl/lib
sudomake
sudo make install
sudo mv /usr/bin/openssl /root/
sudo ln -s /usr/local/ssl/bin/openssl /usr/bin/openssl
openssl version