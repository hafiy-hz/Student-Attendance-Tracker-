<iostream>
<iomanip>
<fstream>
<string>

using namespace std;

const int MAX_COlUMNS = 10;
const int MAX_ROWS = 100;
const int MAX_NAME_LENGHT = 100

char sheetName[60];
char columnNames[10][60];
char textCells[100][10][60];
int columnTypes[10];
int intCells[100][10];
int numColumns;
int numRows;

int main()
{
    char input[51];
    int numCols;
    char continueInsert;
    int i;

    cout << ========================== << endl
    cout << Student Attendance Tracker << endl
    cout << ========================== << endl
    cout << endl;

    cout <<  "Enter Attendance sheet name: ";
    cin.getline(input, 50);
    createSheet(input);

    cout << "Attendance sheet \"" << input << "\" created successfully." << endl;
    cout << endl;"

    cout << "Enter number of columns (max 10):  ";
    while(true)
    {
        if (cin >> numCols)
        {
            if (numCols >= 1 && numcols <=  MAX_COlUMNS)
            {
                break;
            }
            else
            {
                cout << "Error: Invalid number of columns. Itmust  be  between 1 to 10"
            }
        }
        else
        {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Error: Invalid input. Please enter a number"
        }
        cout << "Enter number of columns (max 10):  ";
    }
    cin.ignore();
    numColumns = numCols;

    for (i = 0; i < numCols; i++)
    {
        getColumnInfo(i);
    }

    cout  << "Sheet structure created successfully." << endl;

    continueInsert = 'y';
    while (continueInsert == 'y' || continueInsert == 'Y')
    {
        cout << "------------------------"
        cout << "Insert new attendace row" <<  endl;
        cout << "------------------------"

        insertRow();

        cout << endl;
        cout << "Do you want to insert another row? (y/n): ";
        cin >> continueInsert;
         cin.ignore();
        cout << endl;
    }

    cout << "---------------------" <<  endl;
    cout << "View attendance sheet" <<  endl;
    cout << "---------------------" <<  endl;

    viewCSW();
    cout << endl;

    cout << "-------------------------" << endl;
    cout << "Basic Error Handling Demo" << endl;
    cout << "-------------------------" << endl;
    cout << "Enter StudentID: ";
    cin.getline(input, 50);

    if (isNumber(input))
    {
        int  testValue = convertToInt(input);
        cout << "Valid integer: " << testValue << endl;
    }
    else
    {
        cout <<  "Error: Invalid INT value."
    }
    cout << endl;

    cout << "-------------------------------------------" << endl;
    cout << "End of Milestone 1 Output" << endl;
    cout << "-------------------------------------------" << endl;

    return 0;
}
