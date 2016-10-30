#pragma once

#include <Windows.h>

namespace DotInject {

	public ref class InjectorException : System::Exception {
	public:
		InjectorException(System::String^ message, DWORD SystemErrorCode);
		InjectorException(System::String^ message) : InjectorException(message, 0) {};

		/// <value>The error code returned by GetLastError. See System Error Codes on MSDN.</value>
		property DWORD SystemErrorCode;

	};
}
