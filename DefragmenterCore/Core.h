#pragma once
#include <windows.h>
#include <vector>

#ifdef DEFRAGMENTERCORE_EXPORTS
#define DEFRAGMENTERCORE_API __declspec(dllexport)
#else
#define DEFRAGMENTERCORE_API __declspec(dllimport)
#endif

// -------------------------------- menu section ------------------------------------

struct DriveData
{
	wchar_t Drive[10];
	wchar_t DriveType[20];
	wchar_t SectorPerCluster[5];
	wchar_t BytesPerSector[5];
	wchar_t FreeSpace[5];
	wchar_t FullSpace[5];
};

DEFRAGMENTERCORE_API std::vector<DriveData*> __cdecl getDrives();

// -------------------------- drive operation section ----------------------------

struct DriveInfo
{
	wchar_t Drive[10];
	wchar_t VolumeSerialNumber[10];
	wchar_t NumberSectors[10];
	wchar_t TotalClusters[10];
	wchar_t FreeClusters[10];
	wchar_t TotalReserved[10];
	wchar_t BytesPerSector[10];
	wchar_t BytesPerCluster[10];
	wchar_t BytesPerFileRecordSegment[10];
	wchar_t ClustersPerFileRecordSegment[10];
	wchar_t MftValidDataLength[10];
	wchar_t MftStartLcn[10];
	wchar_t Mft2StartLcn[10];
	wchar_t MftZoneStart[10];
	wchar_t MftZoneEnd[10];
};

DEFRAGMENTERCORE_API DriveInfo* __cdecl getDriveInfo(char& drive);

// -------------------------------- admin section --------------------------------

DEFRAGMENTERCORE_API BOOL __cdecl isUserAdmin();

