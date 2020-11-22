# Bakup Agent
This is the backup agent for [bakup.io](https://bakup.io)

![Bakup Agent CI](https://github.com/Superbition/Bakup-Agent/workflows/Bakup%20Agent%20CI/badge.svg)

## Automatic Installation
1. Download ```install.sh```
2. Ensure you're logged in as the user that you would like the agent to run as
3. Run ```sudo ./install.sh [YOUR-API-KEY]```

## Manual Installation
1. Two configuration files must be created

   ```
   mkdir -p /etc/opt/bakupagent
   touch /etc/opt/bakupagent/{USER_ID,AUTH_TOKEN}
   ```

2. Populate ```USER_ID``` with the ID of the user you wish bakup agent to run under and ```AUTH_TOKEN``` with 
your authentication token

3. Copy the Bakup Agent and rclone binaries in to ```/opt/bakupagent/```.

4. After making the binaries executable, you can start the agent - ```./opt/bakupagent/bakupagent``` 

    The Bakup Agent can be run with the ```-d``` argument to enable debug messages.


## Directory Tree

```
/
├── etc/
│   └── opt/
│       └── bakupagent/
│           ├── USER_ID
│           └── AUTH_TOKEN
└── opt/
    └── bakupagent/
        ├── bakupagent
        └── rclone
```