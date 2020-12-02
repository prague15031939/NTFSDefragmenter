#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;


void selectDrive(char& drive);

void getDriveInfo(char& drive);

void outputDriveInfo(PNTFS_VOLUME_DATA_BUFFER info, char& drive);
