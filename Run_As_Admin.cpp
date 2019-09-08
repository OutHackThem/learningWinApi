#include<Windows.h>
#include<tchar.h>
#include<WtsApi32.h>
#include<strsafe.h>
#include<sddl.h>


#pragma comment(lib, "wtsapi32")
#pragma	comment(lib, "Advapi32")

BOOL checkAdminPriv();
void reLaunchasAdmin();


/*This program is pretty simple actually in the main() there are two checks checkAminPriv() and reLaunchasAdmin() 
Now what happens is that
We first get a handle to the current process and check its privileges and get a LUID to all of them then we get a LUID to the seDebugPriv which is only given to Administrators we loop through the LUID of every priv the current process has and match it with the LUID of seDebugPriv if we get a hit then we know that current process is running as an Administrator simple ain't it ?
*/



void main()
{

	if (!checkAdminPriv())
	{

		reLaunchasAdmin();
	}
	else
	{
		_tprintf("\nADMINISTRATOR\n\t");
		system("whoami /priv");
	}

	system("pause");
}

void reLaunchasAdmin()
{

	// get current file name 
	CHAR lpFile[1024];
	DWORD lpsize = 1024;

	GetModuleFileNameA(
		NULL,
		lpFile,
		lpsize
	);

	SHELLEXECUTEINFO pexInfo;
	
	pexInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	pexInfo.fMask = SEE_MASK_DEFAULT;
	pexInfo.hwnd = NULL;
	pexInfo.lpVerb = _T("runas");
	pexInfo.lpFile = lpFile;
	pexInfo.lpParameters = NULL;
	pexInfo.lpDirectory = NULL;
	pexInfo.nShow = SW_SHOWNORMAL;

	

	if (!ShellExecuteEx(
		&pexInfo
	))
	{
		_tprintf("Sehllexecex not working");
	}
}

BOOL checkAdminPriv()
{
	//get luid of seDebugPriv
	LUID debugPrivLuid;
	if (!LookupPrivilegeValue(
		NULL,
		_T("SeDebugPrivilege"),
		&debugPrivLuid
	))
	{
		_tprintf("could not get priv of the se debug priv");
	}


	//Get Handle to the currnent process token

	HANDLE tokHandle;

	if (!OpenProcessToken(
		GetCurrentProcess(),
		TOKEN_QUERY,
		&tokHandle
	))
	{
		_tprintf("Open process token does not work");
	}



	//Get Toekn information
	// 
	//

	//PTOKEN_PRIVILEGES tokInformation;

	//tokInformation = (PTOKEN_PRIVILEGES*)malloc(tokInformation);

	DWORD tokLength;
	GetTokenInformation(
		tokHandle,
		TokenPrivileges,
		NULL,
		0,
		&tokLength
	);


	PTOKEN_PRIVILEGES tokInformation;
	DWORD tokLength1;
	tokInformation = (PTOKEN_PRIVILEGES)malloc(tokLength);

	if (!GetTokenInformation(
		tokHandle,
		TokenPrivileges,
		tokInformation,
		tokLength,
		&tokLength1
	))
	{
		_tprintf("GetTokenInformation didn't work could not Content");
	}

	PLUID_AND_ATTRIBUTES runner;

	BOOL adminRunning = FALSE;

	for (int i = 0; i <= tokInformation->PrivilegeCount; i++)
	{
		runner = &tokInformation->Privileges[i];
		if ((runner->Luid.HighPart == debugPrivLuid.HighPart) && (runner->Luid.LowPart == debugPrivLuid.LowPart))
		{
			adminRunning = TRUE;
		}
	}
	if (adminRunning == TRUE)
	{
		return TRUE;
	}
	else {
		return FALSE;
	}

}