# Bakup Agent
This is the backup agent for [bakup.io](https://bakup.io)

![Bakup Agent CI](https://github.com/Superbition/Bakup-Agent/workflows/Bakup%20Agent%20CI/badge.svg)

## Automatic Installation
1. Download ```install.sh```
2. Ensure you're logged in as the user that you would like the agent to run as
3. Run ```sudo ./install.sh [YOUR-CLIENT-ID] [YOUR-API-KEY]```

## Manual Installation
1. Three configuration files must be created

   ```
   mkdir -p /etc/opt/bakupagent
   touch /etc/opt/bakupagent/{USER_ID,CLIENT_ID,API_TOKEN}
   ```

2. Populate ```USER_ID``` with the ID of the user you wish bakup agent to run under, then create ```CLIENT_ID``` and 
```API_TOKEN``` with your Client ID and API Token, respectively.

    **NOTE:** Ensure that the ```CLIENT_ID``` and ```API_TOKEN``` files are writable by the user that will run the agent

3. Copy the Bakup Agent and rclone binaries in to ```/opt/bakupagent/```.

4. After making the binaries executable, you can start the agent - ```./opt/bakupagent/bakupagent``` 

    **NOTE:** The Bakup Agent can be run with the ```-d``` argument to enable debug messages.


## Directory Tree

```
/
├── etc/
│   └── opt/
│       └── bakupagent/
│           ├── USER_ID
│           ├── CLIENT_ID
│           └── API_TOKEN
└── opt/
    └── bakupagent/
        ├── bakupagent
        └── rclone
```