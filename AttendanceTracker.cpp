// *********************************************************
// Program: Student Attendance Tracker - Milestone 2
// Course: CCP6114 Programming Fundamentals
// Lecture Class: TC5L
// Tutorial Class: TT15L
// Trimester: 2530
// Member_1: 252UC242D2 | ADAM SYAFIQ BIN SALLEHUDDIN | ADAM.SYAFIQ.SALLEHUDDIN1@STUDENT.MMU.EDU.MY | 012-361 3065
// Member_2: 252UC242QY | MUHAMMAD HAFIY AL-HAFIIZH BIN JOAHARI | MUHAMMAD.HAFIY.ALHAFIIZH1@STUDENT.MMU.EDU.MY | 016-212 3859
// Member_3: 252UC242NE | AIDAN KHALIQ BIN SHAHNAZARUDDIN | AIDAN.KHALIQ.SHAHNZARUDDI1@student.mmu.edu.my | 017-615 5250
// *********************************************************
// Task Distribution
// Member_1: Database handling [Reading, displaying and writing to a file]
// Member_2: 
// Member_3: 
// *********************************************************

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <sstream>

using namespace std;

// globals
const int MAX_COLUMNS = 10;
const int MAX_ROWS = 100;

string sheetName;
string columnNames[MAX_COLUMNS];
string textCells[MAX_ROWS][MAX_COLUMNS];
int columnTypes[MAX_COLUMNS];
int intCells[MAX_ROWS][MAX_COLUMNS];
int numColumns = 0;
int numRows = 0;

// Attendance Tracker core operations
void createSheet(string name);
void getColumnInfo(int colIndex);
void insertRow();
void updateRow();
void deleteRow();
void deleteSheet();
void showRowCount();
int getValidInt(string prompt); // (Aidan)
void ViewCSV();
bool isNumber(const string& str);
int convertToInt(const string& str);

void runAttendanceTracker(const string& databaseFile);

// TC5L database
string createSchoolTerm();
bool fileExists(const string& filename);
void readDatabase(const string& filename);
void loadDatabase(const string& filename);
void saveToCSV(const string& filename);


//fiy
// main
int main()
{
    cout << "===========================================\n";
    cout << "   STUDENT ATTENDANCE TRACKER - COMBINED\n";
    cout << "===========================================\n\n";

    string termName = createSchoolTerm();
    string databaseFile = termName + "_Database.csv";

    if (fileExists(databaseFile))
    {
        cout << "Existing database found.\n\n";
        readDatabase(databaseFile);
        
        // Load the database into the sheet structure
        loadDatabase(databaseFile);

        char choice;
        cout << "Would you like to update the attendance rows? (Y/N): ";
        cin >> choice;
        cin.ignore();
        
        // Validate user choice
        while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n')
        {
            cout << "Invalid input. Please enter Y or N: ";
            cin >> choice;
            cin.ignore();
        }

        if (choice == 'Y' || choice == 'y')
        {
            // Skip sheet creation since it's already loaded, go straight to menu
            bool done = false;
            while (!done)
            {
                int menuChoice;
                cout << "\n======= Attendance Menu (Sheet: " << sheetName << ") =======\n";
                cout << "1. View attendance sheet\n";
                cout << "2. Insert new row\n";
                cout << "3. Update existing row\n";
                cout << "4. Delete a row\n";
                cout << "5. Delete entire sheet\n";
                cout << "6. Show number of rows\n";
                cout << "7. Save and exit\n";
                cout << "8. Exit without saving\n";
                // Aidan: Advanced Error Handling
                menuChoice = getValidInt("Enter your choice (1-8): ");

                switch (menuChoice)
                {
                case 1:
                    ViewCSV();
                    break;
                case 2:
                    insertRow();
                    break;
                case 3:
                    updateRow();
                    break;
                case 4:
                    deleteRow();
                    break;
                case 5:
                    deleteSheet();
                    break;
                case 6:
                    showRowCount();
                    break;
                case 7:
                    ViewCSV();
                    saveToCSV(databaseFile);
                    done = true;
                    break;
                case 8:
                    cout << "Exiting without saving changes.\n";
                    done = true;
                    break;
                default:
                    cout << "Invalid choice. Please choose a number between 1 and 8.\n";
                    break;
                }
            }
        }
        else
            cout << "\nNo changes made. Program ended.\n";
    }
    else
    {
        cout << "No existing database found.\n";
        cout << "Creating new attendance database...\n\n";
        runAttendanceTracker(databaseFile);
    }

    return 0;
}

// Create a school term (database name) and return it
string createSchoolTerm()
{
    string termName;
    cout << "Enter term name: ";
    getline(cin, termName);
    
    // Validate non-empty term name
    if (termName.empty())
    {
        cout << "Warning: Empty term name. Using default name.\n";
        termName = "DefaultTerm";
    }
    
    cout << "Database \"" << termName << "\" created and loaded.\n\n";
    return termName;
}

// Check if a file exists in the filesystem
bool fileExists(const string& filename)
{
    ifstream file(filename);
    bool exists = file.good();
    file.close();
    return exists;
}

// Read an existing CSV database file and display it to the screen.
void readDatabase(const string& filename)
{
    ifstream inFile(filename);

    if (!inFile)
    {
        cout << "Error: Could not open database file \"" << filename << "\" for reading.\n\n";
        return;
    }

    string line;

    cout << "-------------------------------------------\n";
    cout << "Current Attendance Sheet\n";
    cout << "-------------------------------------------\n";

    while (getline(inFile, line))
        cout << line << endl;

    cout << endl;
    inFile.close();
}

// Load an existing CSV database file into the sheet structure
void loadDatabase(const string& filename)
{
    ifstream inFile(filename);

    if (!inFile)
    {
        cout << "Error: Could not open database file \"" << filename << "\" for reading.\n\n";
        return;
    }

    string line;
    
    // Extract sheet name from filename (remove path, extension, and "_Database" suffix)
    string sheetNameFromFile = filename;
    // Remove path if present
    size_t lastSlash = sheetNameFromFile.find_last_of("/\\");
    if (lastSlash != string::npos)
        sheetNameFromFile = sheetNameFromFile.substr(lastSlash + 1);
    
    // Remove "_Database.csv" or ".csv" extension
    if (sheetNameFromFile.length() > 12 && sheetNameFromFile.substr(sheetNameFromFile.length() - 12) == "_Database.csv")
        sheetNameFromFile = sheetNameFromFile.substr(0, sheetNameFromFile.length() - 12);
    else if (sheetNameFromFile.length() > 4 && sheetNameFromFile.substr(sheetNameFromFile.length() - 4) == ".csv")
        sheetNameFromFile = sheetNameFromFile.substr(0, sheetNameFromFile.length() - 4);
    
    createSheet(sheetNameFromFile);

    // Read header row (column names)
    if (getline(inFile, line))
    {
        // Parse column names from CSV
        stringstream ss(line);
        string cell;
        numColumns = 0;
        
        while (getline(ss, cell, ',') && numColumns < MAX_COLUMNS)
        {
            // Trim whitespace
            cell.erase(0, cell.find_first_not_of(" \t"));
            cell.erase(cell.find_last_not_of(" \t") + 1);
            
            columnNames[numColumns] = cell;
            // Determine type: 0 = integer, 1 = text
            columnTypes[numColumns] = (cell.find("INT") != string::npos ||
                                     cell.find("int") != string::npos) ? 0 : 1;
            numColumns++;
        }
    }

    // Read data rows
    numRows = 0;
    while (getline(inFile, line) && numRows < MAX_ROWS)
    {
        if (line.empty()) continue; // Skip empty lines
        
        stringstream ss(line);
        string cell;
        int colIndex = 0;
        
        while (getline(ss, cell, ',') && colIndex < numColumns)
        {
            // Trim whitespace
            cell.erase(0, cell.find_first_not_of(" \t"));
            cell.erase(cell.find_last_not_of(" \t") + 1);
            
            if (columnTypes[colIndex] == 0)
            {
                // Integer column
                if (isNumber(cell))
                    intCells[numRows][colIndex] = convertToInt(cell);
                else
                    intCells[numRows][colIndex] = 0;
            }
            else
            {
                // Text column
                textCells[numRows][colIndex] = cell;
            }
            colIndex++;
        }
        numRows++;
    }

    inFile.close();
    
    cout << "Database loaded: Sheet \"" << sheetName << "\" with " << numRows << " rows and " << numColumns << " columns.\n\n";
}

// Main loop to define the sheet, insert rows, and allow updates/deletes
void runAttendanceTracker(const string& databaseFile)
{
    string input;
    int numCols;
    char continueInsert;

    cout << "Enter Attendance sheet name: ";
    getline(cin, input);
    
    // Validate non-empty sheet name
    if (input.empty())
    {
        cout << "Warning: Empty sheet name. Using default name.\n";
        input = "AttendanceSheet";
    }
    
    createSheet(input);

    // ---------------------------------------------------------
    // AIDAN'S TASK: Replaced old "weak" loop with getValidInt
    // ---------------------------------------------------------
    while (true) {
        numCols = getValidInt("Enter number of columns (max 10): ");
        if (numCols >= 1 && numCols <= MAX_COLUMNS) break;
        cout << "Invalid number. Number must be between 1 and " << MAX_COLUMNS << ".\n";
    }
    // ---------------------------------------------------------

    // cin.ignore(); <--- REMOVED (getValidInt handles this automatically)
    numColumns = numCols;

    for (int i = 0; i < numCols; i++)
        getColumnInfo(i);

    // Initial data entry loop
    continueInsert = 'y';
    while (continueInsert == 'y' || continueInsert == 'Y')
    {
        insertRow();
        cout << "Insert another row? (y/n): ";
        cin >> continueInsert;
        cin.ignore();
    }

    // Menu loop for viewing, updating, deleting, and saving
    bool done = false;
    while (!done)
    {
        int choice;
        cout << "\n======= Attendance Menu =======\n";
        cout << "1. View attendance sheet\n";
        cout << "2. Insert new row\n";
        cout << "3. Update existing row\n";
        cout << "4. Delete a row\n";
        cout << "5. Delete entire sheet\n";
        cout << "6. Show number of rows\n";
        cout << "7. Save and exit\n";
        cout << "8. Exit without saving\n";

        // ---------------------------------------------------------
        // AIDAN'S TASK: Replaced old "weak" input with getValidInt
        // ---------------------------------------------------------
        choice = getValidInt("Enter your choice (1-8): ");
        // ---------------------------------------------------------

        switch (choice)
        {
        case 1:
            ViewCSV();
            break;
        case 2:
            insertRow();
            break;
        case 3:
            updateRow();
            break;
        case 4:
            deleteRow();
            break;
        case 5:
            deleteSheet();
            break;
        case 6:
            showRowCount();
            break;
        case 7:
            ViewCSV();
            saveToCSV(databaseFile);
            done = true;
            break;
        case 8:
            cout << "Exiting without saving changes.\n";
            done = true;
            break;
        default:
            cout << "Invalid choice. Please choose a number between 1 and 8.\n";
            break;
        }
    }
}

// Create a new sheet (clears existing rows/columns)
void createSheet(string name)
{
    sheetName = name;
    numColumns = 0;
    numRows = 0;
}

//fiy
// Insert a new row of data at the end of the sheet
void insertRow()
{
    if (numRows >= MAX_ROWS)
    {
        cout << "Error: Maximum number of rows reached.\n";
        return;
    }

    string input;

    for (int i = 0; i < numColumns; i++)
    {
        // Check if this is a Status column and provide special prompt
        bool isStatus = (columnNames[i].length() == 6) &&
                        (columnNames[i][0] == 'S' || columnNames[i][0] == 's') &&
                        (columnNames[i] == "Status" || columnNames[i] == "status");

        if (isStatus)
            cout << "Enter " << columnNames[i] << " (Present: 1, Absent: 0): ";
        else
            cout << "Enter " << columnNames[i] << ": ";

        getline(cin, input, '\n');

        // Convert column name to lowercase for case-insensitive comparison
        string lowerColName = "";
        for (char& c : columnNames[i]) {
            lowerColName += tolower(c);
        }

        // Enforce integer validation for INT columns or StudentID columns
        bool enforceInt = (columnTypes[i] == 0) || (lowerColName == "studentid") || (lowerColName == "student id");

        // Special validation for Status column: only allow 0 or 1
        if (isStatus)
        {
            if (input == "0" || input == "1")
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
                cout << "Error: Status must be 0 (Absent) or 1 (Present) only. Please try again.\n";
                i--;
            }
        }
        else if (enforceInt)
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
                cout << "Error: Invalid INT value. Please enter a number.\n";
                i--; 
            }
        }
        else
        {
            textCells[numRows][i] = input; 
        }
    }

    numRows++;
    cout << "Row inserted successfully.\n";
}

//fiy
// Update an existing row by index
void updateRow()
{
    if (numRows == 0)
    {
        cout << "No rows to update.\n";
        return;
    }

    // Aidan: Error Handling
    string prompt = "Enter the row number to update (1 to " + to_string(numRows) + "): ";
    int rowIndex = getValidInt(prompt);

    if (rowIndex < 1 || rowIndex > numRows)
    {
        cout << "Row number out of range.\n";
        return;
    }

    rowIndex--; // convert to 0-based index

    string input;
    for (int i = 0; i < numColumns; i++)
    {
        // Check if this is a Status column and provide special prompt
        bool isStatus = (columnNames[i].length() == 6) &&
                        (columnNames[i][0] == 'S' || columnNames[i][0] == 's') &&
                        (columnNames[i] == "Status" || columnNames[i] == "status");

        if (isStatus)
            cout << "Enter new value for " << columnNames[i] << " (Present: 1, Absent: 0): ";
        else
            cout << "Enter new value for " << columnNames[i] << ": ";

        getline(cin, input);

        // Convert column name to lowercase for case-insensitive comparison
        string lowerColName = "";
        for (char& c : columnNames[i]) {
            lowerColName += tolower(c);
        }

        // Enforce integer validation for INT columns or StudentID columns
        bool enforceInt = (columnTypes[i] == 0) || (lowerColName == "studentid") || (lowerColName == "student id");

        // Special validation for Status column: only allow 0 or 1
        if (isStatus)
        {
            if (input == "0" || input == "1")
            {
                if (columnTypes[i] == 0)
                {
                    intCells[rowIndex][i] = convertToInt(input);
                }
                else
                {
                    textCells[rowIndex][i] = input;
                }
            }
            else
            {
                cout << "Error: Status must be 0 (Absent) or 1 (Present) only. Please try again.\n";
                i--; // Retry this column
                continue;
            }
        }
        else if (enforceInt)
        {
            if (isNumber(input))
            {
                if (columnTypes[i] == 0)
                {
                    intCells[rowIndex][i] = convertToInt(input);
                }
                else
                {
                    textCells[rowIndex][i] = input;
                }
            }
            else
            {
                cout << "Error: Invalid INT value. Keeping previous value for this column.\n";
            }
        }
        else
        {
            textCells[rowIndex][i] = input;
        }
    }

    cout << "Row updated successfully.\n";
}

//fiy
// Delete a row by index and shift remaining rows up
void deleteRow()
{
    if (numRows == 0)
    {
        cout << "No rows to delete.\n";
        return;
    }

    // Aidan: Error Handling
    string prompt = "Enter the row number to delete (1 to " + to_string(numRows) + "): ";
    int rowIndex = getValidInt(prompt);

    if (rowIndex < 1 || rowIndex > numRows)
    {
        cout << "Row number out of range.\n";
        return;
    }

    rowIndex--; // convert to 0-based index

    for (int i = rowIndex; i < numRows - 1; i++)
    {
        for (int j = 0; j < numColumns; j++)
        {
            intCells[i][j] = intCells[i + 1][j];
            textCells[i][j] = textCells[i + 1][j];
        }
    }

    numRows--;
    cout << "Row deleted successfully.\n";
}

// Delete the entire sheet (clear all data, columns, and rows)
void deleteSheet()
{
    if (numRows == 0 && numColumns == 0)
    {
        cout << "Sheet is already empty. Nothing to delete.\n";
        return;
    }

    char confirm;
    cout << "WARNING: This will delete ALL data in the sheet (all rows and columns).\n";
    cout << "Are you sure you want to continue? (Y/N): ";
    cin >> confirm;
    cin.ignore();

    if (confirm != 'Y' && confirm != 'y')
    {
        cout << "Delete cancelled. Sheet remains unchanged.\n";
        return;
    }

    // Clear all column names and types
    for (int i = 0; i < MAX_COLUMNS; i++)
    {
        columnNames[i] = "";
        columnTypes[i] = 1; // default to text
    }

    // Clear all cell data
    for (int i = 0; i < MAX_ROWS; i++)
    {
        for (int j = 0; j < MAX_COLUMNS; j++)
        {
            intCells[i][j] = 0;
            textCells[i][j] = "";
        }
    }

    // Reset counters
    numColumns = 0;
    numRows = 0;
    sheetName = "";

    cout << "Entire sheet deleted successfully. Sheet is now empty.\n";
}

// Aidan: Upgraded Row Counter
void showRowCount()
{
    cout << "\n-----------------------------" << endl;
    cout << "   Attendance Sheet Stats    " << endl;
    cout << "-----------------------------" << endl;
    cout << "Total Records (Rows): " << numRows << endl;
    cout << "Remaining Capacity:   " << MAX_ROWS - numRows << endl;
    cout << "-----------------------------" << endl;
}

// Display the current attendance sheet in CSV format to the screen
void ViewCSV()
{
    if (numColumns == 0)
    {
        cout << "No columns defined.\n";
        return;
    }

    // Print column headers
    for (int i = 0; i < numColumns; i++)
    {
        cout << columnNames[i];
        if (i < numColumns - 1) cout << ", ";
    }
    cout << endl;

    // Print all data rows
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numColumns; j++)
        {
            if (columnTypes[j] == 0)
                cout << intCells[i][j];
            else
                cout << textCells[i][j];

            if (j < numColumns - 1) cout << ", ";
        }
        cout << endl;
    }
}



// Save the current attendance sheet to a CSV file
void saveToCSV(const string& filename)
{
    ofstream file(filename);

    if (!file)
    {
        cout << "Error: Could not open file \"" << filename << "\" for writing.\n";
        cout << "Data was not saved.\n";
        return;
    }

    // Write column headers
    for (int i = 0; i < numColumns; i++)
    {
        file << columnNames[i];
        if (i < numColumns - 1) file << ",";
    }
    file << "\n";

    // Write all data rows
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numColumns; j++)
        {
            if (columnTypes[j] == 0)
                file << intCells[i][j];
            else
                file << textCells[i][j];

            if (j < numColumns - 1) file << ",";
        }
        file << "\n";
    }

    file.close();
    
    if (file.fail())
    {
        cout << "Error: Failed to write data to file.\n";
    }
    else
    {
        cout << "\nAttendance saved to database successfully.\n";
    }
}

//fiy
// Validate if a string represents a valid integer number
bool isNumber(const string& str)
{
    if (str.empty()) return false;
    
    // Allow negative sign only at the start
    int start = 0;
    if (str[0] == '-')
    {
        if (str.length() == 1) return false; // Just a minus sign is invalid
        start = 1;
    }
    
    // Check all remaining characters are digits
    for (int i = start; i < str.length(); i++)
    {
        if (!isdigit(str[i])) return false;
    }
    
    return true;
}

// Convert string to integer with error handling
int convertToInt(const string& str)
{
    try
    {
        return stoi(str);
    }
    catch (const invalid_argument&)
    {
        cout << "Warning: Invalid number format. Using 0 as default.\n";
        return 0;
    }
    catch (const out_of_range&)
    {
        cout << "Warning: Number out of range. Using 0 as default.\n";
        return 0;
    }
}

//fiy
// Get column name and determine its data type
void getColumnInfo(int colIndex)
{
    string input;
    cout << "Enter column " << colIndex + 1 << " name: ";
    getline(cin, input);

    // Validate non-empty column name
    if (input.empty())
    {
        cout << "Warning: Empty column name. Using default name.\n";
        input = "Column" + to_string(colIndex + 1);
    }

    columnNames[colIndex] = input;
    // Determine type: 0 = integer, 1 = text
    columnTypes[colIndex] = (input.find("INT") != string::npos ||
                             input.find("int") != string::npos) ? 0 : 1;
}

// Aidan: Advanced Error Handling Helper
int getValidInt(string prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        // Attempt to read an integer
        if (cin >> value)
        {
            // Validation: Check if the input buffer is clean (no hidden letters like 12abc)
            if (cin.peek() == '\n' || cin.peek() == EOF)
            {
                cin.ignore(); // Clear the newline character left in buffer
                return value; // Success! Return the clean integer
            }
        }
        
        // If we reach here, the input was invalid
        cout << "Error: Invalid input! Please enter a numeric integer value." << endl;
        cin.clear();            // Reset the 'fail' state of cin
        cin.ignore(1000, '\n'); // Clear the bad input from memory
    }
}