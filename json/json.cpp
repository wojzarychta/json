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
bool isString(string s);
bool isNum(string s);
bool isValue(string s);
bool isWS(char ch);
bool isMember(string s);
bool isObject(string s);
bool isArray(string s);

bool readJSON(string s);
int readValue(string s, int i = 0);
int readString(string s, int i = 0);
int readNum(string s, int i = 0);
int readArray(string s, int i = 0);
int readMember(string s, int i = 0);
int readObject(string s, int i = 0);
//------------------------------------------

int main()
{
    fstream file;

    menu(file);
    file.close();

    return 0;
}


void menu(fstream& fs)
{
    char choice;
    string command;

    do {
        system("cls");
        cout << ">> ";
        getline(cin >> ws, command); // używamy getline, aby whitespace nie przerywało odczytu 
        choice = command[0];
        int l = command.length();
        if (l > 2 && command[1] == ' ') 
            command = command.substr(2, command.length()-2);
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


bool open(fstream& fs, string& name)
{
    if (fs.is_open()) fs.close();                  //gdy był wcześniej otwarty plik, zamyka go
    if (name.size() < 6 || name.compare(name.size() - 5, 5, ".json") != 0) {               //jeżeli plik nie jest w formacie .json
        return false;
    }
    fs.open(name, ios::in);        //otwarcie pliku
    if (!fs) {                                     //kontrola otwarcia
        return false;
    }
    return true;
}

bool validate(fstream& fs) {
    // wczytaj cały plik do zmiennej json
    fs.seekg(0, ios::end);
    size_t size = fs.tellg();
    string json(size, ' ');
    fs.seekg(0);
    fs.read(&json[0], size);

    return readJSON(json);
}

/*
bool isString(string s) {
    int size = s.length();
    if (size < 2) {
        cout << "BLAD:  " << s << endl;
        return false;
    }
    if (s[0] != '"') {
        cout << "BLAD:  " << s << endl;
        return false;
    }
    for (int i = 1; i < size - 1; i++) {
        char ch = s[i];
        if (ch == '\\') {
            if (i < size - 2 && (s[i + 1] == '\\' || s[i + 1] == '"' || s[i + 1] == 't'))
                i++;
            else {
                cout << "BLAD:  " << s << endl;
                return false;
            }
        }
    }
    if (s[size-1] != '"') {
        cout << "BLAD:  " << s << endl;
        return false;
    }

    return true;
}

bool isNum(string s) {
    int size = s.length();
    if (size < 1)
        return false;
    int i = 0;
    if (s[i] == '-')
        i++;
    if (s[i] == '0') {
        if (size - 1 > i && s[i + 1] != '.') {
            cout << "BLAD:  " << s << endl;
            return false;
        }
    }
    else if (s[i] < '1' || s[i] > '9') {
        cout << "BLAD:  " << s << endl;
        return false;
    }
    i++;
    bool decimal = false;
    for (; i < size; i++) {
        if (s[i] == '.') {
            if (decimal)  // druga kropka, np. 10.23.4
            {
                cout << "BLAD:  " << s << endl;
                return false;
            }
            else
                decimal = true;
            continue;
        }
        if (s[i] < '0' || s[i] > '9') {
            cout << "BLAD:  " << s << endl;
            return false;
        }
    }

    return true;
}

bool isMember(string s) {
    int size = s.length();
    int i = 0;
    while (i < size && isWS(s[i]))
        i++;
    // i - początek stringa
    int j = i;
    while (j < size && isWS(s[i]))
        i++;


    while (j > 0 && isWS(s[j - 1]))
        j--;
    // nasz string jest pomiędzy indeksami i i j
    if (j - i <= 0)
        return false;
    string t = s.substr(i, j - i);

    while (i < size) {
        i f
    }

    return true;
}

bool isObject(string s) {
    int size = s.length();
    if (size < 2) {
        //cout << "BLAD:  " << s << endl;
        return false;
    }
    if (s[0] != '{') {
        //cout << "BLAD:  " << s << endl;
        return false;
    }

    int k = 1, ws = 0;
    for (int i = 1; i < size - 1; i++) {
        if (s[i] == ',') {
            string t = s.substr(k, i - k);
            cout << t;
            if (!isMember(t))
                return false;
            k = i + 1;
        }
        else if (isWS(s[i]))
            ws++;
    }
    string t = s.substr(k, size - 1 - k);
    if (!isMember(t) && ws != size - 2)
        return false;

    if (s[size - 1] != '}') {
        //cout << "BLAD:  " << s << endl;
        return false;
    }

    return true;
}

bool isArray(string s) {
    int size = s.length();
    if (size < 2) {
        //cout << "BLAD:  " << s << endl;
        return false;
    }
    if (s[0] != '[') {
        //cout << "BLAD:  " << s << endl;
        return false;
    }

    int k = 1, ws = 0;
    for (int i = 1; i < size - 1; i++) {
        if (s[i] == ',') {
            string t = s.substr(k, i - k);
            cout << t;
            if (!isValue(t))
                return false;
            k = i + 1;
        }
        else if (isWS(s[i]))
            ws++;
    }
    string t = s.substr(k, size - 1 - k);
    if (!isValue(t) && ws != size - 2)
        return false;

    if (s[size-1] != ']') {
        //cout << "BLAD:  " << s << endl;
        return false;
    }

    return true;
}

bool isValue(string s) {
    int size = s.length();
    int i = 0, j = size;
    while (i < size && isWS(s[i]))
        i++;
    while (j > 0 && isWS(s[j-1]))
        j--;
    // nasz string jest pomiędzy indeksami i i j
    if (j - i <= 0)
        return false;
    string t = s.substr(i, j-i);
    return isString(t) || isNum(t) || t == "true" || t == "false" || t == "null" || isArray(t) || isObject(t);
}
*/


bool isWS(char ch) {
    return ch == '\x20' || ch == '\x09' || ch == '\x0A' || ch == '\x0D';
}

bool readJSON(string s) {
    int size = s.length();
    int j = size - 1;
    int i = readValue(s);
    return i == j;
}

int readString(string s, int i) {
    
    // s[i] == '"'
    int size = s.length();
    int j = i+1;

    for (;; j++) {
        if (j == size) {
            string t = s.substr(i, j - i);
            cout << "BLAD: " << t << endl;
            return -1;
        }
        char c = s[j];
        if (c == '"')
            break;

        if (c == '\\') {
            if (j < size - 2 && (s[j + 1] == '\\' || s[j + 1] == '"' || s[j + 1] == 't'))
                j++;
            else {
                string t = s.substr(i, j - i+1);
                cout << "BLAD: " << t << endl;
                return -1;
            }
        }
    }

    return j;
}

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
    if (s[j] == '0') {
        if (size - 1 > j && s[j + 1] != '.') {
            string t = s.substr(i, j - i + 1);
            cout << "BLAD: " << t << endl;
            return -1;
        }
    }
    else if (s[j] < '1' || s[j] > '9') {
        cout << "BLAD:  " << s << endl;
        return false;
    }
    j++;
    bool decimal = false;
    for (;; j++) {
        if (j == size)
            return j - 1;

        char c = s[j];

        if ((c < '0' || c > '9') && c != '.') 
            return j -1 ;
        

        if (c == '.') {
            if (decimal)  // druga kropka, np. 10.23.4
            {
                string t = s.substr(i, j - i + 1);
                cout << "BLAD: " << t << endl;
                return -1;
            }
            else
                decimal = true;
            continue;
        }
    }

    return -1;
}

int readValue(string s, int i) 
{
    int size = s.length();
    int j = i;

    while (j < size && isWS(s[j]))
        j++;

    char c = s[j];
    switch (c) {
    case 't':
    case 'f':
    case 'n': {
        int x = 1;
    }
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
    default: {
        j = -1;
        //string t = s.substr(i, j - i + 1);
        //cout << "BLAD: " << t << endl;
    }
        
    }
    if (j == -1)
        return j;

    j++;

    while (j < size && isWS(s[j]))
        j++;

    return j - 1; // !!!! sus
}

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

        if (isWS(c))
            continue;

        if (c == ']')
            return j;

        j = readValue(s, j);
        
        if (j == -1)
            return -1;

        j++;

        c = s[j];

        if (c == ']')
            return j;

        

        if (c != ',') {
            string t = s.substr(i, j - i + 1);
            cout << "BLAD: " << t << endl;
            return -1;
        }
    }

    return -1;
}

int readMember(string s, int i) {
    int size = s.length();
    int j = i;
    while (j < size && isWS(s[j]))
        j++;

    // j - początek stringa
    if (s[j] != '"') {
        string t = s.substr(i, j - i + 1);
        cout << "BLAD: " << t << endl;
        return -1;
    }
    j = readString(s, ++j);

    j++; 
    while (j < size && isWS(s[j]))
        j++;
    
    if (s[j] != ':') {
        string t = s.substr(i, j - i + 1);
        cout << "BLAD: " << t << endl;
        return -1;
    }
    
    j = readValue(s, ++j);
    
    return j;
}

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

