#!/bin/bash

# Client example test

# Kill processes before beginning:
killall nc 1>/dev/null 2>&1 
killall ncc_test 1>/dev/null 2>&1 

# Start netcat server:
timeout 10 bash -c "nc -lp 8080 > tmp.log" &

sleep 0.1

# Start client:
CLIENT=$(bin/ncc_test connect)

# Get results
NC_SERVER=$(cat tmp.log)

# Check results:

if [ "$NC_SERVER" = "client test" ]; then
	echo "[+] Passed client test"
else
	printf "[-] Failed client test: \n"
	printf "[?] Server output: \n$NC_SERVER\n"
	printf "[?] Client output: \n$CLIENT\n"
fi

# Delete the file:
rm tmp.log
