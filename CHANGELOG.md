# Changelog

## 2018-12-01
### Added
- Support for new types: CONFIG_INT[8,16,32,64] and CONFIG_UINT[8,16,32,64]
- Support of already allocated strings (CONFIG_STRING)
- Support of non-allocated strings (CONFIG_NEW_STRING)

### Changed
- CONFIG_STRING changed behavior and will require refactoring to CONFIG_NEW_STRING.

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
