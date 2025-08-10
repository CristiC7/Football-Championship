# 📜 Changelog

All notable changes to this project will be documented in this file.  
This project follows [Semantic Versioning](https://semver.org/).

---

## [1.1.0] - 2025-08-09
### Added
- **Match Simulation**: Automatic simulation of match results based on team market value and home advantage.
- **Goal Difference & Goals Scored Tracking**: Teams now store goals for/against and calculate goal difference automatically.
- **Automatic Fixtures Generation**: Round-robin system with home and away matches.
- **Value-Based Standings**: Ranking teams by their market value.
- **Performance Comparison Report**: Compare actual points-based standings with value-based rankings.
- **Case-Insensitive Team Search**: Find teams regardless of case sensitivity.
- **CSV Export**: Export standings to a `.csv` file.
- **Input Validation**: Added checks for invalid or negative inputs.
- **Error Handling**: More robust handling for file operations and user input.
- **Menu Navigation Improvements**: Better handling of invalid menu selections.

### Changed
- Improved data saving/loading structure for championships.
- Updated standings display with dynamic column width and string truncation for long names.

### Fixed
- Bug where previously played matches were not resetting statistics before re-simulation.
- Fixture generation issues when there was an odd number of teams.

---

## [1.0.0] - Initial Release
- Basic championship creation and management.
- Add and remove teams (promotion/relegation).
- Manual updating of match results.
- Display points-based standings.
- Save and load championship data from text files.
