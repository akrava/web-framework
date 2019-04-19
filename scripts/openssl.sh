#!/bin/sh
sudo apt-get install build-essential libssl-dev -y
cd /usr/local/src
wget https://www.openssl.org/source/latest.tar.gz -O openssl-1.1.0.tar.gz
tar -zxvf openssl-1.1.0.tar.gz
cd openssl-1.1.0
./config --prefix=/usr/local/ssl --openssldir=/usr/local/ssl -Wl,-rpath,/usr/local/ssl/lib
make
make install
mv /usr/bin/openssl /root/
ln -s /usr/local/ssl/bin/openssl /usr/bin/openssl
openssl version