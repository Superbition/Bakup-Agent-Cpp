#!/bin/bash

# Check if the user is root
if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit 1
fi

LOCAL_UNINSTALL=false

# If this is a local uninstall (like when doing an upgrade), don't report the uninstall to Bakup
if [ $# -gt 0 ]
then
  if [ $1 == "--local-uninstall" ]
    then
    LOCAL_UNINSTALL=true
  fi
fi

# Stop existing service
echo "Stopping Bakup Agent..."
service bakupagent stop
systemctl disable bakupagent

if ! $LOCAL_UNINSTALL
then
  # Remove agent from Bakup
  echo "Removing agent from Bakup..."
  CLIENT_ID=$(cat /etc/opt/bakupagent/CLIENT_ID)
  API_TOKEN=$(cat /etc/opt/bakupagent/API_TOKEN)
  wget -O/dev/null -q "https://bakup.io/api/agent/v1/uninstall?client_id=$CLIENT_ID&api_token=$API_TOKEN"

  # Delete the bakupagent user
  userdel bakupagent
fi

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
exit 0
