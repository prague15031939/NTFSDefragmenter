#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <fstream>
#include <atlstr.h>
#include "Operators.h"

using namespace std;


int WorkIn(CString directory, CString dr, bool first);

VOLUME_BITMAP_BUFFER* readVolumeBitmap(LPCWSTR drive);

RETRIEVAL_POINTERS_BUFFER* readFileBitmap(wstring fileName);

int Move(LPCWSTR lpSrcName, LPCWSTR drive);