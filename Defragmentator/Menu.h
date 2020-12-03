#pragma once
#include <iostream>
#include <atlstr.h>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <vector>

using namespace std;

struct DriveData ////////////This is how your api for Menu should look like!!!!!
{
	wchar_t Drive[10];
	wchar_t Drive_Type[20];
	wchar_t Sector_Per_Cluster[5];
	wchar_t Bytes_Per_Sector[5];
	wchar_t Free_Space[5];
	wchar_t Full_Space[5];
};

extern std::vector<DriveData*> DriveList; // If you can t get all drive data for structure, leave fields empty, but mention field Drive any way!!!!!!!

void StartWindow();
void outputDrives();
void outputTable();
