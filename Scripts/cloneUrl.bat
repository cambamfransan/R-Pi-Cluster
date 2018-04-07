#!/bin/bash

echo $DIR

pwd
mkdir Jobs
cd Jobs
mkdir $1
cd $1
git clone $2
mkdir _bld
cd _bld
cmake ../$1
make install
