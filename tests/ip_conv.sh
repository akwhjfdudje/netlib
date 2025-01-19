#!/bin/bash

# Run the program with the given IP address:
RES=$(bin/ncc_test convert) 

# Get results:
if [ "$( echo $RES | awk '{ print $4 }' )" = "1" ]; then
	echo -e "${GREEN}[+]${NC} Converted to integer"
else
	echo -e "${RED}[-]${NC} Couldn't convert to integer"
	echo "[?] Output: $RES"
fi
if [ "$( echo $RES | awk '{ print $4 }' )" = "1" ]; then
	echo -e "${GREEN}[+]${NC} Converted to IP"
else
	echo -e "${RED}[-]${NC} Couldn't convert to IP"
	echo "[?] Output: $RES"
fi
if [ "$( echo $RES | awk '{ print $13 }' )" = "127.0.0.1" ]; then
	echo -e "${GREEN}[+]${NC} Converted back to IP"
else
	echo -e "${RED}[-]${NC} Couldn't convert back to IP"
	echo "[?] Output: $RES"
fi
