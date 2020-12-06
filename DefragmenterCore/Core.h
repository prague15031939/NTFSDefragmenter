#pragma once
#include <windows.h>
#include <vector>
#include <queue>
#include <atlstr.h>

#ifdef DEFRAGMENTERCORE_EXPORTS
#define DEFRAGMENTERCORE_API __declspec(dllexport)
#else
#define DEFRAGMENTERCORE_API __declspec(dllimport)
#endif

// -------------------------------- menu section ------------------------------------

struct DriveData
{
	wchar_t Drive[20];
	wchar_t DriveType[20];
	wchar_t SectorPerCluster[20];
	wchar_t BytesPerSector[20];
	wchar_t FreeSpace[20];
	wchar_t FullSpace[20];
};

DEFRAGMENTERCORE_API std::vector<DriveData*> __cdecl getDrives();

// -------------------------- drive operation section ----------------------------

struct DriveInfo
{
	wchar_t Drive[30];
	wchar_t VolumeSerialNumber[30];
	wchar_t NumberSectors[30];
	wchar_t TotalClusters[30];
	wchar_t FreeClusters[30];
	wchar_t TotalReserved[30];
	wchar_t BytesPerSector[30];
	wchar_t BytesPerCluster[30];
	wchar_t BytesPerFileRecordSegment[30];
	wchar_t ClustersPerFileRecordSegment[30];
	wchar_t MftValidDataLength[30];
	wchar_t MftStartLcn[30];
	wchar_t Mft2StartLcn[30];
	wchar_t MftZoneStart[30];
	wchar_t MftZoneEnd[30];
};

DEFRAGMENTERCORE_API DriveInfo* __cdecl getDriveInfo(char& drive);

// -------------------------------- admin section --------------------------------

DEFRAGMENTERCORE_API BOOL __cdecl isUserAdmin();

// -------------------------------- defrager section -----------------------------

struct DefragmentationLogItem
{
	wchar_t result[2];
	wchar_t fullName[4096];
};

struct StartDefragInfo {
	CString directory;
	CString drive;
	bool first = false; // true for root directory (like c or d)
};

DEFRAGMENTERCORE_API DWORD WINAPI WorkIn(LPVOID t); // t must be of type StartDefragInfo

DEFRAGMENTERCORE_API std::queue<DefragmentationLogItem*> __cdecl getDefragmentationLogs();

DEFRAGMENTERCORE_API void __cdecl InitCS();

DEFRAGMENTERCORE_API void __cdecl DeleteCS();

DEFRAGMENTERCORE_API void __cdecl StopDefrager(HANDLE hDefragThread);

DEFRAGMENTERCORE_API std::queue<DefragmentationLogItem*> __cdecl getTestDefragmentationLogs();

DEFRAGMENTERCORE_API DWORD WINAPI Defragmentation(LPVOID t);

DEFRAGMENTERCORE_API void __cdecl InitCS();
DEFRAGMENTERCORE_API void __cdecl DeleteCS();

