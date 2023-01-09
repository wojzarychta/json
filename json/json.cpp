#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

//------------------------------------------
void menu(fstream& fs);
void help();
bool open(fstream& fs, string& name);
bool validate(fstream& fs);
bool readJSON(string s);
int readValue(string s, int i = 0);
int readString(string s, int i = 0);
int readNum(string s, int i = 0);
int readArray(string s, int i = 0);
int readMember(string s, int i = 0);
int readObject(string s, int i = 0);
int readTrueFalseNull(string s, string t, int i = 0);
//------------------------------------------

int main()
{
    fstream file;

    menu(file);
    file.close();

    return 0;
}

/// <summary>
/// interaktywne menu do którego wprowadzamy komendy
/// </summary>
/// <param name="fs"> file stream z plikiem json</param>
void menu(fstream& fs)
{
    char choice;
    string command;

    do {  // nieskończona pętla
        system("cls");
        cout << ">> ";
        getline(cin >> ws, command); // używamy getline, aby whitespace nie przerywało odczytu 
        choice = command[0];  // pierwsza litera to polecenie
        int l = command.length();
        if (l > 2 && command[1] == ' ') 
            command = command.substr(2, command.length()-2);  // wyciągamy string po polecenie z nazwą pliku albo ścieżką
        switch (choice) {
        case 'h': {
            help();
        }
              break;
        case 'o': {
            if (open(fs, command))
                cout << "Plik otwarto pomyslnie" << endl << endl;
            else
                cout << "Blad podczas otwierania pliku" << endl << endl;
            system("pause");
        }
              break;
        case 'c': {
            bool isValid = validate(fs);
            if (isValid)
                cout << "* Plik jest prawidlowy *" << endl << endl;
            else
                cout << "* Plik jest bledny *" << endl << endl;
            system("pause");
        }
              break;
        default: {
            cout << "Niepoprawna komenda" << endl;
            system("pause");
        }
        }
    } while (true);
}


void help()
{
    system("cls");
    cout << "-------------- HELP --------------" << endl <<
        "o nazwa_pliku - otwarcie pliku o nazwie nazwa_pliku, operacja informuje użytkownika, czy plik został otwarty" << endl <<
        "c - operacja sprawdza czy plik jest poprawnym formatem JSON" << endl << endl <<
        "Autor: ........................................." << endl << endl;
    system("pause");
}

/// <summary>
/// otwiera plik 
/// </summary>
/// <param name="fs"> file stream przechowujący plik </param>
/// <param name="name"> nazwa pliku, ścieżka domyślna w katalogu z projektem </param>
/// <returns> true jeżeli prawidłowo otworzono plik </returns>
bool open(fstream& fs, string& name)
{
    if (fs.is_open()) fs.close();                  //gdy był wcześniej otwarty plik, zamyka go
    if (name.size() < 6 || name.compare(name.size() - 5, 5, ".json") != 0) {               //jeżeli plik nie jest w formacie .json
        return false;
    }
    fs.open(name, ios::in);        //otwarcie pliku
    if (!fs) {                     //kontrola otwarcia
        return false;
    }
    return true;
}

/// <summary>
/// sprawdza, czy plik jest poprawnym plikiem json
/// </summary>
/// <param name="fs"> fstream </param>
/// <returns> true, jeżeli plik jest poprawny </returns>
bool validate(fstream& fs) {
    // wczytaj cały plik do zmiennej json
    fs.seekg(0, ios::end);
    size_t size = fs.tellg();
    string json(size, ' ');
    fs.seekg(0);
    fs.read(&json[0], size);

    return readJSON(json);
}

/// <summary>
/// sprawdza czy ch jest znakiem WS
/// </summary>
/// <param name="ch"> znak sprawdzany </param>
/// <returns> true, jeżeli jest </returns>
bool isWS(char ch) {
    return ch == '\x20' || ch == '\x09' || ch == '\x0A' || ch == '\x0D';
}

/// <summary>
/// czyta cały plik json
/// </summary>
/// <param name="s"> plik json w postaci łańcucha znaków </param>
/// <returns> true, jeżeli plik jest prawidłowym plikiem json </returns>
bool readJSON(string s) {
    int size = s.length();
    int j = size - 1;
    int i = readValue(s);
    return i == j;
}

/// <summary>
/// czyta string w pliku json
/// </summary>
/// <param name="s"> plik json w postaci łańcucha znaków </param>
/// <param name="i"> indeks, od którego czytanie się zaczyna </param>
/// <returns> indeks, na którym string się kończy </returns>
int readString(string s, int i) 
{
    // s[i] == '"'
    int size = s.length();
    int j = i+1;

    for (;; j++) { // nieskończona pętla
        if (j == size) { // jeżeli indeks wyjechał poza plik; w innych funkcjach analogicznie
            string t = s.substr(i, j - i);
            cout << "BLAD: " << t << endl;
            return -1;
        }
        char c = s[j];
        if (c == '"')  // koniec stringa
            break;

        if (c == '\\') {  // przypadek wystąpienia znaku '\'
            if (j < size - 2 && (s[j + 1] == '\\' || s[j + 1] == '"' || s[j + 1] == 't'))
                j++;
            else {  // jeżeli po '\' jest coś innego
                string t = s.substr(i, j - i+1);
                cout << "BLAD: " << t << endl;
                return -1;
            }
        }
    }

    return j;
}

/// <summary>
/// czyta liczbę w pliku json
/// </summary>
/// <param name="s"> plik json w postaci łańcucha znaków </param>
/// <param name="i"> indeks, od którego czytanie się zaczyna </param>
/// <returns> indeks, na którym liczba się kończy </returns>
int readNum(string s, int i) {
    int size = s.length();
    int j = i;

    if (j >= size) {  
        string t = s.substr(i, j - i);
        cout << "BLAD: " << t << endl;
        return -1;
    }
       
    if (s[j] == '-')
        j++;
    if (s[j] == '0') {  // po zerze nie mogą być liczby, np. 02134 nie jest poprawną liczbą
        if (size - 1 > j && s[j + 1] != '.') {
            string t = s.substr(i, j - i + 1);
            cout << "BLAD: " << t << endl;
            return -1;
        }
    }
    else if (s[j] < '1' || s[j] > '9') {  // jeżeli znak nie jest cyfrą
        cout << "BLAD:  " << s << endl;
        return false;
    }
    j++;
    bool decimal = false;  // flaga, która mówi czy już wystąpiła kropka dziesiętna w liczbie
    for (;; j++) {
        if (j == size)
            return j - 1;

        char c = s[j];

        if ((c < '0' || c > '9') && c != '.')  //gdy skończy się liczba
            return j - 1;
        

        if (c == '.') {
            if (decimal)  // druga kropka, np. 10.23.4
            {
                string t = s.substr(i, j - i + 1);
                cout << "BLAD: " << t << endl;
                return -1;
            }
            else
                decimal = true;  // ustawia flagę
            continue;
        }
    }

    return -1;
}

/// <summary>
/// czyta wartość
/// </summary>
/// <param name="s"> plik json w postaci łańcucha znaków </param>
/// <param name="i"> indeks, od którego czytanie się zaczyna </param>
/// <returns> indeks, na którym wartość się kończy (razem z WS) </returns>
int readValue(string s, int i) 
{
    int size = s.length();
    int j = i;

    while (j < size && isWS(s[j]))  // omijanie WS
        j++;

    char c = s[j];
    switch (c) {
    case 't':
        j = readTrueFalseNull(s, "true", j);
        break;
    case 'f':
        j = readTrueFalseNull(s, "false", j);
        break;
    case 'n': 
        j = readTrueFalseNull(s, "null", j);
        break;
    case '"':
        j = readString(s, j);
        break;
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '-':
        j = readNum(s, j);
        break;
    case '[':
        j = readArray(s, j);
        break;
    case '{':
        j = readObject(s, j);
        break;
    default: {  // jeżeli jakikolwiek inny znak to zły plik 
        j = -1;
        string t = s.substr(i, j - i + 1);
        cout << "BLAD: " << t << endl;
    }
        
    }
    if (j == -1)
        return j;

    j++;

    while (j < size && isWS(s[j]))  // omijanie WS
        j++;

    return j - 1; 
}

/// <summary>
/// czyta tablicę
/// </summary>
/// <param name="s"> plik json w postaci łańcucha znaków </param>
/// <param name="i"> indeks, od którego czytanie się zaczyna </param>
/// <returns> indeks, na którym tablica się kończy </returns>
int readArray(string s, int i) 
{
    int size = s.length();
    int j = i + 1;  // s[i] == '['

    for (;; j++) {
        if (j == size) {
            string t = s.substr(i, j - i);
            cout << "BLAD: " << t << endl;
            return -1;
        }

        char c = s[j];

        if (isWS(c))  // dodane dla przypadku [ WS ]
            continue;

        if (c == ']')  // dodane dla przypadku [ WS ]
            return j;

        j = readValue(s, j);  // czytamy value , bo array to [ value, value ... ] 
        
        if (j == -1)  // jeżeli błędne value
            return -1;

        c = s[++j];  // następny znak

        if (c == ']')  // koniec tablicy
            return j;

        

        if (c != ',') {  // jeżeli to nie koniec, to następny znak musi być przecinkiem
            string t = s.substr(i, j - i + 1);
            cout << "BLAD: " << t << endl;
            return -1;
        }
    }

    return -1;
}

/// <summary>
/// czyta member
/// </summary>
/// <param name="s"> plik json w postaci łańcucha znaków </param>
/// <param name="i"> indeks, od którego czytanie się zaczyna </param>
/// <returns> indeks, na którym member się kończy </returns>
int readMember(string s, int i) {
    //  
    int size = s.length();
    int j = i;
    while (j < size && isWS(s[j]))  // omnięcie WS
        j++;

    // j - początek stringa
    if (s[j] != '"') {
        string t = s.substr(i, j - i + 1);
        cout << "BLAD: " << t << endl;
        return -1;
    }

    j = readString(s, ++j);  // odczyt stringa

    j++; 
    while (j < size && isWS(s[j]))  // omnięcie WS
        j++;
    
    if (s[j] != ':') {  // w tym miejscu musi być ':'
        string t = s.substr(i, j - i + 1);
        cout << "BLAD: " << t << endl;
        return -1;
    }
    
    j = readValue(s, ++j);  // następnie musi być value
    
    return j;
}

/// <summary>
/// czyta objekt 
/// logika identyczna do readArray!
/// </summary>
/// <param name="s"></param>
/// <param name="i"></param>
/// <returns> indeks, na którym objekt się kończy </returns>
int readObject(string s, int i)
{
    int size = s.length();
    int j = i + 1;  // s[i] == '{'

    for (;; j++) {
        if (j == size) {
            string t = s.substr(i, j - i);
            cout << "BLAD: " << t << endl;
            return -1;
        }

        char c = s[j];

        if (isWS(c))
            continue;

        if (c == '}')
            return j;

        j = readMember(s, j);
        
        if (j == -1)
            return -1;

        j++;

        c = s[j];

        if (c == '}')
            return j;

        if (c != ',') {
            string t = s.substr(i, j - i + 1);
            cout << "BLAD: " << t << endl;
            return -1;
        }
        
    }

    return -1;
}

/// <summary>
/// czyta wartości true, false, null
/// </summary>
/// <param name="s"> plik json jako string</param>
/// <param name="t"> 'true', 'false', 'null' </param>
/// <param name="i"> indeks z pozycją początku wartości </param>
/// <returns> indeks z końcem wartości </returns>
int readTrueFalseNull(string s, string t, int i)
{
    int size = s.length();
    int l = t.length();

    for (int j = 0; j < l; j++) {  // czytamy tyle znaków, ile znaków ma t
        if (i+j == size) {  // jeżeli jesteśmy poza plikiem json
            string w = s.substr(i, j - i);
            cout << "BLAD: " << w << endl;
            return -1;
        }

        char c = s[i + j];
        if (c != t[j]) {  // jeżeli znak z s nie jest identyczny z odpowiadającym mu znakiem z t
            string w = s.substr(i, j - i + 1);
            cout << "BLAD: " << w << endl;
            return -1;
        }
    }

    return i + l - 1;
}


