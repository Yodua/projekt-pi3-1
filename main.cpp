#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <stack>
#include <ctime>

using namespace std;

vector<pair<int, int>> graf; // graf w postaci jednostronnych przejsc miedzy polami
set<int> uniqueChecker; // pomocnicza do vectora nodes, trzeba uzyc razem z wektorem, bo set nie jest indeksowany
set<int> visited;
vector<int> nodes; // wymaga uniqueChecker bo latwiej sprawdzic unikalnosc danych

bool foundTargetBFS = false;
bool foundTargetDFS = false;

// TODO: target jako input uzytkownika zamiast stalej
int target = 17;

bool checkIfInVector(const vector<int>& vec, int const value) {
    for (int const n : vec) {
        if (n == value) return true;
    }
    return false;
}

// Agent uniwersalny dla DFS i BFS
class Agent {
public:
    int id;
    int startingPosition;
    queue<int> q; // kolejka do BFS
    vector<int> neighbours;
    stack<int> s; // stos do DFS

    explicit Agent(int const i) {
        this->id = i;
        this->startingPosition = nodes[rand() % nodes.size()]; //zaczyna z losowego miejsca na grafie
        q.push(startingPosition);
        s.push(startingPosition);
    }

    // pojedynczy krok algorytmu BFS dla jednego agenta
    void BFSstep() {
        if (q.empty()) { //jesli kolejka jest pusta to agent przeszukal wszystko do czego mial dostep
            cout << "BFS: Agent" << this->id << " skonczyl swoja kolejke" << endl;
            return;
        }

        // zdejmujey pierwszy element z kolejki jako obecny
        int const n = q.front();
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
            foundTargetBFS = true; // przerywamy algorytm, bo jeden z agentow znalazl cel
            return;
        }

        // jesli sasiednie pole nie bylo odwiedzone to dodajemy do kolejki
        for (int g : neighbours) {
            if (visited.insert(g).second) q.push(g);
        }
    }

    // pojedynczy krok algorytmu DFS dla jednego agenta
    void DFSstep() {
        if (s.empty()) { // jesli stos jest pusty to agent przeszukal wszystko do czego mial dostep
            cout << "DFS: Agent " << this->id << " skonczyl swoj stos" << endl;
            return;
        }

        // zdejmujemy ostatni element ze stosu jako obecny
        int const n = s.top();
        s.pop();
        visited.insert(n);

        neighbours.clear();

        for (pair<int, int> p : graf) {
            if (n == p.first) neighbours.push_back(p.second);
        }

        cout << "DFS: Agent" << this->id << " odwiedza pole " << n << endl;
        if (n == target) {
            cout << "DFS: Agent" << this->id << " odnalazl cel" << endl;
            foundTargetDFS = true; // przerywamy algorytm bo znaleziono cel
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
        agents.emplace_back(i);
    }

    // algorytm
    while (true) {
        bool found = false;
        for (Agent& a : agents) { //referencje do wszystkich agentow
            if (!a.q.empty()) { // nie ma po co sprawdzac pustej kolejki
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
        agents.emplace_back(i);
    }

    while (true) {
        bool found = false;
        for (Agent& a : agents) {
            if (!a.s.empty()) { // po co sprawdzac pusty stos
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
    srand(time(0)); // bez tego zimenna 'losowa' bedzie zawsze taka sama

    // sprawdzenie czy program ma plik podany jako argument
    if (argc != 2) {
        cerr << "Uzycie: " << argv[0] << " <nazwa_pliku>" << endl;
        return 1;
    }

    // odczyt z pliku
    ifstream plik(argv[1]);

    // sprawdzanie czy plik istnieje (lub czy nie ma jakichs innych problemow)
    if (!plik.is_open()) {
        cerr << "Nie mozna otworzyc pliku" << endl;
        return 1;
    }

    int sourceNode, targetNode;
    while (plik >> sourceNode >> targetNode) { // strumien wpisuje kolejne indeksy pol do x i y
        graf.emplace_back(sourceNode, targetNode); // x i y dodane na stale do par
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

    //pomiar BFS
    clock_t start = clock();
    multiAgentBFS(4);
    clock_t end = clock();
    double const executionTimeBFS = double(end - start) / CLOCKS_PER_SEC;

    visited.clear();
    cout << endl << endl;

    //pomiar DFS
    start = clock();
    multiAgentDFS(3);
    end = clock();
    double const executionTimeDFS = double(end - start) / CLOCKS_PER_SEC;

    cout << endl << endl;

    cout << "Czas wykonywania algorytmu BFS: " << executionTimeBFS << endl;
    cout << "Czas wykonywania algorytmu DFS: " << executionTimeDFS << endl;

    //porownanie czasowe
    if (executionTimeBFS > executionTimeDFS) {
        cout << "Algorytm DFS znalazl cel szybciej o " << executionTimeBFS - executionTimeDFS << endl;
    } else {
        cout << "Algorytm BFS znalazl cel szybciej o " << executionTimeDFS - executionTimeBFS << endl;
    }

    return 0;
}