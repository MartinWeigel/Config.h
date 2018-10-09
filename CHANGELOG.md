# Changelog

## 2018-10-09
### Added
- Support for inline comments: `key = value # Comment`

## 2018-10-08
### Changed
- `Config_load()` takes filename as a `const char*` instead of a `char*`
- The example-callback uses a sender object instead of a global variable

## 2018-09-28
### Added
- Created changelog file to track updates (`CHANGELOG.md`)

### Changed
- Renamed `Config_parse()` to `Config_load()`

## 2018-09-05
### Added
- Included license in header of `Config.h` for easier deployment

### Fixed
- Corrected comment for `Config_parse()`

## 2018-08-31
### Fixed
- Closed configuration file after parsing
