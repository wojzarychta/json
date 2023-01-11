#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;


void interactiveMode();
void displayHelp();
void openFile(char operation[]);
bool parseJSON();
void isCorrectJSON();
bool readValue();
bool readString();
bool readNumber();
bool readArray();
bool readLiteral(char c);
bool readMember();
bool readObject();
bool readValue();
void skipWS();
void displayErrorMessage();

fstream fs;

int main()
{
    interactiveMode();
    fs.close();

    return 0;
}


void interactiveMode()
{
    
    do {  
        system("cls");
        cout << "Wprowadz komende: ";
        char input[100];
        cin.getline(input, sizeof(input)); 
        switch (input[0]) {
        case 'h': 
            displayHelp();
            break;
        case 'o':
            openFile(input);
            break;
        case 'c': 
            isCorrectJSON();
            break;
        default: 
            cout << "Sprobuj ponownie" << endl;
            break;
        }
        system("pause");
    } while (true);
}


void displayHelp()
{
    system("cls");
    cout << " ";   // uzupełnić!!!
}

void openFile(char operation[])
{
    if (fs.is_open()) 
        fs.close();                  
    char dir[100];
    if (operation[1] == ' ') {    
        memcpy(dir, &operation[2], 98);
    }
    fs.open(dir, ios::in);        // open file
    
    if (fs)             
        cout << "Plik otwarto pomyslnie" << endl << endl;
    else
        cout << "Blad. Sprobuj ponownie" << endl << endl;
}

void skipWS() {  
    while (fs) {
        char c = (char)fs.peek();  // check what is the next character
        if (c == '\x20' || c == '\x09' || c == '\x0A' || c == '\x0D')
            fs.get();  // skip WS 
        else
            return;
    }
}

bool parseJSON() {
    if (fs)
        return readValue() && fs.get() == EOF;  // there must be nothing left to read 
    else
        return false;
}

void isCorrectJSON() {
    if (parseJSON())
        cout << "Plik JSON jest prawidlowy" << endl << endl;
    else
        cout << "Plik JSON NIE jest prawidlowy" << endl << endl;
}

bool readString() 
{
    char c;
    while (fs) {
        c = (char)fs.get();
        if (c == '"')   // end of string
            return true;
        if (c == '\\') {   
            c = (char)fs.get();
            if (fs && (c == '\\' || c == '"' || c == 't'))   // exceptions
                continue;
            else {  
                displayErrorMessage();
                return false;
            }
        }
    }
    displayErrorMessage();
    return false;
}

bool readNumber() {
    fs.unget();  // restore first character of Num
    char c = (char)fs.get();
    if (c == '-') {
        c = (char)fs.get();
        if (!fs) {
            displayErrorMessage();
            return false;
        }       
    }      
    if (c == '0') {  
        if (fs && (char)fs.peek() != '.') {   // only possibility after 0 is decimal place, otherwise NaN
            displayErrorMessage();
            return false;
        }
    }
    else if (!(c >= '1' && c <= '9')) {   // if not a digit
        displayErrorMessage();
        return false;
    }

    int decimalPlace = 0;
    while (fs) {
        c = (char)fs.get();
        if ((c < '0' || c > '9') && c != '.') {
            fs.unget();  // need to restore character because that char is not part of the number
            return true;
        }
        if (c == '.')
            decimalPlace++;
        if (decimalPlace > 1) {   // multiple decimal places in number
            displayErrorMessage();
            return false;
        }
    }
    displayErrorMessage();
    return false;
}

bool readValue()
{
    char c; 
    bool correct;  

    skipWS();
    c = (char)fs.get();
    cout << c << endl;
    switch (c) {
    case '[':
        correct = readArray();
        break;
    case '{':
        correct = readObject();
        break;
    case '"':
        correct = readString();
        break;
    case 't':
    case 'f':
    case 'n': 
        correct = readLiteral(c);
        break;
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '-':
        correct = readNumber();
        break;
    
    default: {  
        displayErrorMessage();
        correct = false;
    }     
    }

    skipWS();
    return correct; 
}

bool readArray() 
{
    char c;

    while (fs) {
        skipWS();
        if ((char)fs.peek() == ']')  
        {
            fs.get();
            return true;
        }
        if (!readValue())  
        {
            displayErrorMessage();
            return false;
        }
        c = (char)fs.get();
        if (c == ']')  
            return true;
        if (c != ',') { 
            displayErrorMessage();
            return false;
        }
    }
    displayErrorMessage();
    return false;
}

bool readMember() {
    skipWS();
    if ((char)fs.get() != '"') {
        displayErrorMessage();
        return false;
    }
    if (!readString())
        return false;
    skipWS();
    if ((char)fs.get() != ':') {  
        displayErrorMessage();
        return false;
    }
    
    return readValue(); 
}

bool readObject()
{
    char c;

    while (fs) {
        skipWS();
        if ((char)fs.peek() == '}')  
        {
            fs.get();
            return true;
        }
        if (!readMember())  
        {
            displayErrorMessage();
            return false;
        }
        c = (char)fs.get();
        if (c == '}')  
            return true;
        if (c != ',') {  
            displayErrorMessage();
            return false;
        }
    }
    displayErrorMessage();
    return false;
}

bool readLiteral(char c)
{
    string s;

    switch (c) {
    case 't':
        s = "true";
        break;
    case 'f':
        s = "false";
        break;
    case 'n':
        s = "null";
        break;
    default:
        displayErrorMessage();
        return false;
    }

    for (int j = 1; j < s.length(); j++) {  // j starts from 1, bc first char was already read
        if (!fs) {  
            displayErrorMessage();
            return false;
        }
        c = (char)fs.get();
        if (c != s[j]) {   // if char from file is diffrent from corresponding char from s
            displayErrorMessage();
            return false;
        }
    }

    return true;
}

void displayErrorMessage() {
    cout << "Znaleziono blad w skladni. Blad wystapil na " << fs.tellg() << " znaku w pliku" << endl;
}


