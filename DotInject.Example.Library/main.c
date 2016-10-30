#include <Windows.h>

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	MessageBox(NULL, L"Hello, world!", L"DotInject Example", MB_OK);
}

BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved)
{
	if (nReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hDllHandle);

		CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
	}

	return TRUE;
}
