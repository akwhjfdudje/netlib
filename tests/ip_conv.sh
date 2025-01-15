#!/bin/bash

# Run the program with the given IP address:
RES=$(bin/ncc_test 127.0.0.1) 

# Get results:
if [ "$( echo $RES | awk '{ print $4 }' )" = "1" ]; then
	echo "[+] Converted to integer"
else
	echo "[-] Couldn't convert to integer"
	echo "[?] Output: $RES"
fi
if [ "$( echo $RES | awk '{ print $4 }' )" = "1" ]; then
	echo "[+] Converted to IP"
else
	echo "[-] Couldn't convert to IP"
	echo "[?] Output: $RES"
fi
if [ "$( echo $RES | awk '{ print $13 }' )" = "127.0.0.1" ]; then
	echo "[+] Converted back to IP"
else
	echo "[-] Couldn't convert back to IP"
	echo "[?] Output: $RES"
fi
