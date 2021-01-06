#!/bin/bash

# Check if the user is root
if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit 1
fi

# Check if the api token is supplied
if [ $# -eq 0 ]
  then
    echo "A client ID and api token must be supplied"
    echo "Usage: sudo ./install.sh [CLIENT ID] [API TOKEN]"
    exit 1
fi

# Stop existing service
echo "Stopping existing Bakup Agent..."
service bakupagent stop

# Download dependencies for SSL
echo "Acquiring SSL dependencies..."
apt-get -qq install openssl ca-certificates -y

# Create the directories needed for storing files and the binary
echo "Creating directories..."
mkdir -p /opt/bakupagent
mkdir -p /etc/opt/bakupagent

# Get the user's ID
echo "Obtaining user ID..."
USER_NAME=$(logname)
USER_ID=$(id -u "$USER_NAME")
touch /etc/opt/bakupagent/USER_ID
echo "$USER_ID" | tee /etc/opt/bakupagent/USER_ID > /dev/null

# Create the credentials file for the user to populate
echo "Populating the client ID..."
CLIENT_ID=$1
touch /etc/opt/bakupagent/CLIENT_ID
echo "$CLIENT_ID" | tee /etc/opt/bakupagent/CLIENT_ID > /dev/null
chown "$USER_NAME" /etc/opt/bakupagent/CLIENT_ID

# Create the credentials file for the user to populate
echo "Populating the API token..."
API_TOKEN=$2
touch /etc/opt/bakupagent/API_TOKEN
echo "$API_TOKEN" | tee /etc/opt/bakupagent/API_TOKEN > /dev/null
chown "$USER_NAME" /etc/opt/bakupagent/API_TOKEN

# Create the service file to manage the service
echo "Making service file for systemd..."
echo "[Unit]
Description=Bakup Agent
After=network.target
StartLimitIntervalSec=0
StartLimitBurst=5

[Service]
Type=simple
ExecStart=/opt/bakupagent/bakupagent
Restart=on-failure
RestartSec=5s

[Install]
WantedBy=multi-user.target" | tee /etc/systemd/system/bakupagent.service > /dev/null

# Reload the daemons
systemctl daemon-reload

# Download the executable
echo "Downloading Bakup Agent..."
wget -q localhost/latest/agent -O /opt/bakupagent/bakupagent
chmod +x /opt/bakupagent/bakupagent

# Get rclone binary
echo "Collecting rclone binary..."
wget -q localhost/latest/rclone -O /opt/bakupagent/rclone
chmod +x /opt/bakupagent/rclone

# Get the uninstall script
echo "Getting uninstall script..."
wget -q localhost/latest/uninstall.sh -O /opt/bakupagent/uninstall.sh
chmod +x /opt/bakupagent/uninstall.sh

# Start the service
echo "Starting Bakup service..."
systemctl enable bakupagent
service bakupagent start

echo "DONE."