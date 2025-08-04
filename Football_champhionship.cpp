#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype> // for tolower() function 

using namespace std;

// Team class representing a football team
class Team {
public:
    string name;
    double value;
    int wins;
    int draws;
    int losses;

    Team(string name = "", double value = 0.0)
        : name(name), value(value), wins(0), draws(0), losses(0) {}

    // Calculate total points (3 for win, 1 for draw)
    int getPoints() const {
        return wins * 3 + draws;
    }

    // Display team statistics
    void display() const {
        cout << name << " | Val: " << value << " mil | W: " << wins 
             << " D: " << draws << " L: " << losses << " | Pts: " << getPoints() << endl;
    }
};

// Championship class managing the entire league
class Championship {
private:
    string name;
    vector<Team> teams;
    int stagesCompleted;

public:
    Championship(string name = "") : name(name), stagesCompleted(0) {}

    // Add a new team to the championship
    void addTeam(const Team& team) {
        teams.push_back(team);
    }

    // Remove a team (relegation)
    void relegateTeam(const string& teamName) {
        auto it = find_if(teams.begin(), teams.end(),
            [&](const Team& t) { return t.name == teamName; });
        
        if (it != teams.end()) {
            teams.erase(it);
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

    // Update team stats after a match
    void updateTeamStats(const string& teamName, int wins, int draws, int losses) {
        for (Team& team : teams) {
            if (team.name == teamName) {
                team.wins = wins;
                team.draws = draws;
                team.losses = losses;
                return;
            }
        }
        cout << "Team not found!\n";
    }

    // Display standings sorted by points
    void displayStandings() {
        vector<Team> sorted = teams;
        sort(sorted.begin(), sorted.end(), 
            [](const Team& a, const Team& b) {
                return a.getPoints() > b.getPoints();
            });
        
        cout << "\n--- " << name << " Standings (Stage: " << stagesCompleted << ") ---\n";
        for (size_t i = 0; i < sorted.size(); i++) {
            cout << i + 1 << ". ";
            sorted[i].display();
        }
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

    // Display standings sorted by team value
    void displayValueStandings() {
        vector<Team> sorted = teams;
        sort(sorted.begin(), sorted.end(), 
            [](const Team& a, const Team& b) {
                return a.value > b.value;
            });
        
        cout << "\n--- " << name << " Value-Based Standings ---\n";
        for (size_t i = 0; i < sorted.size(); i++) {
            cout << i + 1 << ". " << sorted[i].name 
                 << " | Val: " << sorted[i].value << " mil\n";
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
        
        cout << "\n--- Performance Comparison Report ---\n";
        cout << "Team | Position (Points) | Position (Value) | Difference\n";
        cout << "-------------------------------------------------------\n";
        
        for (const Team& team : teams) {
            int pointsPos = -1, valuePos = -1;
            
            for (size_t i = 0; i < pointsSorted.size(); i++) {
                if (pointsSorted[i].name == team.name) pointsPos = i + 1;
                if (valueSorted[i].name == team.name) valuePos = i + 1;
            }
            
            cout << team.name << " | " << pointsPos << " | " << valuePos 
                 << " | " << (pointsPos - valuePos) << endl;
        }
    }

    // Complete a stage (matchday)
    void completeStage() {
        stagesCompleted++;
        cout << "Stage " << stagesCompleted << " completed!\n";
        displayStandings();
    }

    // Getter for championship name
    string getName() const { return name; }

    // Save championship data to file
    void saveToFile(ofstream& file) {
        file << name << '\n';
        file << teams.size() << ' ' << stagesCompleted << '\n';
        for (const Team& team : teams) {
            file << team.name << ',' << team.value << ',' 
                 << team.wins << ',' << team.draws << ',' << team.losses << '\n';
        }
    }

    // Load championship data from file
    void loadFromFile(ifstream& file) {
        getline(file, name);
        
        int teamCount;
        file >> teamCount >> stagesCompleted;
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
            
            if (tokens.size() == 5) {
                Team team(tokens[0], stod(tokens[1]));
                team.wins = stoi(tokens[2]);
                team.draws = stoi(tokens[3]);
                team.losses = stoi(tokens[4]);
                teams.push_back(team);
            }
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
        cout << i + 1 << ". " << championships[i].getName() << endl;
    }
    
    int choice;
    cout << "Select championship: ";
    cin >> choice;
    cin.ignore();
    
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
    cin >> value;
    cin.ignore();
    
    currentChampionship->promoteTeam(Team(name, value));
}

void updateTeamStats() {
    if (!currentChampionship) {
        cout << "No championship selected!\n";
        return;
    }
    
    string name;
    int wins, draws, losses;
    cout << "Enter team name: ";
    getline(cin, name);
    cout << "Enter wins: ";
    cin >> wins;
    cout << "Enter draws: ";
    cin >> draws;
    cout << "Enter losses: ";
    cin >> losses;
    cin.ignore();
    
    currentChampionship->updateTeamStats(name, wins, draws, losses);
}

void completeStage() {
    if (!currentChampionship) {
        cout << "No championship selected!\n";
        return;
    }
    
    currentChampionship->completeStage();
}

void displayTeamPosition() {
    if (!currentChampionship) {
        cout << "No championship selected!\n";
        return;
    }
    
    string name;
    cout << "Enter team name: ";
    getline(cin, name);
    
    int position = currentChampionship->getTeamPosition(name);
    if (position != -1) {
        cout << name << " is in position #" << position << endl;
    } else {
        cout << "Team not found!\n";
    }
}

// Main application menu
int main() {
    const string FILENAME = "championship_data.txt";
    int choice;
    
    do {
        cout << "\n===== Romanian Football Championship Manager =====\n";
        cout << "Current Championship: ";
        cout << (currentChampionship ? currentChampionship->getName() : "None") << endl;
        cout << "1. Create New Championship\n";
        cout << "2. Select Championship\n";
        cout << "3. Relegate Team\n";
        cout << "4. Promote Team\n";
        cout << "5. Update Team Statistics\n";
        cout << "6. Complete Stage & Show Standings\n";
        cout << "7. Show Team Position\n";
        cout << "8. Show Value-Based Standings\n";
        cout << "9. Generate Comparison Report\n";
        cout << "10. Save Data\n";
        cout << "11. Load Data\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: createNewChampionship(); break;
            case 2: selectChampionship(); break;
            case 3: relegateTeam(); break;
            case 4: promoteTeam(); break;
            case 5: updateTeamStats(); break;
            case 6: completeStage(); break;
            case 7: displayTeamPosition(); break;
            case 8: 
                if (currentChampionship) currentChampionship->displayValueStandings(); 
                else cout << "No championship selected!\n";
                break;
            case 9: 
                if (currentChampionship) currentChampionship->generateComparisonReport(); 
                else cout << "No championship selected!\n";
                break;
            case 10: saveData(FILENAME); break;
            case 11: 
                loadData(FILENAME); 
                if (!championships.empty()) currentChampionship = &championships[0];
                break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);
    
    return 0;
}