#!/bin/bash

# Server example test:

# NOTE: this test does not check if a client has connected to the server,
# 		it will assume that the client is connecting to the correct address and port.

# Kill processes before beginning:
killall nc 1>/dev/null 2>&1 
killall ncc_test 1>/dev/null 2>&1 

# Start server test:
exec 3< <(echo $(timeout 10 bin/ncc_test serve) | awk '{ print $15 $16 }')

sleep 0.1

# Send message from nc client:
echo "server test" | nc 127.0.0.1 9001 

# bash voodoo magic:
read RES <& 3
exec 3>&-

# Check result:
if [ "$RES" = "servertest" ]; then
	echo "[+] Passed server test"
else
	echo "[-] Failed server test"
	printf "[?] Output: \n$RES\n"
fi
