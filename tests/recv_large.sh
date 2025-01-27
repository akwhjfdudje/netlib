#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Test for receiving large amount of random data 

# Kill processes before beginning:
killall nc 1>/dev/null 2>&1 
killall ncc_test 1>/dev/null 2>&1 
SIZE=479114

# Starting the server:
timeout 10 bin/ncc_test receivelarge $SIZE > test.file &

# Wait:
sleep 0.1
dd if=/dev/urandom count=1 bs=$SIZE of=./random.data 2>/dev/null 1>&2
SIZE=$(wc -c < random.data)

# Getting results
CLIENT=$(nc 127.0.0.1 9001 < random.data) 

SERVER=$(cat test.file | head -n5)

# Checking results:
if [ "$(wc -c < test.file)" = "$SIZE" ]; then
	echo -e "${GREEN}[+]${NC} Received data successfully"
else
	echo -e "${RED}[-]${NC} Failed to receive file"
	printf "[?] Server output: \n"
	printf "$SERVER\n"
	printf "[?] Client output: \n"
	printf "$CLIENT\n"
	exit
fi 

# Delete files:
rm test.file
rm random.data
