#include <iostream>

using namespace std;

enum  Answer { LESS, GREATER, IN_RANGE };

Answer query(const double x, int& steps);
Answer queryTest(const double x, const double value, const double err, int& steps);

// menu
void menuIntervalPlaces(double&, double&, int&);
void menuStartSimulation(const double, const double, const int);
void menuTest(int, const double, const double, const int);

// misc
void printSeparator();
int inputInt();
void enterToContinue();

int main()
{
    srand(time(0));
    double low = 0, high = 0;
    int places = 0;
    while (true) {
        system("cls");
        cout << "Pogadjanje brojeva koriscenjem binarne pretrage u tabeli nepoznate velicine" << endl;
        printSeparator();
        cout << "1. Podesavanje intervala i tacnost na nivou decimalnog mesta" << endl;
        cout << "2. Pokretanje simulacije" << endl;
        cout << "3. Test" << endl;
        cout << endl;
        cout << "0. Kraj rada" << endl;
        printSeparator();
        cout << endl << "Unesite zeljenu opciju: ";
        int option = inputInt();
        if (option == 1) {
            menuIntervalPlaces(low, high, places);
        }
        else if (option == 2) {
            menuStartSimulation(low, high, places);
        }
        else if (option == 3) {
            menuTest(100, low, high, places);
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
    Answer a = query(h, steps);
    while (a == LESS) {
        if (abs(high - h) < err) break;
        l = h;
        increment *= 2;
        h = min(low + increment, high);
        a = query(h, steps);
    };
    if (a == IN_RANGE) {
        cout << "Broj je pronadjen: " << h << endl;
        cout << "Broj koraka: " << steps << endl;
    }
    else if (a == LESS) {
        cout << endl << "Nemoguce!" << endl;
    }
    else {
        double mid;
        bool pogodjen = false;
        while (abs(l - h) > err) {
            mid = (l + h) / 2;
            a = query(mid, steps);
            if (a == IN_RANGE) {
                cout << "Broj je pronadjen: " << round(mid * decade) / decade << endl;
                cout << "Broj koraka: " << steps << endl;
                pogodjen = true;
                break;
            }
            else if (a == LESS) l = mid;
            else h = mid;
        }
        if (!pogodjen) cout << endl << "Nemoguce!" << endl;
    }
    enterToContinue();
}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void menuTest(int n, const double low, const double high, const int places)
{
    double decade = pow(10, places);
    double err = pow(10, -1 * places);

    system("cls");
    while (n--) {
        double value = fRand(low, high);
        value = round(value * decade) / decade;
        //         cout << "ZADATA VREDNOST: " << value << endl;

        int steps = 0;

        double increment = err;
        double l = low, h = low + increment;

        Answer a = queryTest(h, value, err, steps);
        while (a == LESS) {
            if (abs(high - h) < err) break;
            l = h;
            increment *= 2;
            h = min(low + increment, high);
            a = queryTest(h, value, err, steps);
        };
        if (a == IN_RANGE) {
            cout << value << " okej u " << steps << " koraka -> " << h << endl;
        }
        else if (a == LESS) {
            cout << value << " -> nemoguce!" << endl;
        }
        else {
            double mid;
            bool pogodjen = false;
            while (abs(l - h) > err) {
                mid = (l + h) / 2;
                // mid = round(mid * decade) / decade;
                a = queryTest(mid, value, err, steps);
                if (a == IN_RANGE) {
                    cout << value << " okej u " << steps << " koraka -> " << round(mid * decade) / decade << endl;
                    pogodjen = true;
                    break;
                }
                else if (a == LESS) l = mid;
                else h = mid;
            }
            if (!pogodjen) cout << value << " -> nemoguce!" << endl;
        }
        //cout << endl;
        //enterToContinue();
    }
    enterToContinue();
}

Answer query(const double x, int& steps)
{
    steps++;
    cout << "Upit: " << x << endl;
    while (true) {
        cout << "Veci, manji ili u opsegu? [v, m, o] ";
        char ans;
        cin >> ans;
        if (ans == 'v') return GREATER;
        else if (ans == 'm') return LESS;
        else if (ans == 'o') return IN_RANGE;
    }
    cout << endl;
}

Answer queryTest(const double x, const double value, const double err, int& steps)
{
    // cout << "\t" << "upit " << x << " " << endl;
    steps++;
    if (abs(x - value) < err / 2) { return IN_RANGE; }
    else if (x > value) { return GREATER; }
    else if (x < value) { return LESS; }
}



void printSeparator() {
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

void enterToContinue()
{
    cout << endl << "Pritisnite enter za nastavak..." << endl;
    char x = getchar();
    x = getchar();
}