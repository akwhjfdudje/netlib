#!/bin/bash

# Client example test

# Kill processes before beginning:
killall nc 1>/dev/null 2>&1 
killall ncc_test 1>/dev/null 2>&1 

# Start netcat server:
timeout 10 bash -c "nc -lp 8080 > tmp.log" &

sleep 1

# Start client:
CLIENT=$(bin/ncc_test connect)

# Get results
NC_SERVER=$(cat tmp.log)

# Check results:

if [ "$NC_SERVER" = "client test" ]; then
	echo "[+] Passed client test"
else
	echo "[-] Failed client test:"
	echo "[?] Server output: $NC_SERVER"
	echo "[?] Client output: $CLIENT"
fi

# Delete the file:
rm tmp.log
