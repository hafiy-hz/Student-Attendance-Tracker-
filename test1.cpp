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
    return (str1[i] == '\0' && str2[i] == '\0');
}

// Function to create a sheet
void createSheet(char name[])
{
    copyString(sheetName, name);
    numColumns = 0;
    numRows = 0;
}

// Function to check if a string is a number
bool isNumber(char str[])
{
    int len = stringLength(str);
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
    int len = stringLength(str);

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

// Function to get column information from user input
void getColumnInfo(int colIndex)
{
    char input[51];
    char name[51];
    int type = 1; // Default to TEXT
    int i;

    cout << "Enter column " << (colIndex + 1) << " name: ";
    cin.getline(input, 50);

    // Find parenthesis position
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
        // Copy name part before parenthesis
        for (i = 0; i < parenPos; i++)
        {
            name[i] = input[i];
        }
        name[i] = '\0';

        // Remove trailing spaces
        int nameLen = stringLength(name);
        while (nameLen > 0 && name[nameLen - 1] == ' ')
        {
            name[nameLen - 1] = '\0';
            nameLen--;
        }

        // Check for INT in parentheses
        for (i = parenPos; input[i] != '\0' && input[i] != ')'; i++)
        {
            if ((input[i] == 'I' || input[i] == 'i') &&
                (input[i+1] == 'N' || input[i+1] == 'n') &&
                (input[i+2] == 'T' || input[i+2] == 't'))
            {
                type = 0; // INT type
                break;
            }
        }
    }
    else
    {
        // No parenthesis, use whole input as name
        copyString(name, input);
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
    char statusStr[51] = "Status";

    for (int i = 0; i < numColumns; i++)
    {
        // Check if this is Status column
        if (compareString(columnNames[i], statusStr))
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
                intCells[numRows][i] = convertToInt(input);
            }
            else
            {
                cout << "Error: Invalid INT value. Please enter a number." << endl;
                i--; // Retry this column
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
    // Print column names
    for (int i = 0; i < numColumns; i++)
    {
        cout << columnNames[i];
        if (i < numColumns - 1)
        {
            cout << ", ";
        }
    }
    cout << endl;

    // Print all rows
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numColumns; j++)
        {
            if (columnTypes[j] == 0)
            {
                cout << intCells[i][j];
            }
            else
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
    while (true)
    {
        if (cin >> numCols)
        {
            // Successfully read a number
            if (numCols >= 1 && numCols <= MAX_COLUMNS)
            {
                break; // Valid number, exit loop
            }
            else
            {
                cout << "Error: Invalid number of columns. Must be between 1 and 10." << endl;
            }
        }
        else
        {
            // Failed to read a number
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Error: Invalid input. Please enter a number." << endl;
        }
        cout << "Define number of columns (max 10): ";
    }
    cin.ignore();

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
