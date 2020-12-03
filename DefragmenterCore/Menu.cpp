#include "pch.h"
#include "Core.h"
#include <string>
#include <sstream>
#include <cwchar>

DriveData* initDriveData(char *s);

std::vector<DriveData*> __cdecl getDrives() 
{
    std::vector<DriveData*> DriveList;

    char buf[26];
    HANDLE hDisk;
    DWORD lSectorsPerCluster, lBytesPerSector, lNumberOfFreeClusters, lTotalNumberOfClusters;

    GetLogicalDriveStringsA(sizeof(buf), buf);

    wchar_t* DT[] = {
        new wchar_t[8]  {L"Unknown"} ,             // тип диска не может быть определен
        new wchar_t[13] {L"Invalid path"},         // корневой путь является недопустимым
        new wchar_t[10] {L"Removable" },           // съемные носители(флешки)
        new wchar_t[6]  {L"Fixed"},                // фиксированные носители(жесткий диск)
        new wchar_t[14] {L"Network drive"},        // привод является удаленным(сетевым)
        new wchar_t[7]  {L"CD-ROM"},               // CD-ROM привод
        new wchar_t[9]  {L"RAM disk"}              // ОЗУ-диск
    };            

    for (char* s = buf; *s; s += strlen(s) + 1)
    {
        DriveData* item = initDriveData(s);
        wcsncpy_s(item->DriveType, DT[GetDriveTypeA(s)], 20);

        if (GetDiskFreeSpaceA(s, &lSectorsPerCluster, &lBytesPerSector, &lNumberOfFreeClusters, &lTotalNumberOfClusters))
        {
            std::to_wstring(lSectorsPerCluster).copy(item->SectorPerCluster, 20, 0);
            std::to_wstring(lBytesPerSector).copy(item->BytesPerSector, 20, 0);
            std::to_wstring((double)lNumberOfFreeClusters / 1024 * (double)lSectorsPerCluster / 1024 * (double)lBytesPerSector / 1024).copy(item->FreeSpace, 7, 0);
            std::to_wstring((double)lTotalNumberOfClusters / 1024 * (double)lSectorsPerCluster / 1024 * (double)lBytesPerSector / 1024).copy(item->FullSpace, 7, 0);
        }
        DriveList.push_back(item);
    }

    for (int i = 0; i < 7; ++i)
        delete[] DT[i];

    return DriveList;
}

DriveData* initDriveData(char* s)
{
    DriveData* dd = new DriveData();
    std::wostringstream wss;
    wss << s;
    wss.str().copy(dd->Drive, 30, 0);
    wss.str(std::wstring());

    return dd;
}