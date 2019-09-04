#include<Windows.h>
#include<tchar.h>
#include<WtsApi32.h>
#include<iostream>
#include<sddl.h>


#pragma comment(lib, "wtsapi32")
#pragma	comment(lib, "Advapi32")

//Function to shutdown
void byebye(void);

//Function to Enable SeShutdownPrivilege
BOOL enableShutdownPriv();

int main(void)
{
	// This program is to turn off the PC by using the seShutdownPriv

	enableShutdownPriv(); //Enabling Shutdown privileges
	byebye();



	system("pause");
}


void byebye(void)
{
	//The core API call which actually shutsdown the windows
	if (!ExitWindowsEx(
		EWX_REBOOT,
		0
	))
	{
		_tprintf(L"Bybye didn't work");
	}
}

//This function comprises of 3 subfunctions 
// 1)LookupPrivilegeValue Which looks up value of the priv we want this value can be fed to other APIS
// 2)OpenProcessToken get a HANDLE to the running process token
// 3)AdjustTokenPrivileges API to adjust the priv of the token
BOOL enableShutdownPriv()
{

	LUID lpointer;

	//Get priv token 
	if (!LookupPrivilegeValue(
		NULL,
		_T("SeShutdownPrivilege"),
		&lpointer
	))
	{
		_tprintf(L"lookupPriv API problem");
	}


	HANDLE currentProcInfo = GetCurrentProcess();
	HANDLE tokhandler;

	if (!OpenProcessToken(
		currentProcInfo,
		TOKEN_ADJUST_PRIVILEGES,
		&tokhandler
	))
	{
		_tprintf(L"openProcessTok api didn't work");
	}



	TOKEN_PRIVILEGES tokPriv;

	tokPriv.PrivilegeCount = 1;
	tokPriv.Privileges[0].Luid = lpointer;
	tokPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;


	if (!AdjustTokenPrivileges(
		tokhandler,
		FALSE,
		&tokPriv,
		0,
		NULL,
		NULL
	))
	{
		_tprintf(L"Adjust token did not work");
	}

	return TRUE;
}
