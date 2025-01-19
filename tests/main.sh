#!/bin/bash

# Colors:
export RED='\033[0;31m'
export GREEN='\033[0;32m'
export NC='\033[0m' # No Color

tests/ip_conv.sh
tests/server_test.sh
tests/client_test.sh
tests/send_file.sh
tests/send_large.sh
tests/recv_file.sh
tests/recv_large.sh

unset RED
unset GREEN 
unset NC 
