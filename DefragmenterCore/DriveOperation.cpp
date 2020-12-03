#include "pch.h"
#include "Core.h"
#include <winioctl.h>
#include <sstream>

DriveInfo* formDriveInfo(PNTFS_VOLUME_DATA_BUFFER info, char& drive);

DriveInfo* __cdecl getDriveInfo(char& drive)
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

    __try {
        if (DeviceIoControl(hDrive,
            FSCTL_GET_NTFS_VOLUME_DATA,
            NULL,
            0,
            info,
            sizeof(NTFS_VOLUME_DATA_BUFFER),
            &cbWritten,
            NULL))
        {
            return formDriveInfo(info, drive);
        }
        else
            return nullptr;
    }
    __finally {
        CloseHandle(hDrive);
        free(info);
    }
}

DriveInfo* formDriveInfo(PNTFS_VOLUME_DATA_BUFFER info, char& drive)
{
    DriveInfo* item = new DriveInfo();
    std::wostringstream wss;

    wss << drive << ":\\";
    wss.str().copy(item->Drive, 10, 0);
    wss.clear();

    wss << info->VolumeSerialNumber.QuadPart;
    wss.str().copy(item->VolumeSerialNumber, 10, 0);
    wss.clear();

    wss << info->NumberSectors.QuadPart;
    wss.str().copy(item->NumberSectors, 10, 0);
    wss.clear();

    wss << info->TotalClusters.QuadPart;
    wss.str().copy(item->TotalClusters, 10, 0);
    wss.clear();

    wss << info->FreeClusters.QuadPart;
    wss.str().copy(item->FreeClusters, 10, 0);
    wss.clear();

    wss << info->TotalReserved.QuadPart;
    wss.str().copy(item->TotalReserved, 10, 0);
    wss.clear();

    wss << info->BytesPerSector;
    wss.str().copy(item->BytesPerSector, 10, 0);
    wss.clear();

    wss << info->BytesPerCluster;
    wss.str().copy(item->BytesPerCluster, 10, 0);
    wss.clear();

    wss << info->BytesPerFileRecordSegment;
    wss.str().copy(item->BytesPerFileRecordSegment, 10, 0);
    wss.clear();

    wss << info->ClustersPerFileRecordSegment;
    wss.str().copy(item->ClustersPerFileRecordSegment, 10, 0);
    wss.clear();

    wss << info->MftValidDataLength.QuadPart;
    wss.str().copy(item->MftValidDataLength, 10, 0);
    wss.clear();

    wss << info->MftStartLcn.QuadPart;
    wss.str().copy(item->MftStartLcn, 10, 0);
    wss.clear();

    wss << info->Mft2StartLcn.QuadPart;
    wss.str().copy(item->Mft2StartLcn, 10, 0);
    wss.clear();

    wss << info->MftZoneStart.QuadPart;
    wss.str().copy(item->MftZoneStart, 10, 0);
    wss.clear();

    wss << info->MftZoneEnd.QuadPart;
    wss.str().copy(item->MftZoneEnd, 10, 0);
    wss.clear();

    return item;
}


