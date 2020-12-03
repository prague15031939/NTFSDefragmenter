#include "pch.h"
#include "Core.h"
#include <string>
#include <cwchar>

wchar_t* GetWC(char* c);

std::vector<DriveData*> __cdecl getDrives() 
{
    std::vector<DriveData*> DriveList;

    char buf[26];
    HANDLE hDisk;
    DWORD lSectorsPerCluster, lBytesPerSector, lNumberOfFreeClusters, lTotalNumberOfClusters;

    GetLogicalDriveStringsA(sizeof(buf), buf);

    wchar_t* DT[] = {
        new wchar_t[8]  {L"Unknown"} ,             // ��� ����� �� ����� ���� ���������
        new wchar_t[13] {L"Invalid path"},         // �������� ���� �������� ������������
        new wchar_t[10] {L"Removable" },           // ������� ��������(������)
        new wchar_t[6]  {L"Fixed"},                // ������������� ��������(������� ����)
        new wchar_t[14] {L"Network drive"},        // ������ �������� ���������(�������)
        new wchar_t[7]  {L"CD-ROM"},               // CD-ROM ������
        new wchar_t[9]  {L"RAM disk"}              // ���-����
    };            

    for (char* s = buf; *s; s += strlen(s) + 1)
    {
        DriveData* item = new DriveData();

        wcsncpy_s(item->Drive, GetWC(s), 4);
        wcsncpy_s(item->DriveType, DT[GetDriveTypeA(s)], 20);

        if (GetDiskFreeSpaceA(s, &lSectorsPerCluster, &lBytesPerSector, &lNumberOfFreeClusters, &lTotalNumberOfClusters))
        {
            std::to_wstring(lSectorsPerCluster).copy(item->SectorPerCluster, 5, 0);
            std::to_wstring(lBytesPerSector).copy(item->BytesPerSector, 5, 0);
            std::to_wstring((double)lNumberOfFreeClusters / 1024 * (double)lSectorsPerCluster / 1024 * (double)lBytesPerSector / 1024).copy(item->FreeSpace, 5, 0);
            std::to_wstring((double)lTotalNumberOfClusters / 1024 * (double)lSectorsPerCluster / 1024 * (double)lBytesPerSector / 1024).copy(item->FullSpace, 5, 0);
        }
        DriveList.push_back(item);
    }

    for (int i = 0; i < 7; ++i)
        delete[] DT[i];

    return DriveList;
}

wchar_t* GetWC(char* c)
{
    const size_t cSize = strlen(c) + 1;
    wchar_t wc[10];
    size_t s;
    mbstowcs_s(&s, wc, c, cSize);

    return wc;
}