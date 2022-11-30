#!/bin/bash
if [ -d "./build" ] 
then
    rm -rf build 
fi
mkdir build 
cd build && cmake .. 
make
# 当前目录build
cp ../scripts/* ./
cd ..

if [ -d "./bin" ] 
then
    rm -rf bin 
fi
mkdir bin

cp ./build/test/* ./bin
rm -rf ./bin/*Make*
rm ./bin/*make*

cp -r ./bin/* ./scripts

echo "Executables are generated!"
