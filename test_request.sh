#!/bin/bash

IP_ADDRESS="192.168.1.48"

SECONDS=0
DURATION=60

while [ $SECONDS -lt $DURATION ]; do
    RESPONSE=$(curl -s "$IP_ADDRESS")
    echo "Response at $(date +%H:%M:%S):"
    echo "$RESPONSE"
    sleep 0.5  # Adjust the delay between requests if needed
    SECONDS=$((SECONDS + 1))
done