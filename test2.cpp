// Student Attendance Tracker - Milestone 1
// This program creates an attendance sheet, defines columns,
// inserts attendance rows, and displays the sheet in CSV format

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

// Constants
const int MAX_COLUMNS = 10;
const int MAX_ROWS = 100;
const int MAX_NAME_LENGTH = 50;

// Global arrays to store data
char sheetName[51];
char columnNames[10][51];
int columnTypes[10];  // 0 means INT, 1 means TEXT
int intCells[100][10];
char textCells[100][10][51];
int numColumns;
int numRows;

// Function to create a sheet
void createSheet(char name[])
{
    int i = 0;
    while (name[i] != '\0')
    {
        sheetName[i] = name[i];
        i++;
    }
    sheetName[i] = '\0';
    numColumns = 0;
    numRows = 0;
}

// Function to check if a string is a number
bool isNumber(char str[])
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }

    if (len == 0)
    {
        return false;
    }

    int start = 0;
    if (str[0] == '-')
    {
        start = 1;
        if (len == 1)
        {
            return false;
        }
    }

    for (int i = start; i < len; i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return false;
        }
    }

    return true;
}

// Function to convert string to integer
int convertToInt(char str[])
{
    int result = 0;
    int sign = 1;
    int start = 0;
    int len = 0;

    while (str[len] != '\0')
    {
        len++;
    }

    if (str[0] == '-')
    {
        sign = -1;
        start = 1;
    }

    for (int i = start; i < len; i++)
    {
        result = result * 10 + (str[i] - '0');
    }

    return result * sign;
}

// Function to copy a string
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

// Function to compare two strings
bool compareString(char str1[], char str2[])
{
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0')
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
        i++;
    }
    if (str1[i] == '\0' && str2[i] == '\0')
    {
        return true;
    }
    return false;
}

// Function to find length of string
int stringLength(char str[])
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

// Function to get column information from user input
void getColumnInfo(int colIndex)
{
    char input[51];
    char name[51];
    int type;
    int i, j;
    bool foundParen = false;
    int parenPos = -1;

    cout << "Enter column " << (colIndex + 1) << " name: ";
    cin.getline(input, 50);

    // Find if there is a parenthesis
    i = 0;
    while (input[i] != '\0')
    {
        if (input[i] == '(')
        {
            foundParen = true;
            parenPos = i;
            break;
        }
        i++;
    }

    if (foundParen)
    {
        // Copy name part (before parenthesis)
        for (i = 0; i < parenPos; i++)
        {
            name[i] = input[i];
        }
        name[i] = '\0';

        // Remove trailing spaces from name
        int nameLen = stringLength(name);
        while (nameLen > 0 && name[nameLen - 1] == ' ')
        {
            name[nameLen - 1] = '\0';
            nameLen--;
        }

        // Check if it says INT
        bool foundINT = false;
        i = parenPos;
        while (input[i] != '\0' && input[i] != ')')
        {
            if (input[i] == 'I' && input[i+1] == 'N' && input[i+2] == 'T')
            {
                foundINT = true;
                break;
            }
            if (input[i] == 'i' && input[i+1] == 'n' && input[i+2] == 't')
            {
                foundINT = true;
                break;
            }
            i++;
        }

        if (foundINT)
        {
            type = 0; // INT type
        }
        else
        {
            type = 1; // TEXT type
        }
    }
    else
    {
        // No parenthesis, just copy the whole thing as name
        copyString(name, input);
        type = 1; // TEXT type
    }

    copyString(columnNames[colIndex], name);
    columnTypes[colIndex] = type;
}

// Function to insert a new row
void insertRow()
{
    if (numRows >= MAX_ROWS)
    {
        cout << "Error: Maximum number of rows reached." << endl;
        return;
    }

    char input[51];
    char statusStr1[51] = "Status";
    char statusStr2[51] = "status";

    for (int i = 0; i < numColumns; i++)
    {
        // Check if this is Status column
        if (compareString(columnNames[i], statusStr1) || compareString(columnNames[i], statusStr2))
        {
            cout << "Enter " << columnNames[i] << " (Present: 1, Absent: 0): ";
        }
        else
        {
            cout << "Enter " << columnNames[i] << ": ";
        }

        cin.getline(input, 50);

        if (columnTypes[i] == 0) // INT type
        {
            if (isNumber(input))
            {
                int intValue = convertToInt(input);
                intCells[numRows][i] = intValue;
            }
            else
            {
                cout << "Error: Invalid INT value. Please enter a number." << endl;
                i--; // Go back to retry this column
            }
        }
        else // TEXT type
        {
            copyString(textCells[numRows][i], input);
        }
    }

    numRows++;
    cout << "Row inserted successfully." << endl;
}

// Function to display the sheet in CSV format
void viewCSV()
{
    int i, j;

    // Print column names
    for (i = 0; i < numColumns; i++)
    {
        cout << columnNames[i];
        if (i < numColumns - 1)
        {
            cout << ", ";
        }
    }
    cout << endl;

    // Print all rows
    for (i = 0; i < numRows; i++)
    {
        for (j = 0; j < numColumns; j++)
        {
            if (columnTypes[j] == 0) // INT type
            {
                cout << intCells[i][j];
            }
            else // TEXT type
            {
                cout << textCells[i][j];
            }

            if (j < numColumns - 1)
            {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

// Main function
int main()
{
    char input[51];
    int numCols;
    char continueInsert;
    int i;

    // Display header
    cout << "===========================================" << endl;
    cout << "   STUDENT ATTENDANCE TRACKER - MILESTONE 1" << endl;
    cout << "===========================================" << endl;
    cout << endl;

    // Create attendance sheet
    cout << "Enter attendance sheet name: ";
    cin.getline(input, 50);
    createSheet(input);
    cout << "Attendance sheet \"" << input << "\" created successfully." << endl;
    cout << endl;

    // Get number of columns
    cout << "Define number of columns (max 10): ";
    cin >> numCols;
    cin.ignore();

    while (numCols < 1 || numCols > MAX_COLUMNS)
    {
        cout << "Error: Invalid number of columns. Must be between 1 and 10." << endl;
        cout << "Define number of columns (max 10): ";
        cin >> numCols;
        cin.ignore();
    }

    numColumns = numCols;

    // Get column information
    for (i = 0; i < numCols; i++)
    {
        getColumnInfo(i);
    }

    cout << endl;
    cout << "Sheet structure created successfully." << endl;
    cout << endl;

    // Insert attendance rows
    continueInsert = 'y';
    while (continueInsert == 'y' || continueInsert == 'Y')
    {
        cout << "-------------------------------------------" << endl;
        cout << "Insert New Attendance Row" << endl;
        cout << "-------------------------------------------" << endl;

        insertRow();

        cout << endl;
        cout << "Do you want to insert another row? (y/n): ";
        cin >> continueInsert;
        cin.ignore();
        cout << endl;
    }

    // View attendance sheet in CSV mode
    cout << "-------------------------------------------" << endl;
    cout << "View Attendance Sheet (CSV Mode)" << endl;
    cout << "-------------------------------------------" << endl;
    viewCSV();
    cout << endl;

    // Basic Error Handling Demo
    cout << "-------------------------------------------" << endl;
    cout << "Basic Error Handling Demo" << endl;
    cout << "-------------------------------------------" << endl;
    cout << "Enter StudentID: ";
    cin.getline(input, 50);

    if (isNumber(input))
    {
        int testValue = convertToInt(input);
        cout << "Valid integer: " << testValue << endl;
    }
    else
    {
        cout << "Error: Invalid INT value. Please enter a number." << endl;
    }
    cout << endl;

    cout << "-------------------------------------------" << endl;
    cout << "End of Milestone 1 Output" << endl;
    cout << "-------------------------------------------" << endl;

    return 0;
}
