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
            cout << isArray("[  \t]");
            //cout << isValue(" ");
            //cout << validate(fs) << endl;
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

    return isValue(json);
}

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

bool isWS(char ch) {
    return ch == '\x20' || ch == '\x09' || ch == '\x0A' || ch == '\x0D';
}