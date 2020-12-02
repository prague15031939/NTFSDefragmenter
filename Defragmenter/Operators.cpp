#include "Operators.h"

ostream& operator<<(ostream& out, const CString& str)
{
	if (str.GetLength() > 0)
	{
		out << CStringA(str).GetString();
	}

	return out;
}

ostream& operator<<(ostream& out, const LARGE_INTEGER& value)
{
	out << value.QuadPart;

	return out;
}