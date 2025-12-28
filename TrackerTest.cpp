#include <iostream>
#include <iomanip>
#include <fstream>
#include <string> // Replaces cstring
#include <cctype> // for isdigit

using namespace std;

// Constants
const int MAX_COLUMNS = 10;
const int MAX_ROWS = 100;

// Global Variables
string sheetName;
string columnNames[MAX_COLUMNS];
string textCells[MAX_ROWS][MAX_COLUMNS]; // Changed from char array to string
int columnTypes[MAX_COLUMNS];            // 0 for INT, 1 for TEXT
int intCells[MAX_ROWS][MAX_COLUMNS];     // Stores integer data
int numColumns = 0;
int numRows = 0;

// Function Prototypes
void createSheet(string name);
void getColumnInfo(int colIndex);
void insertRow();
void viewCSV();
// Removed copyString and stringLength prototypes as they are no longer needed
bool isNumber(const string& str);
int convertToInt(const string& str);

int main()
{
    string input; // Changed from char array to string
    int numCols;
    char continueInsert;
    int i;

    cout << "===========================================\n";
    cout << "   STUDENT ATTENDANCE TRACKER - MILESTONE 1\n";
    cout << "===========================================\n\n";

    cout << "Enter Attendance sheet name: ";
    getline(cin, input); // Using getline for std::string
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
            cin.clear(); // Clear error flags
            cin.ignore(100, '\n'); // Discard bad input
            cout << "Enter number of columns (max 10): ";
        }
    }

    cin.ignore(); // Clear the newline left in buffer by cin >> numCols
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
        cin.ignore(); // Clear buffer for next loop iteration
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
    cout << "Enter StudentID (to test INT validation): ";
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
    cout << "-------------------------" << endl;
    cout << "End of Milestone 1 Output" << endl;
    cout << "-------------------------" << endl;

    return 0;
}

// Function Definitions

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
        // Check if column is named "Status"
        // Using string::length() and direct character access
        bool isStatus = (columnNames[i].length() == 6) && 
                        (columnNames[i][0] == 'S' || columnNames[i][0] == 's');

        if (isStatus)
            cout << "Enter " << columnNames[i] << " (Present: 1, Absent: 0): ";
        else
            cout << "Enter " << columnNames[i] << ": ";

        getline(cin, input);

        // If column type is INT (0)
        if (columnTypes[i] == 0)
        {
            if (isNumber(input))
            {
                intCells[numRows][i] = convertToInt(input);
            }
            else
            {
                cout << "Error: Invalid INT value. Please enter a number." << endl;
                i--; // Decrement counter to retry this specific column
            }
        }
        else // If column type is TEXT (1)
        {
            textCells[numRows][i] = input; // Direct assignment works for strings
        }
    }

    numRows++;
    cout << "Row inserted successfully." << endl;
}

void viewCSV()
{
    // Print Header
    for (int i = 0; i < numColumns; i++)
    {
        cout << columnNames[i];
        if (i < numColumns - 1)
            cout << ", ";
    }
    cout << endl;

    // Print Data
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
    sheetName = name; // Direct string assignment
    numColumns = 0;
    numRows = 0;
}

// Checks if a string represents a valid integer
bool isNumber(const string& str)
{
    if (str.empty())
        return false;

    size_t start = 0;
    // Handle negative numbers
    if (str[0] == '-')
    {
        if (str.length() == 1)
            return false; // "-" alone is not a number
        start = 1;
    }

    for (size_t i = start; i < str.length(); i++)
    {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

// Uses std::stoi for standard conversion
int convertToInt(const string& str)
{
    try {
        return stoi(str);
    } catch (...) {
        return 0; // Simple fallback
    }
}

void getColumnInfo(int colIndex)
{
    string input;
    string name;
    int type = 1; // Default to TEXT

    cout << "Enter column " << (colIndex + 1) << " name (e.g., Age(INT)): ";
    getline(cin, input);

    // Using string::find to locate parenthesis
    size_t parenPos = input.find('(');

    if (parenPos != string::npos)
    {
        // Use substr to extract the name
        name = input.substr(0, parenPos);

        // Check content inside parenthesis for "INT"
        // We extract the part after the paren and scan it
        string typeStr = input.substr(parenPos);
        
        bool foundInt = false;
        // Scan for the sequence I-N-T
        for (size_t i = 0; i < typeStr.length() - 2; i++) {
            if ((typeStr[i] == 'I' || typeStr[i] == 'i') &&
                (typeStr[i+1] == 'N' || typeStr[i+1] == 'n') &&
                (typeStr[i+2] == 'T' || typeStr[i+2] == 't')) {
                foundInt = true;
                break;
            }
        }
        
        if (foundInt) {
            type = 0; // Set type to INT
        }
    }
    else
    {
        name = input;
    }

    columnNames[colIndex] = name;
    columnTypes[colIndex] = type;
}