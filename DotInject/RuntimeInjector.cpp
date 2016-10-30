#include <Windows.h>
#include <msclr/marshal_cppstd.h>
#include <msclr/marshal_windows.h>

#include "RuntimeInjector.h"
#include "InjectorException.h"

DotInject::RuntimeInjector::RuntimeInjector(System::Diagnostics::Process^ process, System::String^ workingDirectory) : RuntimeInjector(workingDirectory)
{
	this->process = process;

	if (System::Environment::Is64BitProcess != Is64BitProcess()) {
		throw gcnew InjectorException("Process architectures differ");
	}
}

DotInject::RuntimeInjector::RuntimeInjector(System::String^ workingDirectory)
{
	this->workingDirectory = workingDirectory;
}

void DotInject::RuntimeInjector::InjectLibrary(System::String^ libraryPath)
{
	std::wstring libraryPathNative = msclr::interop::marshal_as<std::wstring>(libraryPath);
	size_t libraryPathSize = (libraryPathNative.length() + 1) * sizeof(LPWSTR);

	if (!System::IO::File::Exists(libraryPath)) {
		if (!System::IO::Path::IsPathRooted(libraryPath)) {
			if (!System::IO::File::Exists(System::IO::Path::Combine(WorkingDirectory, libraryPath))) {
				throw gcnew System::IO::FileNotFoundException("Library does not exist", libraryPath);
			}
		}
		else {
			throw gcnew System::IO::FileNotFoundException("Library does not exist", libraryPath);
		}
	}

	if (Process->HasExited) {
		throw gcnew InjectorException("Attempted injection on exited process");
	}
	
	HANDLE processHandle = msclr::interop::marshal_as<HANDLE>(Process->Handle);

	LPVOID remoteAllocatedMemory = VirtualAllocEx(processHandle, NULL, libraryPathSize, MEM_COMMIT, PAGE_READWRITE);
	if (remoteAllocatedMemory == NULL) {
		throw gcnew InjectorException("Could not allocate memory in the process' address space", GetLastError());
	}

	SIZE_T bytesWritten = 0;
	BOOL writeSuccess = WriteProcessMemory(processHandle, remoteAllocatedMemory, libraryPathNative.c_str(), libraryPathSize, &bytesWritten);
	if (!writeSuccess || bytesWritten != libraryPathSize) {
		throw gcnew InjectorException("Could not write to the allocated process memory", GetLastError());
	}

	HANDLE libraryThread = CreateRemoteThread(processHandle, NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(GetModuleHandleW(L"KERNEL32"), "LoadLibraryW")), remoteAllocatedMemory, 0, NULL);
	if (!libraryThread) {
		throw gcnew InjectorException("Could not create a remote thread in the process", GetLastError());
	}
	else {
		WaitForSingleObject(libraryThread, INFINITE);
		CloseHandle(libraryThread);
	}

	VirtualFreeEx(processHandle, remoteAllocatedMemory, sizeof(remoteAllocatedMemory), MEM_RELEASE);
}

void DotInject::RuntimeInjector::InjectLibraries(... array<System::String^>^ libraryPaths)
{
	for (int i = 0; i < libraryPaths->GetLength(0); ++i) {
		this->InjectLibrary(libraryPaths[i]);
	}
}

bool DotInject::RuntimeInjector::Is64BitProcess(HANDLE processHandle)
{
	BOOL wow64;
	BOOL success = IsWow64Process(processHandle, &wow64);

	if (!success) {
		throw gcnew InjectorException("Could not determine the architecture of the process", GetLastError());
	}

	return System::Environment::Is64BitOperatingSystem && !wow64;
}

bool DotInject::RuntimeInjector::Is64BitProcess()
{
	return Is64BitProcess(msclr::interop::marshal_as<HANDLE>(process->Handle));
}