#!/bin/bash

# Server example test:

# NOTE: this test does not check if a client has connected to the server,
# 		it will assume that the client is connecting to the correct address and port.

# Kill processes before beginning:
killall nc 1>/dev/null 2>&1 
killall ncc_test 1>/dev/null 2>&1 

# Starting the server:
timeout 10 bin/ncc_test serve > test.log &

# Wait:
sleep 0.1

# Getting results
CLIENT=$(echo "server test" | nc -w1 127.0.0.1 9001)

SERVER=$(cat test.log)

STATUS=$(tr -d '\0' < test.file)

if [ "$STATUS" = "server test" ]; then
	echo -e "${GREEN}[+]${NC} Passed server test" 
else 
	echo -e "${RED}[-]${NC} Failed server test"
	printf "[?] Output: \n$RES\n"
	printf "[?] Server output: \n"
	printf "$SERVER \n"
	printf "[?] Result: \n"
	printf "$STATUS \n"
fi
