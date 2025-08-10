#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <ctime>
#include <random>
#include <map>
#include <cctype>
#include <limits>

using namespace std;

// Helper function to truncate long strings
static string truncateString(const string& str, size_t width) {
    if (str.length() > width) {
        return str.substr(0, width - 3) + "...";
    }
    return str;
}

// Team class representing a football team
class Team {
public:
    string name;
    double value;
    int wins;
    int draws;
    int losses;
    int goalsFor;
    int goalsAgainst;

    Team(string name = "", double value = 0.0)
        : name(name), value(value), wins(0), draws(0), losses(0), 
          goalsFor(0), goalsAgainst(0) {}

    // Calculate total points (3 for win, 1 for draw)
    int getPoints() const {
        return wins * 3 + draws;
    }

    // Calculate goal difference
    int getGoalDifference() const {
        return goalsFor - goalsAgainst;
    }

    // Display team statistics
    void display() const {
        cout << name << " | Val: " << value << "M | W: " << wins 
             << " D: " << draws << " L: " << losses 
             << " | GF: " << goalsFor << " GA: " << goalsAgainst
             << " | GD: " << getGoalDifference() 
             << " | Pts: " << getPoints() << endl;
    }

    // Reset team statistics
    void resetStats() {
        wins = draws = losses = goalsFor = goalsAgainst = 0;
    }
};

// Match structure to store fixture information
struct Match {
    string homeTeam;
    string awayTeam;
    int homeGoals;
    int awayGoals;
    bool played;

    Match(string home, string away) 
        : homeTeam(home), awayTeam(away), homeGoals(0), awayGoals(0), played(false) {}
};

// Championship class managing the entire league
class Championship {
private:
    string name;
    vector<Team> teams;
    vector<vector<Match>> fixtures;
    int stagesCompleted;
    int totalStages;

    // Helper function to find team index
    int findTeamIndex(const string& teamName) {
        for (size_t i = 0; i < teams.size(); i++) {
            if (teams[i].name == teamName) {
                return i;
            }
        }
        return -1;
    }

public:
    Championship(string name = "") : name(name), stagesCompleted(0), totalStages(0) {}

    // Add a new team to the championship
    void addTeam(const Team& team) {
        teams.push_back(team);
    }

    // Remove a team (relegation)
    void relegateTeam(const string& teamName) {
        int index = findTeamIndex(teamName);
        if (index != -1) {
            teams.erase(teams.begin() + index);
            cout << teamName << " relegated successfully.\n";
        } else {
            cout << "Team not found!\n";
        }
    }

    // Add a new team (promotion)
    void promoteTeam(const Team& team) {
        teams.push_back(team);
        cout << team.name << " promoted successfully.\n";
    }

    // Generate fixtures using round-robin algorithm
    void generateFixtures() {
        fixtures.clear();
        if (teams.size() < 2) {
            cout << "Not enough teams to generate fixtures!\n";
            return;
        }

        // Create a copy of teams and remove dummy if odd number
        vector<string> teamNames;
        for (const Team& team : teams) {
            teamNames.push_back(team.name);
        }
        
        if (teamNames.size() % 2 != 0) {
            teamNames.push_back("BYE"); // Dummy team for bye weeks
        }

        int n = teamNames.size();
        totalStages = (n - 1) * 2; // Home and away
        
        // Generate rounds
        for (int round = 0; round < totalStages; round++) {
            vector<Match> roundMatches;
            for (int i = 0; i < n / 2; i++) {
                string home = teamNames[i];
                string away = teamNames[n - 1 - i];
                
                // Skip matches with dummy team
                if (home != "BYE" && away != "BYE") {
                    if (round % 2 == 0) {
                        roundMatches.push_back(Match(home, away));
                    } else {
                        roundMatches.push_back(Match(away, home));
                    }
                }
            }
            fixtures.push_back(roundMatches);
            
            // Rotate teams
            rotate(teamNames.begin() + 1, teamNames.begin() + n - 1, teamNames.end());
        }
        
        cout << "Generated " << totalStages << " stages with " 
             << fixtures[0].size() << " matches per stage\n";
    }

    // Display fixtures for a specific stage
    void displayFixtures(int stage) {
        if (stage < 0 || stage >= static_cast<int>(fixtures.size())) {
            cout << "Invalid stage number!\n";
            return;
        }

        cout << "\n--- " << name << " Stage " << stage + 1 << " Fixtures ---\n";
        for (const Match& match : fixtures[stage]) {
            cout << match.homeTeam << " vs " << match.awayTeam;
            if (match.played) {
                cout << " : " << match.homeGoals << "-" << match.awayGoals;
            }
            cout << endl;
        }
    }

    // Play a specific stage
    void playStage(int stage) {
        if (stage < 0 || stage >= static_cast<int>(fixtures.size())) {
            cout << "Invalid stage number!\n";
            return;
        }

        // Reset if already played
        if (fixtures[stage][0].played) {
            for (Match& match : fixtures[stage]) {
                // Find teams and reverse previous result
                int homeIndex = findTeamIndex(match.homeTeam);
                int awayIndex = findTeamIndex(match.awayTeam);
                
                if (homeIndex != -1 && awayIndex != -1) {
                    Team& home = teams[homeIndex];
                    Team& away = teams[awayIndex];
                    
                    // Reverse previous stats
                    home.goalsFor -= match.homeGoals;
                    home.goalsAgainst -= match.awayGoals;
                    away.goalsFor -= match.awayGoals;
                    away.goalsAgainst -= match.homeGoals;
                    
                    if (match.homeGoals > match.awayGoals) {
                        home.wins--;
                        away.losses--;
                    } else if (match.homeGoals < match.awayGoals) {
                        home.losses--;
                        away.wins--;
                    } else {
                        home.draws--;
                        away.draws--;
                    }
                }
            }
        }

        // Play matches
        for (Match& match : fixtures[stage]) {
            int homeIndex = findTeamIndex(match.homeTeam);
            int awayIndex = findTeamIndex(match.awayTeam);
            
            if (homeIndex != -1 && awayIndex != -1) {
                Team& home = teams[homeIndex];
                Team& away = teams[awayIndex];
                
                // Simulate match (simple simulation based on team value)
                double homeAdvantage = 1.2;
                double homeStrength = home.value * homeAdvantage;
                double awayStrength = away.value;
                double totalStrength = homeStrength + awayStrength;
                
                // Generate random goals (0-5)
                random_device rd;
                mt19937 gen(rd());
                poisson_distribution<> homeDist(homeStrength / 50);
                poisson_distribution<> awayDist(awayStrength / 50);
                
                match.homeGoals = min(5, homeDist(gen));
                match.awayGoals = min(5, awayDist(gen));
                match.played = true;
                
                // Update team statistics
                home.goalsFor += match.homeGoals;
                home.goalsAgainst += match.awayGoals;
                away.goalsFor += match.awayGoals;
                away.goalsAgainst += match.homeGoals;
                
                if (match.homeGoals > match.awayGoals) {
                    home.wins++;
                    away.losses++;
                } else if (match.homeGoals < match.awayGoals) {
                    home.losses++;
                    away.wins++;
                } else {
                    home.draws++;
                    away.draws++;
                }
            }
        }
        
        stagesCompleted = stage + 1;
        cout << "Stage " << stage + 1 << " completed!\n";
        displayStandings();
    }

    // Display standings sorted by points and goal difference
    void displayStandings() {
        vector<Team> sorted = teams;
        sort(sorted.begin(), sorted.end(), 
            [](const Team& a, const Team& b) {
                if (a.getPoints() != b.getPoints()) {
                    return a.getPoints() > b.getPoints();
                }
                if (a.getGoalDifference() != b.getGoalDifference()) {
                    return a.getGoalDifference() > b.getGoalDifference();
                }
                return a.goalsFor > b.goalsFor;
            });
        
        // Dynamic width calculation
        const int nameWidth = 50;  // Max width for team names
        const int baseWidth = 35;  // Width for other columns
        
        cout << "\n--- " << name << " Standings (Stage: " << stagesCompleted << ") ---\n";
        
        // Header
        cout << left << setw(4) << "Pos" 
             << setw(nameWidth) << "Team"
             << setw(5) << "Pts"
             << setw(4) << "W"
             << setw(4) << "D"
             << setw(4) << "L"
             << setw(5) << "GF"
             << setw(5) << "GA"
             << setw(5) << "GD"
             << endl;
        
        // Separator
        cout << string(4 + nameWidth + baseWidth, '-') << endl;
        
        for (size_t i = 0; i < sorted.size(); i++) {
            cout << left << setw(4) << i + 1 
                 << setw(nameWidth) << truncateString(sorted[i].name, nameWidth)
                 << setw(5) << sorted[i].getPoints()
                 << setw(4) << sorted[i].wins
                 << setw(4) << sorted[i].draws
                 << setw(4) << sorted[i].losses
                 << setw(5) << sorted[i].goalsFor
                 << setw(5) << sorted[i].goalsAgainst
                 << setw(5) << sorted[i].getGoalDifference()
                 << endl;
        }
    }

    // Export standings to CSV file
    void exportStandingsToCSV(const string& filename) {
        ofstream file(filename);
        if (!file) {
            cerr << "Error opening file for writing!\n";
            return;
        }
        
        vector<Team> sorted = teams;
        sort(sorted.begin(), sorted.end(), 
            [](const Team& a, const Team& b) {
                return a.getPoints() > b.getPoints();
            });
        
        file << "Position,Team,Points,Wins,Draws,Losses,GF,GA,GD\n";
        for (size_t i = 0; i < sorted.size(); i++) {
            file << i + 1 << ","
                 << sorted[i].name << ","
                 << sorted[i].getPoints() << ","
                 << sorted[i].wins << ","
                 << sorted[i].draws << ","
                 << sorted[i].losses << ","
                 << sorted[i].goalsFor << ","
                 << sorted[i].goalsAgainst << ","
                 << sorted[i].getGoalDifference() << "\n";
        }
        
        cout << "Standings exported to " << filename << endl;
    }

    // Get position of a specific team
    int getTeamPosition(const string& teamName) {
        vector<Team> sorted = teams;
        sort(sorted.begin(), sorted.end(), 
            [](const Team& a, const Team& b) {
                return a.getPoints() > b.getPoints();
            });
        
        for (size_t i = 0; i < sorted.size(); i++) {
            if (sorted[i].name == teamName) {
                return i + 1;
            }
        }
        return -1;
    }

    // Find team by name (case-insensitive)
    Team* findTeam(const string& teamName) {
        for (Team& team : teams) {
            if (equal(team.name.begin(), team.name.end(), teamName.begin(), teamName.end(),
                [](char a, char b) { 
                    return tolower(a) == tolower(b); 
                })) {
                return &team;
            }
        }
        return nullptr;
    }

    // Display standings sorted by team value
    void displayValueStandings() {
        vector<Team> sorted = teams;
        sort(sorted.begin(), sorted.end(), 
            [](const Team& a, const Team& b) {
                return a.value > b.value;
            });
        
        const int nameWidth = 50;
        
        cout << "\n--- " << name << " Value-Based Standings ---\n";
        cout << left << setw(4) << "Pos" 
             << setw(nameWidth) << "Team"
             << "Value (M€)\n";
        cout << string(4 + nameWidth + 15, '-') << endl;
        
        for (size_t i = 0; i < sorted.size(); i++) {
            cout << left << setw(4) << i + 1 
                 << setw(nameWidth) << truncateString(sorted[i].name, nameWidth)
                 << sorted[i].value << "M\n";
        }
    }

    // Compare actual standings vs value-based standings
    void generateComparisonReport() {
        vector<Team> pointsSorted = teams;
        vector<Team> valueSorted = teams;
        
        sort(pointsSorted.begin(), pointsSorted.end(), 
            [](const Team& a, const Team& b) {
                return a.getPoints() > b.getPoints();
            });
        
        sort(valueSorted.begin(), valueSorted.end(), 
            [](const Team& a, const Team& b) {
                return a.value > b.value;
            });
        
        const int nameWidth = 50;
        
        cout << "\n--- Performance Comparison Report ---\n";
        cout << left << setw(nameWidth) << "Team"
             << setw(10) << "Pts Pos"
             << setw(10) << "Val Pos"
             << "Diff\n";
        cout << string(nameWidth + 30, '-') << endl;
        
        for (const Team& team : teams) {
            int pointsPos = -1, valuePos = -1;
            
            for (size_t i = 0; i < pointsSorted.size(); i++) {
                if (pointsSorted[i].name == team.name) pointsPos = i + 1;
                if (valueSorted[i].name == team.name) valuePos = i + 1;
            }
            
            cout << left << setw(nameWidth) << truncateString(team.name, nameWidth)
                 << setw(10) << pointsPos
                 << setw(10) << valuePos
                 << (pointsPos - valuePos) << endl;
        }
    }

    // Reset all statistics
    void resetChampionship() {
        for (Team& team : teams) {
            team.resetStats();
        }
        stagesCompleted = 0;
        for (auto& stage : fixtures) {
            for (Match& match : stage) {
                match.homeGoals = 0;
                match.awayGoals = 0;
                match.played = false;
            }
        }
        cout << "Championship statistics reset!\n";
    }

    // Getter for championship name
    string getName() const { return name; }

    // Get number of teams
    int getTeamCount() const { return teams.size(); }

    // Save championship data to file
    void saveToFile(ofstream& file) {
        file << name << '\n';
        file << teams.size() << ' ' << stagesCompleted << ' ' << totalStages << '\n';
        for (const Team& team : teams) {
            file << team.name << ',' << team.value << ',' 
                 << team.wins << ',' << team.draws << ',' 
                 << team.losses << ',' << team.goalsFor << ',' 
                 << team.goalsAgainst << '\n';
        }
        
        // Save fixtures
        file << fixtures.size() << '\n';
        for (const auto& stage : fixtures) {
            file << stage.size() << '\n';
            for (const Match& match : stage) {
                file << match.homeTeam << ',' << match.awayTeam << ','
                     << match.homeGoals << ',' << match.awayGoals << ','
                     << match.played << '\n';
            }
        }
    }

    // Load championship data from file
    void loadFromFile(ifstream& file) {
        getline(file, name);
        
        int teamCount;
        file >> teamCount >> stagesCompleted >> totalStages;
        file.ignore();  // Skip newline
        
        teams.clear();
        for (int i = 0; i < teamCount; i++) {
            string line;
            getline(file, line);
            stringstream ss(line);
            string token;
            
            vector<string> tokens;
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            
            if (tokens.size() == 7) {
                Team team(tokens[0], stod(tokens[1]));
                team.wins = stoi(tokens[2]);
                team.draws = stoi(tokens[3]);
                team.losses = stoi(tokens[4]);
                team.goalsFor = stoi(tokens[5]);
                team.goalsAgainst = stoi(tokens[6]);
                teams.push_back(team);
            }
        }
        
        // Load fixtures
        int stageCount;
        file >> stageCount;
        file.ignore();
        
        fixtures.clear();
        for (int i = 0; i < stageCount; i++) {
            int matchCount;
            file >> matchCount;
            file.ignore();
            
            vector<Match> stageMatches;
            for (int j = 0; j < matchCount; j++) {
                string line;
                getline(file, line);
                stringstream ss(line);
                vector<string> tokens;
                string token;
                
                while (getline(ss, token, ',')) {
                    tokens.push_back(token);
                }
                
                if (tokens.size() == 5) {
                    Match match(tokens[0], tokens[1]);
                    match.homeGoals = stoi(tokens[2]);
                    match.awayGoals = stoi(tokens[3]);
                    match.played = tokens[4] == "1";
                    stageMatches.push_back(match);
                }
            }
            fixtures.push_back(stageMatches);
        }
    }
};

// Global variables
vector<Championship> championships;
Championship* currentChampionship = nullptr;

// Save all championships to file
void saveData(const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file for writing!\n";
        return;
    }
    
    file << championships.size() << '\n';
    for (Championship& champ : championships) {
        champ.saveToFile(file);
    }
    
    cout << "Data saved successfully to " << filename << endl;
}

// Load all championships from file
void loadData(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file for reading!\n";
        return;
    }
    
    championships.clear();
    int count;
    file >> count;
    file.ignore();
    
    for (int i = 0; i < count; i++) {
        Championship champ;
        champ.loadFromFile(file);
        championships.push_back(champ);
    }
    
    currentChampionship = nullptr;
    cout << "Data loaded successfully from " << filename << endl;
}

// Main menu functions
void createNewChampionship() {
    string name;
    cout << "Enter championship name: ";
    getline(cin, name);
    
    championships.emplace_back(name);
    currentChampionship = &championships.back();
    cout << "Championship created successfully!\n";
}

void selectChampionship() {
    if (championships.empty()) {
        cout << "No championships available!\n";
        return;
    }
    
    cout << "\nAvailable Championships:\n";
    for (size_t i = 0; i < championships.size(); i++) {
        cout << i + 1 << ". " << championships[i].getName() 
             << " (" << championships[i].getTeamCount() << " teams)\n";
    }
    
    int choice;
    cout << "Select championship (0 to cancel): ";
    cin >> choice;
    cin.ignore();
    
    if (choice == 0) return;
    
    if (choice > 0 && choice <= static_cast<int>(championships.size())) {
        currentChampionship = &championships[choice - 1];
        cout << "Selected: " << currentChampionship->getName() << endl;
    } else {
        cout << "Invalid selection!\n";
    }
}

void relegateTeam() {
    if (!currentChampionship) {
        cout << "No championship selected!\n";
        return;
    }
    
    string name;
    cout << "Enter team to relegate: ";
    getline(cin, name);
    currentChampionship->relegateTeam(name);
}

void promoteTeam() {
    if (!currentChampionship) {
        cout << "No championship selected!\n";
        return;
    }
    
    string name;
    double value;
    cout << "Enter team name: ";
    getline(cin, name);
    cout << "Enter team value (in millions): ";
    
    // Input validation
    while (!(cin >> value) || value < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid value! Please enter a positive number: ";
    }
    cin.ignore();
    
    currentChampionship->promoteTeam(Team(name, value));
}

void findTeam() {
    if (!currentChampionship) {
        cout << "No championship selected!\n";
        return;
    }
    
    string name;
    cout << "Enter team name: ";
    getline(cin, name);
    
    Team* team = currentChampionship->findTeam(name);
    if (team) {
        cout << "\nTeam found:\n";
        team->display();
    } else {
        cout << "Team not found!\n";
    }
}

void generateFixtures() {
    if (!currentChampionship) {
        cout << "No championship selected!\n";
        return;
    }
    
    if (currentChampionship->getTeamCount() < 2) {
        cout << "Not enough teams to generate fixtures!\n";
        return;
    }
    
    currentChampionship->generateFixtures();
    cout << "Fixtures generated successfully!\n";
}

void playStage() {
    if (!currentChampionship) {
        cout << "No championship selected!\n";
        return;
    }
    
    int stage;
    cout << "Enter stage number to play: ";
    cin >> stage;
    cin.ignore();
    
    currentChampionship->playStage(stage - 1);
}

void resetChampionship() {
    if (!currentChampionship) {
        cout << "No championship selected!\n";
        return;
    }
    
    char confirm;
    cout << "Are you sure you want to reset all statistics? (y/n): ";
    cin >> confirm;
    cin.ignore();
    
    if (tolower(confirm) == 'y') {
        currentChampionship->resetChampionship();
    }
}

// Main application menu
int main() {
    const string FILENAME = "championship_data.txt";
    int choice;
    
    do {
        cout << "\n===== Romanian Football Championship Manager =====\n";
        cout << "Current Championship: ";
        if (currentChampionship) {
            cout << currentChampionship->getName() << " (Stage: " 
                 << currentChampionship->getName() << ")";
        } else {
            cout << "None";
        }
        cout << endl;
        
        cout << "1. Create New Championship\n";
        cout << "2. Select Championship\n";
        cout << "3. Relegate Team\n";
        cout << "4. Promote Team\n";
        cout << "5. Find Team Information\n";
        cout << "6. Generate Fixtures\n";
        cout << "7. Play Stage\n";
        cout << "8. Show Standings\n";
        cout << "9. Show Team Position\n";
        cout << "10. Show Value-Based Standings\n";
        cout << "11. Generate Comparison Report\n";
        cout << "12. Export Standings to CSV\n";
        cout << "13. Save Data\n";
        cout << "14. Load Data\n";
        cout << "15. Reset Championship Statistics\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        
        // Input validation
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }
        cin.ignore();
        
        switch (choice) {
            case 1: createNewChampionship(); break;
            case 2: selectChampionship(); break;
            case 3: relegateTeam(); break;
            case 4: promoteTeam(); break;
            case 5: findTeam(); break;
            case 6: generateFixtures(); break;
            case 7: playStage(); break;
            case 8: 
                if (currentChampionship) currentChampionship->displayStandings(); 
                else cout << "No championship selected!\n";
                break;
            case 9: 
                if (currentChampionship) {
                    string name;
                    cout << "Enter team name: ";
                    getline(cin, name);
                    int position = currentChampionship->getTeamPosition(name);
                    if (position != -1) {
                        cout << name << " is in position #" << position << endl;
                    } else {
                        cout << "Team not found!\n";
                    }
                } else {
                    cout << "No championship selected!\n";
                }
                break;
            case 10: 
                if (currentChampionship) currentChampionship->displayValueStandings(); 
                else cout << "No championship selected!\n";
                break;
            case 11: 
                if (currentChampionship) currentChampionship->generateComparisonReport(); 
                else cout << "No championship selected!\n";
                break;
            case 12:
                if (currentChampionship) {
                    string filename;
                    cout << "Enter filename (e.g., standings.csv): ";
                    getline(cin, filename);
                    currentChampionship->exportStandingsToCSV(filename);
                } else {
                    cout << "No championship selected!\n";
                }
                break;
            case 13: saveData(FILENAME); break;
            case 14: 
                loadData(FILENAME); 
                if (!championships.empty()) currentChampionship = &championships[0];
                break;
            case 15: resetChampionship(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 0);
    
    return 0;
}