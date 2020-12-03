#include "CheckUser.h"

BOOL IsUserAdmin()
{
	BOOL flag;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorGroup;

	flag = AllocateAndInitializeSid(&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0,
		0,
		0,
		0,
		0,
		0,
		&AdministratorGroup);

	if (flag)
	{
		if (!CheckTokenMembership(NULL,
			AdministratorGroup,
			&flag))
		{
			flag = FALSE;
		}

		FreeSid(AdministratorGroup);
	}

	return flag;
}

void AdminErrorOutput()
{
	cout << "Sorry,but you not an administrator!!!" << endl;
	cout << "Press any key to complete work...";

	_getch();

	system("CLS");
}