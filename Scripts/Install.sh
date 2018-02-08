#!/bin/bash

mkdir ~/cluster
cd ~/cluster
git clone https://github.com/cambamfransan/R-Pi-Cluster.git
mkdir _bld
cd _bld
sudo apt-get install qt5-default -y
sudo apt-get install rapidjson-dev -y
sudo apt-get install libprotobuf-dev -y
sudo apt-get install cmake -y
sudo apt-get install protobuf-compiler -y
sudo apt-get install libprotobuf10 -y
sudo apt-get install libprotoc-dev -y
cmake ../R-Pi-Cluster
make -j3 install
