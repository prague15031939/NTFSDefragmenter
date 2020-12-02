#pragma once
#include <iostream>
#include <atlstr.h>

using namespace std;


ostream& operator<<(ostream& out, const CString& str);

ostream& operator<<(ostream& out, const LARGE_INTEGER& value);