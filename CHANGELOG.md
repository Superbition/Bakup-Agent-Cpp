## [v2.0.0 (2020-12-19)](https://github.com/Superbition/Bakup-Agent/releases/tag/v2.0.0)

### Added
- Installation steps added to the README (#27)
- Error reporting for failed HTTP requests (#29)
- Agent will retry failed network requests (#33)
- Agent can receive multiple jobs from one request and execute them at a given time (#35)
- Multi-threaded the execution of jobs to maintain responsiveness of the agent (#36)

### Changed
- CMake build executable renamed to `Bakup_Agent` (#28)
- Greatly simplified the main program loop for maintainability (#30)
- Debug class can print info, success and error text (#34)
- API requests now contain mandatory Client ID header (#37)
