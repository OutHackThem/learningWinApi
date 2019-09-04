#include<Windows.h>
#include<tchar.h>
#include<WtsApi32.h>
#include<iostream>
#include<sddl.h>


#pragma comment(lib, "wtsapi32")
#pragma	comment(lib, "Advapi32")



BOOL enableDebugPriv();

int main(void)
{
	
	enableDebugPriv(); //Enabling Shutdown privileges


	DWORD	level = 1; // As we want a  WTS_PROCESS_INFO_EX structure
	PWTS_PROCESS_INFO_EX processListing = NULL;
	DWORD	processCount = 0;

	if (!WTSEnumerateProcessesEx(
		WTS_CURRENT_SERVER_HANDLE,
		&level,
		WTS_ANY_SESSION,
		(LPTSTR *)&processListing,
		&processCount))
	{

		_tprintf(_T("Error with the  enumerateProcessEx call"));
	}

	_tprintf(_T("Total Processes : [%d]"), processCount);

	_tprintf(_T("\nCount\tPid\tprocessId\tpOwner\tName\n"));

	LPTSTR suid = NULL;

	TCHAR acName[1024];
	DWORD ccName = 1024;
	TCHAR domName[1024];
	DWORD refDomName = 1024;
	SID_NAME_USE pUse;


	for (int i = 0; i < processCount; i++)
	{

		_tprintf(_T("%d\t"), i);
		_tprintf(_T("%d\t"), processListing->ProcessId);
		if (!ConvertSidToStringSid(
			processListing->pUserSid,
			&suid
		))
		{
			_tprintf(_T("-\t"));
		}
		else
		{
			_tprintf(_T("%s\t"), suid);
		}
		if (!LookupAccountSid(
			NULL,
			processListing->pUserSid,
			acName,
			&ccName,
			domName,
			&refDomName,
			&pUse
		)) {
			_tprintf(_T("-\t"));
		}
		else
		{
			_tprintf(_T("%s\\%s\t"), domName, acName);
		}

		_tprintf(_T("%s\t\n"), processListing->pProcessName);
		processListing++;
	}



	system("pause");
}



BOOL enableDebugPriv()
{

	LUID lpointer;

	//Get priv token 
	if (!LookupPrivilegeValue(
		NULL,
		_T("SeDebugPrivilege"),
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