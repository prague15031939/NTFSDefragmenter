#include "DriveOperation.h"
#include "Operators.h"

DriveInfo* driveInfo = new DriveInfo();

void selectDrive(char& drive)
{
    cout << "\nYour choise: ";
    drive = _getche();
    drive = toupper(drive);
    cout << "\n\n";
}

void getDriveInfo(char& drive)
{
    PNTFS_VOLUME_DATA_BUFFER info;
    DWORD cbWritten;
    HANDLE hDrive;
    char name[5];
    name[0] = drive;
    name[1] = ':';
    name[2] = 0;

    hDrive = CreateFileA(name,
        GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL);

    info = (PNTFS_VOLUME_DATA_BUFFER)malloc(sizeof(NTFS_VOLUME_DATA_BUFFER));

    if (DeviceIoControl(hDrive,
        FSCTL_GET_NTFS_VOLUME_DATA,
        NULL,
        0,
        info,
        sizeof(NTFS_VOLUME_DATA_BUFFER),
        &cbWritten,
        NULL))
    {
        outputDriveInfo(info, drive);
    }
    else
    {
        cout << "Error: " << GetLastError() << endl;
    }

    CloseHandle(hDrive);
    free(info);
}

void outputDriveInfo(PNTFS_VOLUME_DATA_BUFFER info, char& drive)
{
    system("CLS");
    cout << "Drive " << drive << ":\\" << endl << endl;
    cout << "1. Volume Serial Number: " << info->VolumeSerialNumber << endl;
    cout << "2. Number of sectors: " << info->NumberSectors << endl;
    cout << "3. Total number of clusters: " << info->TotalClusters << endl;
    cout << "4. Number of free clusters: " << info->FreeClusters << endl;
    cout << "5. Number of reserved clusters: " << info->TotalReserved << endl;
    cout << "6. Number of bytes in sector: " << info->BytesPerSector << endl;
    cout << "7. Number of bytes in cluster: " << info->BytesPerCluster << endl;
    cout << "8. Number of bytes in a file record segment: " << info->BytesPerFileRecordSegment << endl;
    cout << "9. Number of clusters in a file record segment: " << info->ClustersPerFileRecordSegment << endl;
    cout << "10. Length of master file table: " << info->MftValidDataLength << "bytes" << endl;
    cout << "11. The starting logical cluster number of the master file table: "<< info->MftStartLcn << endl;
    cout << "12. The starting logical cluster number of the master file table mirror: " << info->Mft2StartLcn << endl;
    cout << "13. The starting logical cluster number of the master file table zone: " << info->MftZoneStart << endl;
    cout << "14. The ending logical cluster number of the master file table zone: " << info->MftZoneEnd << endl;
    
    cout << "\nPress any key..." << endl;
    _getch();
}