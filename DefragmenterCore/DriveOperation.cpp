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
    wss.str().copy(item->Drive, 30, 0);
    wss.str(std::wstring());

    wss << info->VolumeSerialNumber.QuadPart;
    wss.str().copy(item->VolumeSerialNumber, 30, 0);
    wss.str(std::wstring());

    wss << info->NumberSectors.QuadPart;
    wss.str().copy(item->NumberSectors, 30, 0);
    wss.str(std::wstring());

    wss << info->TotalClusters.QuadPart;
    wss.str().copy(item->TotalClusters, 30, 0);
    wss.str(std::wstring());

    wss << info->FreeClusters.QuadPart;
    wss.str().copy(item->FreeClusters, 30, 0);
    wss.str(std::wstring());

    wss << info->TotalReserved.QuadPart;
    wss.str().copy(item->TotalReserved, 30, 0);
    wss.str(std::wstring());

    wss << info->BytesPerSector;
    wss.str().copy(item->BytesPerSector, 30, 0);
    wss.str(std::wstring());

    wss << info->BytesPerCluster;
    wss.str().copy(item->BytesPerCluster, 30, 0);
    wss.str(std::wstring());

    wss << info->BytesPerFileRecordSegment;
    wss.str().copy(item->BytesPerFileRecordSegment, 30, 0);
    wss.str(std::wstring());

    wss << info->ClustersPerFileRecordSegment;
    wss.str().copy(item->ClustersPerFileRecordSegment, 30, 0);
    wss.str(std::wstring());

    wss << info->MftValidDataLength.QuadPart;
    wss.str().copy(item->MftValidDataLength, 30, 0);
    wss.str(std::wstring());

    wss << info->MftStartLcn.QuadPart;
    wss.str().copy(item->MftStartLcn, 30, 0);
    wss.str(std::wstring());

    wss << info->Mft2StartLcn.QuadPart;
    wss.str().copy(item->Mft2StartLcn, 30, 0);
    wss.str(std::wstring());

    wss << info->MftZoneStart.QuadPart;
    wss.str().copy(item->MftZoneStart, 30, 0);
    wss.str(std::wstring());

    wss << info->MftZoneEnd.QuadPart;
    wss.str().copy(item->MftZoneEnd, 30, 0);
    wss.str(std::wstring());

    return item;
}