## [v6.1.0 (2021-08-01)](https://github.com/Superbition/Bakup-Agent/releases/tag/v6.1.0)

### Added
- Always log when a job completes (#92)

### Changed
- During an uninstall, remove the `bakupagent` user (#93)

### Fixed
- If a job fails, add the failed command and error message to separate response elements (#94)

## [v6.0.0 (2021-07-31)](https://github.com/Superbition/Bakup-Agent/releases/tag/v6.0.0)

### Added
- Send current version of rClone in HTTP headers to automate updating (#84)
- The agent no longer runs as the installing user, instead a `bakupagent` user is created during install which is used 
  for executing jobs (#88)

### Changed
- Jobs now run in a shell that mimics that of an interactive shell that the user would open (#84)
- The install script now solely uses systemctl to manage the server, rather than also using System V's service command 
  (#89)

### Fixed
- Add additional checks that a shell for a job is started correctly (#87)
- Stop rClone outputting a warning about missing config files when checking its version (#90)
