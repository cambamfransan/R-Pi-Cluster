#!/bin/bash

mkdir ~/cluster
cd ~/cluster
git clone https://github.com/cambamfransan/R-Pi-Cluster.git
cd R-Pi-Cluster
git merge origin/python
cd ../
mkdir _bld
cd _bld
sudo apt-get update
sudo apt-get update --fix-missing
sudo apt-get install qt5-default -y
sudo apt-get install rapidjson-dev -y
sudo apt-get install libprotobuf-dev -y
sudo apt-get install cmake -y
sudo apt-get install protobuf-compiler -y
sudo apt-get install libprotobuf10 -y
sudo apt-get install libprotoc-dev -y
sudo apt-get install nodejs -y
sudo apt-get install npm -y
sudo npm install express --save
cmake ../R-Pi-Cluster
make -j3 install
