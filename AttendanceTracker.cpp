// *********************************************************
// Program: AttendanceTracker.cpp
// Course: CCP6114 Programming Fundamentals
// Lecture Class: TC5L
// Tutorial Class: TT15L
// Trimester: 2530
// Member_1: 252UC242D2 | ADAM SYAFIQ BIN SALLEHUDDIN | ADAM.SYAFIQ.SALLEHUDDIN@STUDENT.MMU.EDU.MY | 012-361 3065
// Member_2: 252UC242QY | MUHAMMAD HAFIY AL-HAFIIZH BIN JOAHARI | MUHAMMAD.HAFIY.ALHAFIIZH1@STUDENT.MMU.EDU.MY | 016-212 3859
// Member_3: 252UC242NE | AIDAN KHALIQ BIN SHAHNAZARUDDIN | AIDAN.KHALIQ.SHAHNZARUDDI1@student.mmu.edu.my | 017-615 5250
// *********************************************************
// Task Distribution
// Member_1:
// Member_2: Implemented the main program structure, insertRow.
// Member_3:
// *********************************************************


#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;


//constants 
const int MAX_COLUMNS = 10;
const int MAX_ROWS = 100;

// variables
string sheetName;
string columnNames[MAX_COLUMNS];
string textCells[MAX_ROWS][MAX_COLUMNS];
int columnTypes[MAX_COLUMNS]; 
int intCells[MAX_ROWS][MAX_COLUMNS];
int numColumns = 0;
int numRows = 0;

// function
void createSheet(string name);
void getColumnInfo(int colIndex);
void insertRow();
void viewCSV();


bool isNumber(const string& str);
int convertToInt(const string& str);

//hafiy
int main()
{
    string input;
    int numCols;
    char continueInsert;
    int i;

    cout << "=================================\n";
    cout << "   STUDENT ATTENDANCE TRACKER    \n";
    cout << "=================================\n\n";

    cout << "Enter Attendance sheet name: ";
    getline(cin, input);
    createSheet(input);

    cout << "Attendance sheet \"" << input << "\" created successfully." << endl;
    cout << endl;

    cout << "Enter number of columns (max 10): ";
    while (true)
    {
        if (cin >> numCols && numCols >= 1 && numCols <= MAX_COLUMNS)
        {
            break;
        }
        else
        {
            cout << "Error: Invalid number of columns.\n";
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Enter number of columns (max 10): ";
        }
    }

    cin.ignore();
    numColumns = numCols;

    for (i = 0; i < numCols; i++)
    {
        getColumnInfo(i);
    }

    cout << "Sheet structure created successfully." << endl;


    continueInsert = 'y';
    while (continueInsert == 'y' || continueInsert == 'Y')
    {
        cout << "------------------------" << endl;
        cout << "Insert New Attendance Row" << endl;
        cout << "------------------------" << endl;

        insertRow();

        cout << endl;
        cout << "Do you want to insert another row? (y/n): ";
        cin >> continueInsert;
        cin.ignore();
        cout << endl;
    }

    cout << "---------------------" << endl;
    cout << "View attendance sheet" << endl;
    cout << "---------------------" << endl;



    viewCSV();
    cout << endl;

    cout << "-------------------------" << endl;
    cout << "Basic Error Handling Demo" << endl;
    cout << "-------------------------" << endl;
    cout << "Enter StudentID: "; // this  to  test  the  validity  of  the  integer  input
    getline(cin, input);

    if (isNumber(input))
    {
        int testValue = convertToInt(input);
        cout << "Valid integer: " << testValue << endl;
    }
    else
    {
        cout << "Error: Invalid INT value." << endl;
    }

    cout << endl;
    cout << "-------------------" << endl;
    cout << "End of Milestone 1 " << endl;
    cout << "-------------------" << endl;

    return 0;
}


//hafiy
void insertRow()
{
    if (numRows >= MAX_ROWS)
    {
        cout << "Error: Maximum number of rows reached." << endl;
        return;
    }

    string input;

    for (int i = 0; i < numColumns; i++)
    {

        bool isStatus = (columnNames[i].length() == 6) &&
                        (columnNames[i][0] == 'S' || columnNames[i][0] == 's') &&
                        (columnNames[i] == "Status" || columnNames[i] == "status");

        if (isStatus)
            cout << "Enter " << columnNames[i] << " (Present: 1, Absent: 0): ";
        else
            cout << "Enter " << columnNames[i] << ": ";

        getline(cin, input, '\n');

        string lowerColName = "";
        for (char& c : columnNames[i]) {
            lowerColName += tolower(c);
        }

        bool enforceInt = (columnTypes[i] == 0) || (lowerColName == "studentid") || (lowerColName == "student id");

        if (enforceInt)
        {
            if (isNumber(input))
            {
                if (columnTypes[i] == 0) 
                {
                    intCells[numRows][i] = convertToInt(input);
                } 
                else 
                {
                    textCells[numRows][i] = input;
                }
            }
            else
            {
                cout << "Error: Invalid INT value. Please enter a number." << endl;
                i--; 
            }
        }
        else
        {
            textCells[numRows][i] = input; 
        }

    }

    numRows++;
    cout << "Row inserted successfully." << endl;
}

void viewCSV()
{
    for (int i = 0; i < numColumns; i++)
    {
        cout << columnNames[i];
        if (i < numColumns - 1)
            cout << ", ";
    }
    cout << endl;

    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numColumns; j++)
        {
            if (columnTypes[j] == 0)
                cout << intCells[i][j];
            else
                cout << textCells[i][j];

            if (j < numColumns - 1)
                cout << ", ";
        }
        cout << endl;
    }
}


void createSheet(string name)
{
    sheetName = name;
    numColumns = 0;
    numRows = 0;
}

bool isNumber(const string& str)
{
    int len = str.length();
    if (len == 0)
        return false;

    int start = 0;
    if (str[0] == '-')
    {
        if (len == 1)
            return false;
        start = 1;
    }

    for (int i = start; i < len; i++)
    {
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    return true;
}

int convertToInt(const string& str)
{
    try{
        return stoi(str);
    }
    catch (const invalid_argument&){
        return 0; //  handle error 
    }

}
  

void getColumnInfo(int colIndex)
{
    string input;
    string name;
    int type = 1;
    int i;

    cout << "Enter column " << (colIndex + 1) << " name: ";
    getline(cin, input, '\n');

    int parenPos = -1;
    for (i = 0; i < (int)input.length(); i++)
    {
        if (input[i] == '(')
        {
            parenPos = i;
            break;
        }
    }

    if (parenPos >= 0)
    {
       
        name = input.substr(0, parenPos); 

        
        while(name.length() > 0 && name.back() == ' ') {
            name.pop_back();
        }

        
        for (i = parenPos; i < (int)input.length(); i++)
        {
            if ((input[i] == 'I' || input[i] == 'i') &&
                (input[i + 1] == 'N' || input[i + 1] == 'n') &&
                (input[i + 2] == 'T' || input[i + 2] == 't'))
            {
                type = 0;
                break;
            }
        }
    }
    else
    {
        name = input;
    }
    columnNames[colIndex] = name;
    columnTypes[colIndex] = type;
}

