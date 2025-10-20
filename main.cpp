#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

vector<vector<string>> graf;

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



    return 0;
}