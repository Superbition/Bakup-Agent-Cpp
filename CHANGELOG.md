## [v5.0.6 (2021-06-16)](https://github.com/Superbition/Bakup-Agent/releases/tag/v5.0.6)

### Fixed
- Close left over pipes after a job is complete (#80)
- Don't save outputs for wget that are status updates to Bakup.io (#81)
- Make the update script kill itself to not leave any processes in the control group (#82)

## [v5.0.5 (2021-05-30)](https://github.com/Superbition/Bakup-Agent/releases/tag/v5.0.5)

### Fixed
- Do a full restart on install to fix upgrading issues (#78)

## [v5.0.4 (2021-05-30)](https://github.com/Superbition/Bakup-Agent/releases/tag/v5.0.4)

### Fixed
- Fixed an issue where jobs would only run when the agent was in debug mode (#76)

## [v5.0.3 (2021-05-30)](https://github.com/Superbition/Bakup-Agent/releases/tag/v5.0.3)

### Added
- Added upgrade path that doesn't remove agent from [Bakup.io](https://bakup.io) (#74)

## [v5.0.2 (2021-05-29)](https://github.com/Superbition/Bakup-Agent/releases/tag/v5.0.2)

### Fixed
- Updated the URL in the update script to pull down the install script (#72)

## [v5.0.1 (2021-05-28)](https://github.com/Superbition/Bakup-Agent/releases/tag/v5.0.1)

### Fixed
- Add libcurl as a dependency in the install script for greater compatibility (#70)

## [v5.0.0 (2021-05-08)](https://github.com/Superbition/Bakup-Agent/releases/tag/v5.0.0)

### Added
- Agent sends a ping during first setup to provide details of the host OS to Bakup.io (#58)
- Agent version is now sent in headers to ensure the agent is kept up to date (#61)
- Added a self-updating feature for effortless agent updates (#64)

### Changed
- Add clean up stage to jobs that are sent to the agent that will run regardless of the success of the backup (#55)
- Added ability to elevate privileges for jobs that require it, such as upgrading (#60)