#!/bin/bash

protos=`find ./ -name '*.proto'`
for i in $protos
do
    echo 'building ' $i
    protoc --cpp_out=./code $i
done
