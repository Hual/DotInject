# DotInject
A .NET library for injecting libraries into processes, using the CreateRemoteThread method.

## Purpose
The purpose of this library is to remove the need of P/Invoke (which is quite ugly mixed with .NET code) and add better library injection support for .NET, with more advanced error handling.

## Classes
  * **RuntimeInjector** - an injector which takes an already-started process and injects libraries into it
  * **SuspendingInjector** - an injector which spawns a new process, injects libraries into it, and then starts it
  
## Exceptions
  * **InjectorException** - Gets thrown when an error has occurred while injecting into a process. Has a Message property and a SystemErrorCode property which returns the [System Error Code](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) returned while injecting
  
## Methods
  * **InjectLibrary** - available in *RuntimeInjector* and *SuspendingInjector*. Injects a library into a process
  * **InjectLibraries** - available in *RuntimeInjector* and *SuspendingInjector*. Injects a list of libraries into a process
  * **Start** - available in *SuspendingInjector*. Starts the process and finalizes the injector.

## Examples
  * [**Nodepad injection**](https://github.com/Hual/DotInject/blob/master/DotInject.Example.Executable/Program.cs) - The goal of this example is to show how basic injection using the library works. It injects a custom dynamic-link library into Notepad and a message saying "Hello, world!" pops up from inside Notepad.
