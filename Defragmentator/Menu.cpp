#include "Menu.h"

void StartWindow()
{
	cout << "Welcome file system defragmenter" << endl;
	cout << "Press any key to start work...";
	_getch();
}

// ���������� ��� ��������� �����
void outputDrives() {
    char buf[26];
    HANDLE hDisk;
    DWORD lSectorsPerCluster, lBytesPerSector, lNumberOfFreeClusters, lTotalNumberOfClusters;
    LPDWORD SectorsPerCluster = &lSectorsPerCluster;
    LPDWORD BytesPerSector = &lBytesPerSector;
    LPDWORD NumberOfFreeClusters = &lNumberOfFreeClusters;
    LPDWORD TotalNumberOfClusters = &lTotalNumberOfClusters;

    GetLogicalDriveStringsA(sizeof(buf), buf); //��������� ����� ��������, ������������ ���������� ����� � �������
    char* DT[] = { new char[8] {"Unknown"} ,   //��� ����� �� ����� ���� ���������
        new char[13] {"Invalid path"},         //�������� ���� �������� ������������
        new char[10] {"Removable" },           //������� ��������(������)
        new char[6] {"Fixed"},                 //������������� ��������(������� ����)
        new char[14] {"Network drive"},        //������ �������� ���������(�������)
        new char[7] {"CD-ROM"},                //CD-ROM ������
        new char[9] {"RAM disk"} };            //���-����

    system("CLS");
    
    outputTable();
    for (char* s = buf; *s; s += strlen(s) + 1)
    {
        cout << left << setw(10) << s;
        cout << left << setw(15) << DT[GetDriveTypeA(s)];
        if (GetDiskFreeSpaceA(s, SectorsPerCluster, BytesPerSector, NumberOfFreeClusters, TotalNumberOfClusters))
        {
            cout << setw(20) << lSectorsPerCluster;
            cout << setw(20) << lBytesPerSector;
            cout << fixed;
            cout.precision(2);
            cout << setw(15) << (double)lNumberOfFreeClusters / 1024 * (double)lSectorsPerCluster / 1024 * (double)lBytesPerSector / 1024;
            cout << setw(15) << (double)lTotalNumberOfClusters / 1024 * (double)lSectorsPerCluster / 1024 * (double)lBytesPerSector / 1024 << endl;
        }
        else
        {
            cout << "Error: " << GetLastError() << endl;
        }
    }
    cout << "Press Esc to complete work" << endl;

    for (int i = 0; i < 7; ++i)
    {
        delete[] DT[i];
    }
}

void outputTable()
{
    cout << left << setw(10) << "Drive";
    cout << left << setw(15) << "Drive Type";
    cout << left << setw(20) << "Sector Per Cluster";
    cout << left << setw(20) << "Bytes Per Sector";
    cout << left << setw(15) << "Free Space(GB)";
    cout << left << setw(15) << "Full Space(GB)" << endl;
}