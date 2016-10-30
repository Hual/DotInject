#include "InjectorException.h"

DotInject::InjectorException::InjectorException(System::String^ message, DWORD systemErrorCode) : Exception(message)
{
	SystemErrorCode = systemErrorCode;
}
