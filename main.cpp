#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

vector<pair<int, int>> graf;

int main(int argc, char* argv[]) {

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

    return 0;
}