# ⚽ Romanian Football Championship Manager

This is a console-based C++ application for managing football championships. It supports creating, updating, and tracking multiple leagues, including team promotion, relegation, matchday progress, and performance comparison reports.

---

## ✨ Features

- Create and manage multiple championships
- Add (promote) or remove (relegate) teams
- Update team match results (wins, draws, losses)
- Track league stages and display standings
- Sort standings by:
  - Match points
  - Team market value
- Compare actual performance vs market value expectations
- Save/load championship data to/from file

---

## 🛠 Technologies

- Language: C++
- File handling: `.txt` data files
- STL: `vector`, `string`, `fstream`, `algorithm`, `sstream`

---

## 🧾 How It Works

### 👥 Team Model

Each `Team` contains:
- Name
- Market value (in millions)
- Wins, draws, losses
- Points are calculated: `Points = 3 × Wins + 1 × Draws`

---

### 🏆 Championship Functionalities

A `Championship` can:

- Add teams (`promoteTeam`)
- Remove teams (`relegateTeam`)
- Update team stats after matches
- Track number of completed stages
- Display:
  - Points-based standings
  - Value-based standings
  - Performance comparison reports
- Save or load all data using text files

---

## 📋 Sample Menu

===== Romanian Football Championship Manager =====

Current Championship: Liga 1

1. Create New Championship

2. Select Championship

3. Relegate Team

4. Promote Team

5. Update Team Statistics

6. Complete Stage & Show Standings

7. Show Team Position

8. Show Value-Based Standings

9. Generate Comparison Report

10. Save Data

11. Load Data

12. Exit

---

## 💾 Sample `championship_data.txt`

```txt
2
Liga 1
16 4
Universitatea Craiova,100.0,3,1,0
Farul Constanta,95.0,3,1,0
AFC UTA Arad,90.0,2,2,0
Fotbal Club Rapid 1923 SA,85.0,2,1,0
ACS Campionii Fotbal Club Arges,80.0,2,0,2
AFC Botosani,75.0,1,2,0
AS FC Universitatea Cluj,70.0,1,2,1
SC Dinamo 1948 SA,65.0,1,2,1
ACS Petrolul 52,60.0,1,1,2
ACS Suporter Club Otelul Galati,55.0,1,1,1
SC Fotbal Club FCSB SA,50.0,1,1,2
FC CFR 1907 Cluj SA,45.0,1,1,2
AFC Unirea 04 Slobozia,40.0,1,0,2
AFC Hermannstadt,35.0,0,3,1
FC Metaloglobus Bucuresti,30.0,0,1,3
AFK Csikszereda,25.0,0,1,3
Liga 2
20 1
Steaua Bucuresti,50.0,1,0,0
CS Concordia Chiajna,48.0,1,0,0
CS Afumati,46.0,1,0,0
SC FC Voluntari SA,44.0,1,0,0
CSC Dumbravita,42.0,1,0,0
Asociatia Club Sportiv Muncitoresc Resita,40.0,1,0,0
FC Bihor Oradea,38.0,1,0,0
AFC Chindia Targoviste,36.0,0,1,0
ACS Sepsi OSK Sfantu Gheorghe,34.0,0,1,0
ACS FC Corvinul Hunedoara 1921,32.0,0,1,0
CSM Slatina,30.0,0,1,0
Clubul Sportiv Gloria Bistrita,28.0,0,1,0
AFC ASA Tg.Mures,26.0,0,1,0
ACS FC Bacau,24.0,0,0,1
CS Dinamo Bucuresti,22.0,0,0,1
CSM Olimpia Satu Mare,20.0,0,0,1
AFC Metalul Buzău,18.0,0,0,1
CSC 1599 Selimbar,16.0,0,0,1
CS Tunari,14.0,0,0,1
AFC Campulung - Muscel 2022,12.0,0,0,1
```

---

## 📈 Comparison Report Example

--- Performance Comparison Report ---

Team | Position (Points) | Position (Value) | Difference
-------------------------------------------------------
Farul Constanta | 2 | 2 | 0

AFC UTA Arad | 3 | 3 | 0

FC CFR 1907 Cluj SA | 12 | 12 | 0

...

---

## 🔜 To-Do / Future Improvements
⏳ Input Validation – Better handling of invalid inputs

⏳ Match Simulation – Simulate matches between teams

⏳ Fixtures Generation – Automatic generation of match schedules

⏳ Advanced Statistics – Goal difference and goals scored

⏳ Team Search – Case-insensitive team searching

⏳ Data Export – Export standings to CSV

⏳ Error Handling – More comprehensive error checking

⏳ User Experience – Improved menu navigation

---

## 👨‍💻 Author
Created by Cristi

This project simulates football league management for educational purposes using modern C++ file and class handling.
