#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <stack>
#include <ctime>

using namespace std;

vector<pair<int, int>> graf;
set<int> uniqueChecker;
set<int> visited;
vector<int> nodes;

bool foundTargetBFS = false;
bool foundTargetDFS = false;

int target = 17;

bool checkIfInVector(vector<int> vec, int value) {
    for (int n : vec) {
        if (n == value) return true;
    }
    return false;
}

class Agent {
public:
    int id;
    int position;
    queue<int> q;
    vector<int> neighbours;
    stack<int> s;

    Agent(int i) {
        this->id = i;
        this->position = nodes[rand() % nodes.size()];
        q.push(position);
        s.push(position);
    }

    void BFSstep() {
        if (q.empty()) {
            cout << "BFS: Agent" << this->id << " skonczyl swoja kolejke" << endl;
            return;
        }

        int n = q.front();
        q.pop();
        visited.insert(n);
        neighbours.clear();

        // szukamy sasiadow
        for (pair<int, int> p : graf) {
            if (n == p.first) neighbours.push_back(p.second);
        }

        cout << "BFS: Agent" << this->id << " odwiedza pole " << n << endl;
        if (n == target) {
            cout << "BFS: Agent" << this->id << " odnalazl cel" << endl;
            foundTargetBFS = true;
            return;
        }

        // jesli sasiednie pole nie bylo odwiedzone to dodajemy do kolejki
        for (int g : neighbours) {
            if (visited.insert(g).second) q.push(g);
        }
    }

    void DFSstep() {
        if (s.empty()) {
            cout << "DFS: Agent " << this->id << " skonczyl swoj stos" << endl;
            return;
        }

        int n = s.top();
        s.pop();
        visited.insert(n);

        neighbours.clear();
        for (pair<int, int> p : graf) {
            if (n == p.first) neighbours.push_back(p.second);
        }

        cout << "DFS: Agent" << this->id << " odwiedza pole " << n << endl;
        if (n == target) {
            cout << "DFS: Agent" << this->id << " odnalazl cel" << endl;
            foundTargetDFS = true;
            return;
        }

        for (int g : neighbours) {
            if (visited.insert(g).second) s.push(g);
        }
    }
};

// BFS
void multiAgentBFS(int const agentCount) {

    // tworzenie agentow
    vector<Agent> agents;
    for (int i = 0; i < agentCount; i++) {
        agents.push_back(Agent(i));
    }

    while (true) {
        bool found = false;
        for (Agent& a : agents) {
            if (!a.q.empty()) {
                a.BFSstep();
                if (foundTargetBFS) {
                    cout << "Algorytm BFS zakonczyl dzialanie";
                    found = true;
                    break;
                }
            }
        }
        if (found) break;

        //konczenie petli w wypadku nie znalezienia celu
        bool anyActive = false;
        for (Agent& a : agents) {
            if (!a.q.empty()) {
                anyActive = true;
            }
        }

        if (!anyActive) {
            cout << "BFS: Nie znaleziono celu.\n"
                    "Cel nie istnieje lub agenci nie byli w stanie do niego dotrzec z ich pol startowych.\n"
                    "Sproboj ponownie lub zwieksz ilosc agentow" << endl;
            break;
        }
    }
}

// DFS
void multiAgentDFS(int const agentCount) {
    vector<Agent> agents;
    for (int i = 0; i < agentCount; i++) {
        agents.push_back(Agent(i));
    }

    while (true) {
        bool found = false;
        for (Agent& a : agents) {
            if (!a.s.empty()) {
                a.DFSstep();
                if (foundTargetDFS) {
                    cout << "Algorytm DFS zakonczyl dzialanie";
                    found = true;
                    break;
                }
            }
        }
        if (found) break;

        //konczenie petli w wypadku nie znalezienia celu
        bool anyActive = false;
        for (Agent& a : agents) {
            if (!a.s.empty()) {
                anyActive = true;
            }
        }

        if (!anyActive) {
            cout << "DFS: Nie znaleziono celu.\n"
                    "Cel nie istnieje lub agenci nie byli w stanie do niego dotrzec z ich pol startowych.\n"
                    "Sproboj ponownie lub zwieksz ilosc agentow" << endl;
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    srand(time(0));

    // sprawdzanie poprawnosci uruchomienia programu
    if (argc != 2) {
        cerr << "Uzycie: " << argv[0] << " <nazwa_pliku>" << endl;
        return 1;
    }

    // odczyt z pliku
    ifstream plik(argv[1]);

    // sprawdzanie czy plik sie otworzyl
    if (!plik.is_open()) {
        cerr << "Nie mozna otworzyc pliku" << endl;
        return 1;
    }

    int x, y; // tymczasowe zmienne
    while (plik >> x >> y) { // strumien wpisuje kolejne indeksy pol do x i y
        graf.push_back({x, y}); // x i y dodane na stale do par
    }

    // wyswietlenie par z grafu
    for (pair<int, int> p : graf) {
        cout << p.first << " " << p.second << endl;
    }

    // tworzenie listy wszystkich miejsc grafu
    for (pair<int, int> p : graf) {
        if (uniqueChecker.insert(p.first).second) nodes.push_back(p.first); //.second zwraca wartosc boolean, w zaleznosci od tego czy instrukcja sie powiodla
        if (uniqueChecker.insert(p.second).second) nodes.push_back(p.second);
    }
    clock_t start;
    clock_t end;

    start = clock();
    multiAgentBFS(4);
    end = clock();
    double executionTimeBFS = double(end - start) / CLOCKS_PER_SEC;

    visited.clear();
    cout << endl << endl;

    start = clock();
    multiAgentDFS(3);
    end = clock();
    double executionTimeDFS = double(end - start) / CLOCKS_PER_SEC;

    cout << endl << endl;

    cout << "Czas wykonywania algorytmu BFS: " << executionTimeBFS << endl;
    cout << "Czas wykonywania algorytmu DFS: " << executionTimeDFS << endl;
    if (executionTimeBFS > executionTimeDFS) {
        cout << "Algorytm DFS znalazl cel szybciej o " << executionTimeBFS - executionTimeDFS << endl;
    } else {
        cout << "Algorytm BFS znalazl cel szybciej o " << executionTimeDFS - executionTimeBFS << endl;
    }

    return 0;
}