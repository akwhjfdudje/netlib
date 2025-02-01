#!/bin/bash
# https://unix.stackexchange.com/questions/330086/send-random-data-over-tcp-for-a-while-and-count-how-many-bytes-were-sent
# Test for receiving large amount of random data 

# Kill processes before beginning:
killall nc 1>/dev/null 2>&1 
killall ncc_test 1>/dev/null 2>&1 
SIZE=479114

# Starting the server:
timeout 10 bin/ncc_test receivelarge $SIZE &> test.log &

# Wait:
sleep 0.1

# Creating random data
dd if=/dev/urandom count=1 bs=$SIZE of=./random.data 2>/dev/null 1>&2
SIZE=$(wc -c < random.data)

# Getting results
CLIENT=$(nc -w2 127.0.0.1 9001 < random.data) 

SERVER=$(cat test.log)

STATUS=$(cmp --silent test.file random.data; echo $?)

# Checking results:
if [ "$STATUS" = "0" ]; then
	echo -e "${GREEN}[+]${NC} Received data successfully"
else
	echo -e "${RED}[-]${NC} Failed to receive data"
	printf "[?] Server output: \n"
	printf "$SERVER\n"
	printf "[?] Client output: \n"
	printf "$CLIENT\n"
	exit
fi 

# Delete files:
rm test.log
rm test.file
rm random.data
