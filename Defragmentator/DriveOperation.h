#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <winioctl.h>

using namespace std;

struct DriveInfo ////////////This is how your api for DriveOperation should look like!!!!!
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

extern DriveInfo* driveInfo; // Return NULL if mistake, I will show MessageBox with error

void selectDrive(char& drive);

void getDriveInfo(char& drive);

void outputDriveInfo(PNTFS_VOLUME_DATA_BUFFER info, char& drive);
