#!/bin/bash
# https://unix.stackexchange.com/questions/330086/send-random-data-over-tcp-for-a-while-and-count-how-many-bytes-were-sent

# Test for receiving /etc/passwd into the current directory

# Kill processes before beginning:
killall nc 1>/dev/null 2>&1 
killall ncc_test 1>/dev/null 2>&1 

# Starting the server:
timeout 10 bin/ncc_test receive > test.file &

# Wait:
sleep 0.1

# Getting results
CLIENT=$(cat /etc/passwd | nc 127.0.0.1 9001)

SERVER=$(cat test.file | awk -F: '{ print $1 }' | grep root)

# Checking results:
if [ "$SERVER" = "root" ]; then
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
rm test.file
