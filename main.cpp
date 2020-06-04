#include <iostream>
#include <cstdlib>
#include <fstream>
#include <time.h>
#include <sstream>
#include <algorithm>
#include <windows.h>

using namespace std;

int menu();
void ranking();
int cube(string a);
void NewGame();
string instruction(int power);
void sortRank();
void baner(int pl, int turn, int power);
void clearBoard(string board[], int position[4][2]);
void printBoard(string board[], int position[4][2]);
void changePosition(string throwC, string position[4][2], int pl);
void addToRanking(string winName, int turn);
bool question();

int main()
{
    srand(time(NULL));
    bool endProgram = false;

    while (!endProgram)
    {
        switch ( menu() )
        {
        case 1:
            NewGame();
            system("cls");
            break;
        case 2:
            sortRank();
            ranking();
            break;
        case 3:
            endProgram = true;
            system("cls");
            break;
        default:
            system("cls");
            cout << "Wybierz numer od 1 do 3!" << endl;
            break;
        }
    }

    return 0;
}

bool question()
{
    fstream file;
    string answer, goodAnswer;
    string line;

    file.open("pytania.txt", ios::in);

    //losuj liczbe od 0 do 19 (aby przewinac inne pytania z pliku)
    //srand(time(NULL));
    int r = rand()%20;

    //usuwanie spamu (1 pytanie zajmuje 6 linijek)
    for (int i = 0; i < r*6; i++)
    {
        getline(file, line);
    }

    //wczytanie wlasciwego pytanka
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    for (int i = 0; i < 5; i++)
    {
        getline(file, line);
        cout << line << endl;
    }
    file >> goodAnswer;
    file.close();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout << endl << "Podaj odpowiedz: ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    cin >> answer;

    transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
    if (answer == goodAnswer) return true;
    else return false;
}

//Dodaje nowe imie do rankingu
void addToRanking(string winName, int turn)
{
    int points = 7100 - turn*100;

    fstream file;
    file.open("ranking.txt", ios::out | ios::app);

    file << winName << " - " << points << endl;

    file.clear();
    file.close();
    sortRank();
}

//Zmienia pozycję gracza pl w tablicy pozycji graczy o wartość a
void changePosition(string throwC, string position[4][2], int pl)
{
    int a = cube(throwC);


        if (throwC == "r" || throwC == "R")
        {

        string numString = position[pl-1][0];
        int num = atoi(numString.c_str());
        num += a;
        if (num < 0) num = 0;

        ostringstream ss;
        ss << num;
        numString = ss.str();

        if(num < 10)
            numString = "0" + numString;

        position[pl-1][0] = numString;

        }

        else

        for (int i = 0; i < 4; i++)
        {
            if (i != pl-1)
            {
                string numString = position[i][0];
                int num = atoi(numString.c_str());
                num += a;
                if (num < 0) num = 0;

                ostringstream ss;
                ss << num;
                numString = ss.str();

                if(num < 10)
                numString = "0" + numString;

                position[i][0] = numString;
            }
        }


}

//Wyswietla maly baner na gorze, zawierajacy najwazniejsze informacje o graczu
void baner(int pl, int turn, int power)
{
    system("cls");
    cout << "GRACZ " << pl << ", TURA " << turn << ", RZUT MOCY " << power << endl << endl;
}

//Pobiera czysta plansze z pliku txt, a nastepnie nanosi na nia pozycje graczy
void clearBoard(string board[], string position[4][2])
{
    fstream file;
    string pawn;
    string Pposition;
    string line;

    file.open("plansza.txt", ios::in);
    for (int i = 0; i < 20; i++)
        getline(file, board[i]);
    file.close();

    for (int m = 0; m < 4; m++)
    {
        pawn = position[m][1];
        Pposition = position[m][0];

        for (int i = 0; i < 20; i++)
        {
            line = board[i];
            for (int j = 0; j < 79; j++)
            {
                if (line[j] == Pposition[0] && line[j+1] == Pposition[1])
                {
                    line[j+1] = line[j] = pawn[0];
                    board[i] = line;
                }
            }
        }
    }
}

//Drukuje aktualna plansze na ekran (najpierw wywoluje clearBoard)
void printBoard(string board[], string position[4][2])
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),13);
    clearBoard(board, position);
    for (int i = 0; i < 20; i++)
        cout << board[i] << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
}

//Daje informacje graczowi i pozwala wykonac jeden z dwoch rzutow kostka
string instruction(int power)
{
    string choice;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    cout << endl << "Witaj szczesliwy graczu!" << endl;
    cout << "TT - Telimena (gracz 1)" << endl;
    cout << "RR - Ksiadz Robak (gracz 2)" << endl;
    cout << "ZZ - Zosia (gracz 3)" << endl;
    cout << "JJ - Jankiel (gracz 4)" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout << endl << "Mozesz wykonac dwa rodzaje rzutu kostka: " << endl;
    cout << "Nacisnij R, aby wykonac klasyczny rzut, poruszajacy Twoim pionkiem" << endl;
    cout << "Nacisnij L, aby wykonac rzut cofajacy innych graczy o wyrzucona liczbe oczek" << endl;
    cout << "Uwaga! Rzut cofajacy innych graczy mozesz wykonywac co 3 tury!" << endl;

    do
    {
    cout << endl << "wybieram: ";
    cin >> choice;
    if ((choice != "L" && choice != "l" && choice != "R" && choice != "r") || ((choice == "l" || choice == "L") && power < 3))
        cout << "Nie mozesz wykonac tej operacji!" << endl;
    }while ((choice != "L" && choice != "l" && choice != "R" && choice != "r") || ((choice == "l" || choice == "L") && power < 3));
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);

    return choice;
}

int cube(string a)
{
    //srand(time(NULL));
    int number;

    if (a == "R" || a == "r")
        number = rand()%6+1;
    else
        number = rand()%6-6;

    cout << "Wyrzuciles " << number << "!" << endl;
    return number;
}

void NewGame()
{
    //deklaracja zmiennych
    bool endGame = false;
    string win;
    string choice;
    int turn = 1;
    string throwC;
    int PowerThrow[4] = {0,0,0,0};
    string board[20];
    string position[4][2] = { {"00","T"},{"00","R"},{"00","Z"},{"00","J"} };


    //czyszczenie ekranu po poprzednim menu
    system("cls");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);

    //Petla nakazujaca uzytkownikowi wybrac wlasciwa liczbe graczy
    do
    {
    cout << "Podaj liczbe graczy (1-4): ";
    cin >> choice;

    if (choice != "1" && choice != "2" && choice != "3" && choice != "4")
    {
        system("cls");
        cout << "Podaj prawidlowa wartość!" << endl;
    }

    } while (choice != "1" && choice != "2" && choice != "3" && choice != "4");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);

    //Po wyborze wlasciwej liczby graczy, zaczynamy grać
    int p = atoi(choice.c_str());
    //Pierwsza petla bedzie sie powtarzac, dopoki ktos nie wygra
    while (!endGame)
    {
        //Druga petla zmienia nam graczy wykonujacych swoj ruch
        for (int player = 1; player <= p; player++)
        {
            //---------------------------------------------------
            //NAJWAZNIEJSZA CZESC PROGRAMU!!!

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
            baner(player, turn, PowerThrow[player-1]);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            //Zadaje pytanie zezwalajace na wykonanie ruchu
            if (question())
            {

            system("cls");
            //Wyswietla wiersz z informacjami
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
            baner(player, turn, PowerThrow[player-1]);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            //Drukuje plansze zaktualizowana po ruchu ostatniego gracza
            printBoard(board, position);
            //Pozwala wybrac wlasciwy rzut R/L
            throwC = instruction(PowerThrow[player-1]);

            //Jesli zuzyles rzut mocy - reset licznika
            if (throwC == "L" || throwC == "l") PowerThrow[player-1] = 0;


            //Zmiana pozycji gracza (Ciebie lub innych)
            changePosition(throwC, position, player);

            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
                cout << endl << "Zla odpowiedz! Tracisz swoja kolejke." << endl;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            }


            //Koniec tury
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
            cout << "Nacisnij enter, aby zakonczyc ture...";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            getchar(); getchar();




            //Sprawdza, czy gracz przekroczyl juz mete
            win = position[player-1][0];
            if ( atoi(win.c_str()) >= 71 )
            {
                string winName;

                system("cls");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),13);
                cout << "Wygrywa GRACZ " << player << "!" << endl << endl;
                cout << "Zwyciezco! " << "Wpisz swoje imie: ";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
                cin >> winName;
                addToRanking(winName, turn);
                endGame = true;
                break;
            }




            //Regeneracja rzutu mocy (stan naladowania wyswietlany na banerze)
            if (PowerThrow[player-1] < 3) PowerThrow[player-1]++;


            //---------------------------------------------------
        }
        turn++;
    }
}

void sortRank()
{
    fstream ranking;
    string name[11];
    string bufferS;
    int bufferI;
    int score[11];
    int n = 0;

    //wczytanie danych do tablicy
    ranking.open("ranking.txt", ios::in);
    while (ranking >> name[n])
    {
        ranking >> bufferS;
        ranking >> score[n];
        n++;
    }
    n--;
    ranking.clear();
    ranking.close();

    //sortowanie babelkowe
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (score[j] < score[j+1])
            {
                bufferS = name[j];
                bufferI = score[j];
                name[j] = name[j+1];
                score[j] = score[j+1];
                name[j+1] = bufferS;
                score[j+1] = bufferI;
            }
        }
    }

    //Zapisanie 10 najlepszych wynikow
    ranking.open("ranking.txt", ios::out);
    for (int i = 0; i <= n && i <= 10; i++)
        ranking << name[i] << " - " << score[i] << endl;
    ranking.close();
}

void ranking()
{
    system("cls");

    string text;
    fstream ranking;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
    ranking.open("ranking.txt", ios::in);
    for(int i = 1; getline(ranking, text); i++)
        cout << i << ". " << text << " punktow" << endl;
    ranking.close();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout << endl << "Nacisnij enter, aby powrocic do menu";
    getchar(); getchar();
    system("cls");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
}

int menu()
{
    string a;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    cout << endl << "------------------------------" << endl;
    cout << "Witaj w swiecie Pana Tadeusza!" << endl;
    cout << "1. NOWA GRA" << endl;
    cout << "2. RANKING" << endl;
    cout << "3. WYJSCIE" << endl;
    cout << "------------------------------" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout << endl << "Wybierz i zatwierdz enterem: ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);

    cin >> a;
    return atoi(a.c_str());
}
