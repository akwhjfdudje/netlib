#!/bin/bash

# This test sends a large amount of data to a server

# Cleanup:
killall nc 1>/dev/null 2>&1 
killall ncc_test 1>/dev/null 2>&1 

# Starting the server, piping out to the file:
timeout 20 nc -lp 8080 > test.file &

# Wait a bit:
sleep 0.1

# Size of stream:
SIZE=21471433

# Sending the file:
CLIENT=$(bin/ncc_test sendlarge $SIZE) 

# Getting and checking results:
RES=$(cat test.file)

if [ "$(wc -c < test.file)" = "$SIZE" ]; then
	echo "[+] Sent data successfully"
else
	echo "[-] Couldn't send data"
	printf "[?] Server output:\n"
	printf "$RES\n"
	printf "[?] Client output:\n"
	printf "$CLIENT\n"
	exit
fi

# Deleting the file:
rm test.file
