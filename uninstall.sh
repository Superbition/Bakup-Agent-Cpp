#!/bin/bash

# Check if the user is root
if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit 1
fi

# Stop existing service
echo "Stopping Bakup Agent..."
service bakupagent stop
systemctl disable bakupagent

# Delete directories
echo "Deleting directories and data..."
rm -r /opt/bakupagent
rm -r /etc/opt/bakupagent

# Remove service file
echo "Removing service file..."
rm /etc/systemd/system/bakupagent.service

## Reload systemctl
echo "Reloading systemctl"
systemctl daemon-reload

echo "DONE."