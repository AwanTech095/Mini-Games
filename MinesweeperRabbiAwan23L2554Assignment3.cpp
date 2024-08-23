#include <iostream>
#include <iomanip>//formatting-setw
#include <random>
#include <ctime>
#include <cmath>
#include<cstdlib>
#include <chrono>//included for timer
#include <thread>//included for the timer

//1) I have commented the clear screen function for now - you can remove the comments to use it(it will clear the previous screen after each turn)
//2) I tried to add a timer but a's file handling part was excluded-I have commented that too in the InputTake function 
using namespace std;

class Game {
public:
    virtual void StartGame() = 0;
    virtual void Show() = 0;
    virtual void Recursive(int row, int col) = 0;
    virtual void FillArray() = 0;
    virtual void PrintTable() = 0;
    virtual int BombCount() = 0;
    virtual int FlagCount() = 0;
    virtual void GeneratingMines() = 0;
    virtual int CountBomb(int row, int col) = 0;
    virtual void Assign() = 0;
    virtual bool isFlag(int row, int col) = 0;
    virtual bool AnyMinesLeft() = 0;
    virtual int TakeInput(int InitialBombCount) = 0;
    virtual int PlayMineCraft() = 0;
    virtual void Reveal(int row, int col) = 0;
    virtual bool RowCheck(int row) = 0;
    virtual bool ColCheck(int col) = 0;
    virtual bool BombCheck(int row,int col) = 0;
    virtual bool IsUsed(int row, int col) = 0;
    virtual bool isSpace(int row, int col) = 0;

};

class Minesweeper : public Game {
private:
    char** FrontEnd;
    char** BackEnd;
    int sIzE;
    int TBOMBS;
    int difficultyLevel; 

public:
    Minesweeper() {
        sIzE = 0;
        FrontEnd = nullptr;
        BackEnd = nullptr;
        TBOMBS = 0;
        difficultyLevel = 0; 
    }
    
    Minesweeper(int size, int bombs) {
        sIzE = size;
        TBOMBS = bombs;
        FrontEnd = new char* [sIzE];
        BackEnd = new char* [sIzE];
        for (int i = 0; i < sIzE; ++i) {
            FrontEnd[i] = new char[sIzE];
            BackEnd[i] = new char[sIzE];
        }
        difficultyLevel = 0; 
    }
    
    ~Minesweeper() {
        for (int i = 0; i < sIzE; ++i) {
            delete[] FrontEnd[i];
            delete[] BackEnd[i];
        }
        delete[] FrontEnd;
        delete[] BackEnd;
    }
    
    void StartGame() override {
        cout << "Welcome to Minesweeper!" << endl;
        cout << "Select difficulty level:" << endl;
        cout << "1. Easy (10x10 grid, 10 mines)" << endl;
        cout << "2. Intermediate (16x16 grid, 40 mines)" << endl;
        cout << "3. Advanced (20x20 grid, 99 mines)" << endl;
        cout << "Enter your choice: ";
        cin >> difficultyLevel;
        switch (difficultyLevel) {
        case 1:
            sIzE = 10;
            TBOMBS = 10;
            break;
        case 2:
            sIzE = 16;
            TBOMBS = 40;
            break;
        case 3:
            sIzE = 20;
            TBOMBS = 99;
            break;
        default:
            cout << "Invalid choice. Exiting..." << endl;
            exit(1);
        }
        
        FrontEnd = new char* [sIzE];
        BackEnd = new char* [sIzE];
        for (int i = 0; i < sIzE; i++) {
            FrontEnd[i] = new char[sIzE];
            BackEnd[i] = new char[sIzE];
        }
    }


    void Show() override {
        
        const int Size = sIzE;
        for (int i = 0; i < Size; i++)
        {
            for (int j = 0; j < Size; j++)
            {
                if (BackEnd[i][j] == 'B')
                {
                    Reveal(i, j);
                }
            }
        }
    }

    void Recursive(int row, int col) override {
        
        if (!RowCheck(row))
            return;
        if (!ColCheck(col))
            return;
        if (BackEnd[row][col] == 'U')//U means space pehle khuli hoi hai
            return;
        Show();
        if (isSpace(row, col))
        {
            FrontEnd[row][col] = ' ';//replacing front end with spaces
            BackEnd[row][col] = 'B';//replacing back end with spaces
            Recursive(row + 1, col);
            Recursive(row - 1, col);//jab koi space ayegi-recursive wala fnc call-agar woh row grid me hai ke nahi

            Recursive(row + 1, col - 1);
            Recursive(row + 1, col + 1);

            Recursive(row - 1, col + 1);
            Recursive(row - 1, col - 1);

            Recursive(row, col + 1);
            Recursive(row, col - 1);
        }//checking 8 spaces of the r c cell-opening the the cell where there is space
    }

    void Reveal(int row, int col)
    {
        if (BackEnd[row][col] == 'B')
        {
            if (RowCheck(row + 1))
            {

                if (!BombCheck(row + 1, col) && BackEnd[row + 1][col] != 'U' && BackEnd[row + 1][col] != 'B')
                    FrontEnd[row + 1][col] = BackEnd[row + 1][col];

                if (ColCheck(col + 1))
                {
                    if (!BombCheck(row + 1, col + 1) && BackEnd[row + 1][col + 1] != 'U' && BackEnd[row + 1][col + 1] != 'B')
                        FrontEnd[row + 1][col + 1] = BackEnd[row + 1][col + 1];
                }

                if (ColCheck(col - 1))
                {
                    if (!BombCheck(row + 1, col - 1) && BackEnd[row + 1][col - 1] != 'U' && BackEnd[row + 1][col - 1] != 'B')
                        FrontEnd[row + 1][col - 1] = BackEnd[row + 1][col - 1];
                }
            }

            if (RowCheck(row - 1))
            {

                if (!BombCheck(row - 1, col) && BackEnd[row - 1][col] != 'U' && BackEnd[row - 1][col] != 'B')
                    FrontEnd[row - 1][col] = BackEnd[row - 1][col];

                if (ColCheck(col + 1))
                {
                    if (!BombCheck(row - 1, col + 1) && BackEnd[row - 1][col + 1] != 'U' && BackEnd[row - 1][col + 1] != 'B')
                        FrontEnd[row - 1][col + 1] = BackEnd[row - 1][col + 1];
                }

                if (ColCheck(col - 1))
                {
                    if (!BombCheck(row - 1, col - 1) && BackEnd[row - 1][col - 1] != 'U' && BackEnd[row - 1][col - 1] != 'B')
                        FrontEnd[row - 1][col - 1] = BackEnd[row - 1][col - 1];
                }
            }

            if (ColCheck(col + 1))
                if (!BombCheck(row, col + 1) && BackEnd[row][col + 1] != 'U' && BackEnd[row][col + 1] != 'B')
                    FrontEnd[row][col + 1] = BackEnd[row][col + 1];

            if (ColCheck(col - 1))
                if (!BombCheck(row, col - 1) && BackEnd[row][col - 1] != 'U' && BackEnd[row][col - 1] != 'B')
                    FrontEnd[row][col - 1] = BackEnd[row][col - 1];
        }
    }

    void FillArray() override {
       
        const int Size = sIzE;
        // Will Basically Fill the array for first time use, can also use
        // loops to fill the array again and again, kind of like reset
        for (int row = 0; row < Size; row++)
        {
            for (int col = 0; col < Size; col++)
            {
                FrontEnd[row][col] = '.';
                BackEnd[row][col] = '.';
            }
        }

    }

    void PrintTable() override {
        
         // Will print the table, imp since want to show the table at diff times
        const int Size = sIzE;
        cout << "Flag Count: " << FlagCount() << endl; // will show flag
        cout << "   ";

        for (int col = 0; col < Size; col++)
            cout << setw(2) << col << " ";
        cout << endl;
        // setw and other spaces for formatting
        for (int row = 0; row < Size; row++)
        {
            cout << setw(2) << row << " ";
            for (int col = 0; col < Size; col++)
            {
                cout << setw(2) << FrontEnd[row][col] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    int BombCount() override {
        
        const int Size = sIzE;
        int count = 0;
        for (int row = 0; row < Size; row++)
        {
            for (int col = 0; col < Size; col++)
            {
                if (BackEnd[row][col] == '*')
                    count++;
            }
        }
        return count;
    }

    int FlagCount() override {
       
        const int Size = sIzE;
        int count = 0;
        for (int row = 0; row < Size; row++)
        {
            for (int col = 0; col < Size; col++)
            {
                if (BackEnd[row][col] == 'F')
                    count++;
            }
        }
        return count;
    }

    bool IsUsed(int row, int col) override {
       
        if (BackEnd[row][col] == 'U' || BackEnd[row][col] == 'B')//U is a cell which does not has space in its neighbouring
            return 1;
        return 0;
    }

    void GeneratingMines() override {
        const int Size = sIzE;
        const int tbomb = TBOMBS;
        int filled = 0;

        mt19937 rng(std::time(0));
        uniform_int_distribution<int> dist(0, Size - 1);

        // Generate and assign bombs to the board
        while (filled < tbomb) {
            int row = dist(rng);
            int col = dist(rng);
            if (BackEnd[row][col] != '*') { // Ensure there's no bomb already
                BackEnd[row][col] = '*';
                filled++;
            }
        }
    }
    bool RowCheck(int row) // To check row doesnt exceed size or are less than size
    {
        const int Size = sIzE;
        if (row >= Size || row < 0)
            return 0;

        else
            return 1;
    }

    bool ColCheck(int col) // To check col doesnt exceed size or are less than size
    {
        const int Size = sIzE;
        if (col >= Size || col < 0)
            return 0;

        else
            return 1;
    }

    bool BombCheck(int row, int col) // To check if bomb is there
    {

        if (RowCheck(row) && ColCheck(col))
        { // To check row and col dont exceed size or are less than size

            if (BackEnd[row][col] == '*')
                return 1;
        }

        return 0;
    }

    bool isSpace(int row, int col) override // To check if it is space or not
    {
        if (BackEnd[row][col] == '0')
            return 1;
        return 0;
    }
    int CountBomb(int row, int col) override {
       
        int count = 0;

        if (RowCheck(row + 1))
        {

            if (BackEnd[row + 1][col] == '*')
                count++;

            if (ColCheck(col + 1))
            {
                if (BackEnd[row + 1][col + 1] == '*')
                    count++;
            }

            if (ColCheck(col - 1))
            {
                if (BackEnd[row + 1][col - 1] == '*')
                    count++;
            }
        }

        if (RowCheck(row - 1))
        {

            if (BackEnd[row - 1][col] == '*')
                count++;

            if (ColCheck(col + 1))
            {
                if (BackEnd[row - 1][col + 1] == '*')
                    count++;
            }

            if (ColCheck(col - 1))
            {
                if (BackEnd[row - 1][col - 1] == '*')
                    count++;
            }
        }

        if (ColCheck(col + 1))
            if (BackEnd[row][col + 1] == '*')
                count++;

        if (ColCheck(col - 1))
            if (BackEnd[row][col - 1] == '*')
                count++;

        return count;

    }

    void Assign() override {
        const int Size = sIzE;

        // Iterate through the board and assign numbers indicating nearby bombs
        for (int row = 0; row < Size; row++) {
            for (int col = 0; col < Size; col++) {
                if (BackEnd[row][col] != '*') { // Skip cells with bombs
                    int count = 0;

                    // Check adjacent cells for bombs
                    for (int i = -1; i <= 1; i++) {
                        for (int j = -1; j <= 1; j++) {
                            if (i == 0 && j == 0) continue; // Skip current cell
                            int r = row + i;
                            int c = col + j;
                            if (r >= 0 && r < Size && c >= 0 && c < Size && BackEnd[r][c] == '*') {
                                count++;
                            }
                        }
                    }

                    // Assign number of adjacent bombs to the current cell
                    BackEnd[row][col] = '0' + count;
                }
            }
        }
    }

    bool isFlag(int row, int col) override {
       
        if (BackEnd[row][col] == 'F')
            return 1;
        return 0;
    }

    bool AnyMinesLeft() override {
        
        const int Size = sIzE;
        int count = 0;
        for (int row = 0; row < Size; row++)
        {
            for (int col = 0; col < Size; col++)
            {
                if (BackEnd[row][col] == '*')
                    count++;
            }
        }
        if (count > 0)
            return 1;
        return 0;
    }

    int TakeInput(int InitialBombCount) override {
      
        const int Size = sIzE;
        int row, col;
        int k = 1;
        bool GameOver = 0, isZero = 1;
        char Decision;
        bool isFirstMove = 1;

        while (!GameOver && AnyMinesLeft())
        {
#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif
//added to clear out screen
            
            auto start = chrono::steady_clock::now();
            if (isFirstMove) // only for flag rn
            {
                isFirstMove = 0;
                cout << "Do you want to flag or not ? Click L to expose the cell and Click R to flag (R/L): ";
                cin >> Decision;
                if (Decision == 'R' || Decision == 'r' || Decision == 'L' || Decision == 'l')
                {
                    cout << "Enter Coordinates (row col): ";
                    cin >> row >> col;
                    auto end = chrono::steady_clock::now();
                    auto elapsed_seconds = chrono::duration_cast<chrono::seconds>(end - start);
                    cout << "Time elapsed: " << elapsed_seconds.count() << " seconds" << endl;
                    
                    this_thread::sleep_for(chrono::milliseconds(1000));
                    if (Decision == 'R' || Decision == 'r')
                    {
                        if (RowCheck(row) && ColCheck(col))
                        {
                            FrontEnd[row][col] = 'F'; // to show
                            BackEnd[row][col] = 'F';  // So no bomb there either, to actually remove the bomb
                            
                            //system(CLEAR_SCREEN);
                            PrintTable();
                        }
                        else
                        {
                            cout << "Invalid Input" << endl; // if row, col is out of array, or its a flag or space than invalid input
                            PrintTable();
                        }
                    }
                    else if (BombCheck(row, col) && RowCheck(row) && ColCheck(col) && !isSpace(row, col))
                    {
                        int i = 0, j = 0; // if bomb-
                        while (BombCheck(i, j) || isSpace(i, j))//to find U(cell containing 8 bombs)//no bomb or no space
                        {
                            j++;
                            if (j == Size - 1)
                            {
                                j = 0;
                                i++;
                            }
                        }//stops when its find a box where there is a number-no bomb no space
                        char temp = BackEnd[row][col];
                        BackEnd[row][col] = BackEnd[i][j];
                        BackEnd[i][j] = temp;
                        FrontEnd[row][col] = BackEnd[row][col];
                        BackEnd[row][col] = 'U';
                        //system(CLEAR_SCREEN);
                        PrintTable();
                    }
                    else if (!BombCheck(row, col) && RowCheck(row) && ColCheck(col) && isSpace(row, col))
                    {
                        //system(CLEAR_SCREEN);
                        Recursive(row, col);
                        PrintTable();
                    }
                    else if (!BombCheck(row, col) && RowCheck(row) && ColCheck(col) && !isSpace(row, col))
                    {
                        FrontEnd[row][col] = BackEnd[row][col];
                        BackEnd[row][col] = 'U';
                        //system(CLEAR_SCREEN);
                        PrintTable();
                    }
                    else
                    {
                        //system(CLEAR_SCREEN);
                        cout << "Invalid Input" << endl; // if row, col is out of array, or its a flag or space than invalid input
                        PrintTable();
                    }

                    // }// end bracker of space else
                }
                else
                {
                    //system(CLEAR_SCREEN);
                    cout << "Invalid Input" << endl; // if row, col is out of array, or its a flag or space than invalid input
                    PrintTable();
                } // end bracket of decision if
            }     // end bracket of is not first move

            else
            {
                if (FlagCount() < InitialBombCount)
                {
                    cout << "Do you want to flag or not ? Click L to expose the cell and Click R to flag (R/L): ";
                    cin >> Decision;
                    if (Decision == 'R' || Decision == 'r' || Decision == 'L' || Decision == 'l')
                    {
                        if (Decision == 'R' || Decision == 'r')
                        {
                            cout << "Enter Coordinates (row col): ";
                            cin >> row >> col;
                            auto end = chrono::steady_clock::now();
                            auto elapsed_seconds = chrono::duration_cast<chrono::seconds>(end - start);
                            cout << "Time elapsed: " << elapsed_seconds.count() << " seconds" << endl;

                            this_thread::sleep_for(chrono::milliseconds(1000));
                            if (RowCheck(row) && ColCheck(col) && isSpace(row, col))
                            {
                                FrontEnd[row][col] = 'F'; // to show
                                BackEnd[row][col] = 'F';  // So no bomb there either, to actually remove the bomb
                                //system(CLEAR_SCREEN);
                                PrintTable();
                            }
                            else
                            {
                                if (RowCheck(row) && ColCheck(col) && !isFlag(row, col) && !isSpace(row, col) && !IsUsed(row, col))
                                {
                                    FrontEnd[row][col] = 'F'; // to show
                                    BackEnd[row][col] = 'F';  // So no bomb there either, to actually remove the bomb
                                   // system(CLEAR_SCREEN);
                                    PrintTable();
                                }
                                else
                                {
                                    //system(CLEAR_SCREEN);
                                    cout << "Invalid Input" << endl; // if row, col is out of array, or its a flag or space than invalid input
                                    PrintTable();
                                    // ofce the first space will just open all the other spaced but that part of the code will only work
                                    //  for the first space
                                }
                            }
                        }
                        else
                        { // if its not a flag
                            cout << "Enter Coordinates (row col): ";
                            cin >> row >> col;
                            auto end = chrono::steady_clock::now();
                            auto elapsed_seconds = chrono::duration_cast<chrono::seconds>(end - start);
                            cout << "Time elapsed: " << elapsed_seconds.count() << " seconds" << endl;

                            
                            this_thread::sleep_for(chrono::milliseconds(1000));
                            if (RowCheck(row) && ColCheck(col) && isSpace(row, col))
                            { // will show space
                                //system(CLEAR_SCREEN);
                                Recursive(row, col);
                                // PrintTable();
                                PrintTable();
                            }
                            else
                            {
                                if (RowCheck(row) && ColCheck(col) && !isFlag(row, col) && !isSpace(row, col) && !IsUsed(row, col))
                                {
                                    if (!BombCheck(row, col))
                                    {
                                        FrontEnd[row][col] = BackEnd[row][col];
                                        BackEnd[row][col] = 'U';
                                       // system(CLEAR_SCREEN);
                                        PrintTable();
                                    }
                                    else
                                    { // if its a bomb, didnt need this for flag
                                        //system(CLEAR_SCREEN);
                                        PrintTable();
                                        cout << "You hit a bomb!!!....Game Over" << endl;
                                        GameOver = 1;

                                        k = 0;
                                    }
                                }
                                else
                                {
                                    system(CLEAR_SCREEN);
                                    cout << "Invalid Input" << endl; // part of the choosing no flag if, wrong coordnates
                                    PrintTable();
                                }
                            }
                        }
                    }
                    else
                    {
                        //system(CLEAR_SCREEN);
                        cout << "Invalid Input" << endl; // part of the choosing yes/no if, wrong decision
                        PrintTable();
                    }
                }
                else
                {
                    if (AnyMinesLeft())
                    { // mines are left, you loose, else you win
                        //system(CLEAR_SCREEN);
                        PrintTable();
                        cout << "You hit a bomb!!!....Game Over" << endl;
                        k = 0;
                    }
                    else {
                        cout << "Congratulations!!! You Win" << endl;
                    }
                    GameOver = 1;
                }
            }
        }
        int score = abs(InitialBombCount - BombCount());
        return k;

    }

    int PlayMineCraft() override {
       

        FillArray();
        GeneratingMines();
        Assign();
        PrintTable();
        int IntialBombCount = BombCount();
        int k = TakeInput(IntialBombCount);
        return k;
    }
};

int main() {
    Minesweeper game(10,10);
    game.StartGame();
    int result = game.PlayMineCraft();
    while (result == 2) {
        result = game.PlayMineCraft();
    }
    return 0;
}



