#include "Defrager.h"
#include "CheckUser.h"
#include "Menu.h"
#include "DriveOperation.h"


int main()
{
    setlocale(LC_ALL, "Russian");

    char drive;
    char* dr = new char[7]{ "\\\\.\\C:" };
    int res = 0;

    if (!IsUserAdmin())
    {
        AdminErrorOutput();
        return 0;
    }

    StartWindow();
    
    while (true)
    {
        outputDrives();
        selectDrive(drive);        
        if (drive == 27)
        {
            delete[] dr;

            return 0;
        }
        if (drive <= 65 || drive >= 90 && drive <= 97 || drive >= 122 && drive != 27)
        {
            cout << "\n\nIncorrect input!" << endl;
            cout << "Press any key..." << endl;
            _getch();
            continue;
        }

        dr[4] = drive;

        system("CLS");
        cout << "You selected drive " << drive << ":\\" << endl;
        int choise = 0;

        cout << "\n1)Defragment" << endl;
        cout << "2)Show information" << endl;
        cout << "0)Return to main menu" << endl;
        cout << "\nChoose action: ";
        choise = _getche();

        switch (choise)
        {
        case '1':
            res = WorkIn(drive, dr, true);
            break;

        case '2':
            getDriveInfo(drive);
            continue;

        case '0':
            continue;

        default:
            cout << "\n\nIncorrect input!" << endl;
            cout << "Try again" << endl;
            cout << "Press any key..." << endl;
            _getch();
            continue;
        }

        switch (res)
        {
        case 0: 
            cout << "\nDefragment success" << endl;
            break;

        case -1:
            cout << "\nDefaragmentation not required" << endl;
            break;

        default:
            cout << "\nDefragmentation failed! (0x" << res << ")" << endl;
        }

        cout << "Press any key..." << endl;
        _getch();
    }

    cout << "Press any key..." << endl;
    _getch();

    delete[] dr;

    return 0;
}