// Milestone 1

#include <iostream>
#include <string>

using namespace std;

const int MAX_COLUMNS = 10;
const int MAX_ROWS = 100;

char sheetName[60];
char columnNames[MAX_COLUMNS][60];
char textCells[MAX_ROWS][MAX_COLUMNS][60];
int intCells[MAX_ROWS][MAX_COLUMNS];
int columnTypes[MAX_COLUMNS]; // 0 = INT, 1 = TEXT
int numColumns = 0;
int numRows = 0;

void createSheet(char name[]);
void getColumnInfo(int colIndex);
void insertRow();
void viewCSV();
void copyString(char dest[], char source[]);
int stringLength(char str[]);
bool isNumber(char str[]);
int convertToInt(char str[]);

// main
int main()
{
    char input[51];
    int numCols;
    char continueInsert;

    cout << "===========================================\n";
    cout << "   STUDENT ATTENDANCE TRACKER - MILESTONE 1\n";
    cout << "===========================================\n\n";

    cout << "Enter attendance sheet name: ";
    cin.getline(input, 50);
    createSheet(input);

    cout << "Attendance sheet \"" << sheetName << "\" created successfully.\n\n";

    cout << "Define number of columns (max 10): ";
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
            cout << "Define number of columns (max 10): ";
        }
    }
    cin.ignore();
    numColumns = numCols;

    for (int i = 0; i < numColumns; i++)
    {
        getColumnInfo(i);
    }

    cout << "\nSheet structure created successfully.\n\n";

    continueInsert = 'y';
    while (continueInsert == 'y' || continueInsert == 'Y')
    {
        cout << "-------------------------------------------\n";
        cout << "Insert New Attendance Row\n";
        cout << "-------------------------------------------\n";
        insertRow();

        cout << "\nDo you want to insert another row? (y/n): ";
        cin >> continueInsert;
        cin.ignore();
        cout << endl;
    }

    cout << "-------------------------------------------\n";
    cout << "View Attendance Sheet (CSV Mode)\n";
    cout << "-------------------------------------------\n";
    viewCSV();

    cout << "\n-------------------------------------------\n";
    cout << "Basic Error Handling Demo\n";
    cout << "-------------------------------------------\n";
    cout << "Enter StudentID: ";
    cin.getline(input, 50);

    if (!isNumber(input))
    {
        cout << "Error: Invalid INT value. Please enter a number.\n";
    }
    else
    {
        cout << "Valid integer entered.\n";
    }

    cout << "\n-------------------------------------------\n";
    cout << "End of Milestone 1 Output\n";
    cout << "-------------------------------------------\n";

    return 0;
}

// functions

void createSheet(char name[])
{
    copyString(sheetName, name);
    numRows = 0;
}

void getColumnInfo(int colIndex)
{
    char input[50];
    char name[50];
    int type = 1; // TEXT by default

    cout << "Enter column " << colIndex + 1 << " name: ";
    cin.getline(input, 50);

    int i = 0;
    while (input[i] != '(' && input[i] != '\0')
    {
        name[i] = input[i];
        i++;
    }
    name[i] = '\0';

    for (int j = 0; input[j] != '\0'; j++)
    {
        if ((input[j] == 'I' || input[j] == 'i') &&
            (input[j + 1] == 'N' || input[j + 1] == 'n') &&
            (input[j + 2] == 'T' || input[j + 2] == 't'))
        {
            type = 0;
            break;
        }
    }

    copyString(columnNames[colIndex], name);
    columnTypes[colIndex] = type;
}

void insertRow()
{
    if (numRows >= MAX_ROWS)
    {
        cout << "Error: Maximum rows reached.\n";
        return;
    }

    char input[50];

    for (int i = 0; i < numColumns; i++)
    {
        cout << "Enter " << columnNames[i];

        if (stringLength(columnNames[i]) == 6 &&
            (columnNames[i][0] == 'S' || columnNames[i][0] == 's'))
        {
            cout << " (Present: 1, Absent: 0)";
        }

        cout << ": ";
        cin.getline(input, 50);

        if (columnTypes[i] == 0)
        {
            if (!isNumber(input))
            {
                cout << "Error: Invalid INT value. Please enter a number.\n";
                i--;
                continue;
            }
            intCells[numRows][i] = convertToInt(input);
        }
        else
        {
            copyString(textCells[numRows][i], input);
        }
    }

    numRows++;
    cout << "Row inserted successfully.\n";
}

void viewCSV()
{
    for (int i = 0; i < numColumns; i++)
    {
        cout << columnNames[i];
        if (i < numColumns - 1) cout << ", ";
    }
    cout << endl;

    for (int r = 0; r < numRows; r++)
    {
        for (int c = 0; c < numColumns; c++)
        {
            if (columnTypes[c] == 0)
                cout << intCells[r][c];
            else
                cout << textCells[r][c];

            if (c < numColumns - 1) cout << ", ";
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
    while (str[len] != '\0') len++;
    return len;
}

bool isNumber(char str[])
{
    if (str[0] == '\0') return false;
    int i = (str[0] == '-') ? 1 : 0;
    for (; str[i] != '\0'; i++)
        if (str[i] < '0' || str[i] > '9') return false;
    return true;
}

int convertToInt(char str[])
{
    int result = 0, sign = 1, i = 0;
    if (str[0] == '-')
    {
        sign = -1;
        i = 1;
    }
    for (; str[i] != '\0'; i++)
        result = result * 10 + (str[i] - '0');
    return result * sign;
}
