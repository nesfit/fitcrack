#!/bin/bash
echo "Generating GRPC CPP headers..."
protoc -I protocol --grpc_out=protocol/ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` protocol/protocol.proto
protoc -I protocol --cpp_out=protocol/ protocol/protocol.proto
