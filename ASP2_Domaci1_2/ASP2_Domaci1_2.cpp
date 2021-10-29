#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

enum  Answer { LESS, GREATER, IN_RANGE };

Answer query(const double x, int& steps);

// menu
void menuIntervalPlaces(double&, double&, int&);
void menuStartSimulation(const double, const double, const int);

// misc
inline void printSeparator();
inline int inputInt();
inline void enterToContinue();

int main()
{
    double low = 0, high = 0;
    int places = -1;
    while(true) {
        system("cls");
        cout << "Pogadjanje brojeva koriscenjem binarne pretrage u tabeli nepoznate velicine" << endl;
        printSeparator();
        cout << "1. Podesavanje intervala i tacnost na nivou decimalnog mesta" << endl;
        cout << "2. Pokretanje simulacije" << endl;
        cout << endl;        
        cout << "0. Kraj rada" << endl;
        printSeparator();
        cout << endl << "Unesite zeljenu opciju: ";
        int option = inputInt();
        if (option == 1) {
            menuIntervalPlaces(low, high, places);
        }
        else if (option == 2) {
            if (places == -1 || low >= high) {
                cout << endl << "Uneti podaci nisu korektni!";
                enterToContinue();
            }
            menuStartSimulation(low, high, places);
        }
        else if (option == 0) break;
    }
}

void menuIntervalPlaces(double& low, double& high, int& places)
{
    system("cls");
    cout << "Unesite levu granicu opsega pretrage:  ";
    cin >> low;
    cout << "Unesite desnu granicu opsega pretrage: ";
    cin >> high;
    cout << "Unesite tacnost (broj decimalnih mesta): ";
    cin >> places;
    cout << endl;
    enterToContinue();
}

void menuStartSimulation(const double low, const double high, const int places)
{
    double err = pow(10, -1 * places);
    double decade = pow(10, places);
    double increment = err;

    double l = low, h = low + increment;
    int steps = 0;
    
    // finding the interval
    Answer a = query(h, steps);
    while (a == LESS) { 
        if (abs(high - h) < err) break;
        l = h; 
        increment *= 2; 
        h = min(low + increment, high); 
        a = query(h, steps); 
    };

    if (a == IN_RANGE) {
        cout << endl << "Broj je pronadjen: " << h << endl;
        cout << "Broj koraka: " << steps << endl;
    }
    else if(a == LESS) {
        cout << endl << "Nemoguce!" << endl;
    }
    else {
        // binary search
        double mid;
        bool pogodjen = false;
        while (abs(l - h) > err) {
            mid = (l + h) / 2;            
            a = query(mid, steps);
            if (a == IN_RANGE) {
                cout << endl << "Broj je pronadjen: " << round(mid * decade) / decade << endl;
                cout << "Broj koraka: " << steps << endl;
                pogodjen = true;
                break;
            }
            else if (a == LESS) l = mid;
            else h = mid;
        }
        if(!pogodjen) cout << endl << "Nemoguce!" << endl;        
    }    
    enterToContinue();
}


Answer query(const double x, int& steps)
{
    steps++;
    cout << "Upit: ";
    cout << std::fixed << x << endl;
    while (true) {
        cout << "Veci, manji ili u opsegu? [v, m, o] ";
        char ans;
        cin >> ans;
        cout << endl;
        if (ans == 'v') return GREATER;
        else if (ans == 'm') return LESS;
        else if (ans == 'o') return IN_RANGE;
    }   
}

inline void printSeparator() {
    cout << "====================================================================" << endl;
}

inline int inputInt() {
    string x;
    cin >> x;
    string::const_iterator it = x.begin();
    while (it != x.end() && isdigit(*it)) ++it;
    bool isNumber = !x.empty() && it == x.end();
    if (isNumber) return atoi(x.c_str());
    return -1;
}

inline void enterToContinue()
{
    cout << endl << "Pritisnite enter za nastavak..." << endl;
    char x = getchar();
    x = getchar();
}