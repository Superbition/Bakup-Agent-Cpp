## [v3.0.0 (2021-01-09)](https://github.com/Superbition/Bakup-Agent/releases/tag/v3.0.0)

### Added
- Agent checks received job for valid JSON (#38)
- Implemented the ResponseBuilder class to unify the generation of responses sent to Bakup.io (#41)
- User's can remotely change the agent's credentials (#43)
- Commands can now specify if the agent should skip the next poll time (#44)
- Add test to check a failed job fails gracefully (#46)
- Included an uninstall script (#48)
- Agent fails any request that doesn't have SSL and reports it to Bakup.io (#50)
- Hash check on all files at install (#51)

### Changed
- Change time format and clean up debug output (#39 & #42)
- Updated README to include new Client ID file (#45)

### Fixed
- Agent would segfault when attempting to process an empty job (#47)
- Removed redundant JSON parse (#49)
