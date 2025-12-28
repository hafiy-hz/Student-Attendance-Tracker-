#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

const int MAX_COLUMNS = 10;
const int MAX_ROWS = 100;
const int MAX_NAME_LENGHT = 100;

char sheetName[60];
char columnNames[10][60];
char textCells[100][10][60];
int columnTypes[10];
int intCells[100][10];
int numColumns = 0;
int numRows = 0;

// function
void createSheet(char name[]);
void getColumnInfo(int colIndex);
void insertRow();
void viewCSV();
void copyString(char dest[], char source[]);
int stringLength(char str[]);
bool isNumber(char str[]);
int convertToInt(char str[]);

//fiy
int main()
{
    char input[51];
    int numCols;
    char continueInsert;
    int i;

    cout << "===========================================\n";
    cout << "   STUDENT ATTENDANCE TRACKER - MILESTONE 1\n";
    cout << "===========================================\n\n";

    cout << "Enter Attendance sheet name: ";
    cin.getline(input, 50);
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
        cout << "Insert new attendace row" << endl;
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
    cout << "Enter StudentID: ";
    cin.getline(input, 50);

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
    cout << "-------------------------" << endl;
    cout << "End of Milestone 1 Output" << endl;
    cout << "-------------------------" << endl;

    return 0;
}


//fiy
void insertRow()
{
    if (numRows >= MAX_ROWS)
    {
        cout << "Error: Maximum number of rows reached." << endl;
        return;
    }

    char input[51];

    for (int i = 0; i < numColumns; i++)
    {
        int len = stringLength(columnNames[i]);
        bool isStatus = (len == 6) &&
                        (columnNames[i][0] == 'S' || columnNames[i][0] == 's');

        if (isStatus)
            cout << "Enter " << columnNames[i] << " (Present: 1, Absent: 0): ";
        else
            cout << "Enter " << columnNames[i] << ": ";

        cin.getline(input, 50);

        if (columnTypes[i] == 0)
        {
            if (isNumber(input))
            {
                intCells[numRows][i] = convertToInt(input);
            }
            else
            {
                cout << "Error: Invalid INT value. Please enter a number." << endl;
                i--;
            }
        }
        else
        {
            copyString(textCells[numRows][i], input);
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

void copyString(char dest[], char source[])
{
    int i = 0;
    while (source[i] != '\0')
    {
        dest[i] = source[i];
        i++;
    }
    dest[i] = '\0';
}

int stringLength(char str[])
{
    int len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}

void createSheet(char name[])
{
    copyString(sheetName, name);
    numColumns = 0;
    numRows = 0;
}

bool isNumber(char str[])
{
    int len = stringLength(str);
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

int convertToInt(char str[])
{
    int result = 0;
    int sign = 1;
    int start = 0;

    if (str[0] == '-')
    {
        sign = -1;
        start = 1;
    }

    for (int i = start; str[i] != '\0'; i++)
        result = result * 10 + (str[i] - '0');

    return result * sign;
}

void getColumnInfo(int colIndex)
{
    char input[51];
    char name[51];
    int type = 1;
    int i;

    cout << "Enter column " << (colIndex + 1) << " name: ";
    cin.getline(input, 50);

    int parenPos = -1;
    for (i = 0; input[i] != '\0'; i++)
    {
        if (input[i] == '(')
        {
            parenPos = i;
            break;
        }
    }

    if (parenPos >= 0)
    {
        for (i = 0; i < parenPos; i++)
            name[i] = input[i];

        name[i] = '\0';

        for (i = parenPos; input[i] != '\0'; i++)
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
        copyString(name, input);
    }

    copyString(columnNames[colIndex], name);
    columnTypes[colIndex] = type;
}
