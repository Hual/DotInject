#pragma once

#include "InjectorException.h"

namespace DotInject {

	public ref class RuntimeInjector {
	public:
		/// <summary>
		/// Creates a new instance of RuntimeInjector to inject libraries into a process with a custom working directory.
		/// <para>
		/// <list type="bullet">
		/// <item><description>The <paramref name="process"/> parameter takes the process to inject libraries into.</description></item>
		/// <item><description>The <paramref name="workingDirectory"/> parameter takes a string containing the process' working directory path.</description></item>
		/// </list>
		/// </para>
		/// <para>
		/// Throws a <see cref="DotInject::InjectorException" /> exception when the process is of a different architecture than the injecting process.
		/// </para>
		/// </summary>
		/// <param name="process">The process to inject libraries into.</param>
		/// <param name="workingDirectory">The working directory path of the process, used for loading libraries from relative paths.</param>
		/// <exception cref="DotInject::InjectorException">Thrown when the process is of a different architecture than the injecting process.</exception>
		RuntimeInjector(System::Diagnostics::Process^ process, System::String^ workingDirectory);

		/// <summary>
		/// Creates a new instance of RuntimeInjector to inject libraries into a process with a default working directory.
		/// <para>The <paramref name="process"/> parameter takes the process to inject libraries into.
		/// </para>
		/// <para>
		/// Throws a <see cref="DotInject::InjectorException" /> exception when the process is of a different architecture than the injecting process.
		/// </para>
		/// </summary>
		/// <param name="process">The process to inject libraries into.</param>
		/// <exception cref="DotInject::InjectorException">Thrown when the process is of a different architecture than the injecting process.</exception>
		RuntimeInjector(System::Diagnostics::Process^ process) : RuntimeInjector(process, System::IO::Path::GetDirectoryName(process->MainModule->FileName)) {};

		/// <summary>
		/// Injects a library into the process.
		/// <para>The <paramref name="libraryPath"/> parameter takes a string containing the path to the library file.</para>
		/// <para>
		/// Throws a <see cref="DotInject::InjectorException" /> exception when an error occurs while injecting a library into the process.
		/// Throws a <see cref="System::IO::FileNotFoundException" /> exception when the library file was not found.
		/// </para>
		/// </summary>
		/// <param name="libraryPath">The path to the library file. Can be absolute or relative.</param>
		/// <exception cref="System::IO::FileNotFoundException">Thrown when the library file was not found.</exception>
		/// <exception cref="DotInject::InjectorException">Thrown when an error occurs while injecting the library into the process.</exception>
		/// <returns>No value is returned.</returns>
		void InjectLibrary(System::String^ libraryPath);

		/// <summary>
		/// Injects a list of libraries into the process.
		/// <para>The <paramref name="libraryPaths"/> parameter takes a variable parameter list of strings containing the paths to the library files.</para>
		/// <para>
		/// Throws a <see cref="DotInject::InjectorException" /> exception when an error occurs while injecting a library into the process.
		/// Throws a <see cref="System::IO::FileNotFoundException" /> exception when the library file was not found.
		/// </para>
		/// </summary>
		/// <param name="libraryPaths">The paths to the library files. Can be absolute or relative.</param>
		/// <exception cref="System::IO::FileNotFoundException">Thrown when a library file was not found.</exception>
		/// <exception cref="DotInject::InjectorException">Thrown when an error occurs while injecting a library into the process.</exception>
		/// <returns>No value is returned.</returns>
		void InjectLibraries(... array<System::String^>^ libraryPaths);
		
		/// <value>The process that is injected into. Read-only.</value>
		property System::Diagnostics::Process^ Process {
			System::Diagnostics::Process^ get() {
				return process;
			}
		}

		/// <value>The working directory, used for relative library paths. Defaults to the process executable's directory. Read-only.</value>
		property System::String^ WorkingDirectory {
			System::String^ get() {
				return workingDirectory;
			}
		}

	protected:
		RuntimeInjector(System::String^ workingDirectory);

		static bool Is64BitProcess(HANDLE processHandle);
		bool Is64BitProcess();

		System::Diagnostics::Process^ process;

	private:

		System::String^ workingDirectory;

	};
}
