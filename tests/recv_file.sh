#!/bin/bash
# Test for receiving /etc/passwd into the current directory

# Kill processes before beginning:
killall nc 1>/dev/null 2>&1 
killall ncc_test 1>/dev/null 2>&1 

# Starting the server:
timeout 10 bin/ncc_test receive > test.log &

# Wait:
sleep 0.1

# Getting results
CLIENT=$(nc 127.0.0.1 9001 < /etc/passwd)

SERVER=$(cat test.log)

STATUS=$(cmp --silent /etc/passwd test.file; echo $?)

# Checking results:
#if [ "$SERVER" = "root" ]; then
if [ "$STATUS" = "0" ]; then
	echo -e "${GREEN}[+]${NC} Received file successfully"
else
	echo -e "${RED}[-]${NC} Failed to receive file"
	printf "[?] Server output: \n"
	printf "$SERVER\n"
	printf "[?] Client output: \n"
	printf "$CLIENT\n"
	exit
fi 

# Delete the file:
rm test.log
rm test.file
