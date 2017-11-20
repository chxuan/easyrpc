#!/bin/bash
protoc -I=./ --cpp_out=./code proto_message.proto
