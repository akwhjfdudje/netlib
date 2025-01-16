#!/bin/bash

# File sending test
# Sends /etc/passwd to the current directory

# Kill processes before beginning:
killall nc 1>/dev/null 2>&1 
killall ncc_test 1>/dev/null 2>&1 

# Starting the server, piping out to the file:
timeout 10 nc -lp 8080 > test.file &

# Wait a bit:
sleep 0.3 

# Sending the file:
CLIENT=$(bin/ncc_test sendfile) 

# Getting and checking results:
RES=$(cat test.file)

if [ "$(echo $RES | awk -F: '{ print $1 }')" = "root" ]; then
	echo "[+] Sent file successfully"
else
	echo "[-] Couldn't send file"
	printf "[?] Server output:\n"
	printf "$RES\n"
	printf "[?] Client output:\n"
	printf "$CLIENT\n"
fi

# Deleting the file:
rm test.file
