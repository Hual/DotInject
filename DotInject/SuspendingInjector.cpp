#define NATIVE_STRING(string) reinterpret_cast<LPWSTR>(static_cast<LPVOID>(System::Runtime::InteropServices::Marshal::StringToHGlobalUni(string)))

#include <Windows.h>

#include "SuspendingInjector.h"
#include "InjectorException.h"

DotInject::SuspendingInjector::SuspendingInjector(System::String^ path, System::String^ workingDirectory, System::String^ arguments) : RuntimeInjector(workingDirectory), started(false)
{
	if (!System::IO::File::Exists(path)) {
		throw gcnew System::IO::FileNotFoundException("Executable does not exist", path);
	}

	LPWSTR pathNative = NATIVE_STRING(path);
	LPWSTR argsNative = NATIVE_STRING(arguments);
	LPWSTR dirNative = NATIVE_STRING(WorkingDirectory);

	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);

	BOOL result = CreateProcessW(pathNative, argsNative, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_SUSPENDED, NULL, dirNative, &si, &pi);

	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr(pathNative));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr(argsNative));
	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr(dirNative));

	if (result == TRUE) {
		process = System::Diagnostics::Process::GetProcessById(pi.dwProcessId);
		mainThreadHandle = pi.hThread;
		
		if (System::Environment::Is64BitProcess != Is64BitProcess(pi.hProcess)) {
			throw gcnew InjectorException("Process architectures differ");
		}
	}
	else {
		throw gcnew InjectorException("Could not create process", GetLastError());
	}
}

DotInject::SuspendingInjector::~SuspendingInjector()
{
	this->!SuspendingInjector();
}

DotInject::SuspendingInjector::!SuspendingInjector()
{

}

bool DotInject::SuspendingInjector::Start()
{
	DWORD result = ResumeThread(mainThreadHandle);

	if (result == -1) {
		throw gcnew InjectorException("Could not start process", GetLastError());
	}

	return (started = ResumeThread(mainThreadHandle) == 1);
}
