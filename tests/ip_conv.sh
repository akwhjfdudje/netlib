#!/bin/bash

# Run the program with the given IP address:
RES=$(bin/ncc_test 127.0.0.1) 

# Get results:
if [ "$( echo $RES | awk '{ print $4 }' )" = "1" ]; then
	echo "[+] Converted to integer"
fi
if [ "$( echo $RES | awk '{ print $4 }' )" = "1" ]; then
	echo "[+] Converted to IP"
fi
if [ "$( echo $RES | awk '{ print $13 }' )" = "127.0.0.1" ]; then
	echo "[+] Converted back to IP"
fi
