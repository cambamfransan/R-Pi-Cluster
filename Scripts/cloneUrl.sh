# 1: database 2: Job URL 3: URLName

mkdir %1
cd %1
git clone $2
mkdir _bld
cd _bld
cmake ../$3
make install
