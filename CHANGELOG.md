## [v5.0.0 (2021-05-08)](https://github.com/Superbition/Bakup-Agent/releases/tag/v5.0.0)

### Added
- Agent sends a ping during first setup to provide details of the host OS to Bakup.io (#58)
- Agent version is now sent in headers to ensure the agent is kept up to date (#61)
- Added a self-updating feature for effortless agent updates (#64)

### Changed
- Add clean up stage to jobs that are sent to the agent that will run regardless of the success of the backup (#55)
- Added ability to elevate privileges for jobs that require it, such as upgrading (#60)