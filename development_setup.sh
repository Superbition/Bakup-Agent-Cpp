# Check if the authentication token is supplied
if [[ $# -eq 0 ]]
  then
    echo "An authentication token must be supplied"
    exit 1
fi

# Create the directories needed for storing files and the binary
echo "Creating directories..."
sudo mkdir -p /opt/bakupagent
sudo mkdir -p /etc/opt/bakupagent

# Create the credentials file for the user to populate
echo "Populating the authentication token..."
AUTH_TOKEN=$1
sudo touch /etc/opt/bakupagent/AUTH_TOKEN
echo $AUTH_TOKEN | sudo tee /etc/opt/bakupagent/AUTH_TOKEN > /dev/null

# Get the user's ID
echo "Obtaining user ID..."
USER_ID=$(id -u)
sudo touch /etc/opt/bakupagent/USER_ID
echo $USER_ID | sudo tee /etc/opt/bakupagent/USER_ID > /dev/null

# Create the service file to manage the service
echo "Making service file for systemd..."
echo "[Unit]
Description=Bakup Agent
After=network.target
StartLimitIntervalSec=0

[Service]
Type=simple
ExecStart=/opt/bakupagent/bakupagent

[Install]
WantedBy=multi-user.target" | sudo tee /etc/systemd/system/bakupagent.service > /dev/null

echo "DONE."