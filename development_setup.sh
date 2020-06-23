# Create the user that bakup will run as
sudo useradd bakup-agent

# Create the directories needed for storing files and the binary
sudo mkdir -p /etc/bakupagent
sudo mkdir -p /user/bin/bakupagent

# Create the credentials file for the user to populate
touch /etc/bakupagent/AUTH_TOKEN

# Create the service file to manage the service
echo "[Unit]
Description=Bakup Agent
After=network.target
StartLimitIntervalSec=0

[Service]
Type=simple
User=bakup-agent
ExecStart=/usr/bin/bakup-agent

[Install]
WantedBy=multi-user.target" | sudo tee /etc/systemd/system/bakupagent.service > /dev/null
