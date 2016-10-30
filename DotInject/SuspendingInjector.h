#pragma once

#include "RuntimeInjector.h"

namespace DotInject {

	public ref class SuspendingInjector : public RuntimeInjector {
	public:
		/// <summary>
		/// Creates a new instance of the injector. Starts a suspended process with command-line arguments and a custom working directory and prepares it for library injection on startup.
		/// <para>
		/// <list type="bullet">
		/// <item><description>The <paramref name="path"/> parameter takes a string containing the path to the executable to create the process from.</description></item>
		/// <item><description>The <paramref name="path"/> parameter takes a string containing the path to the working directory.</description></item>
		/// <item><description>The <paramref name="arguments"/> parameter takes a string containing the command-line arguments to start the process with.</description></item>
		/// </list>
		/// </para>
		/// <para>
		/// Throws a <see cref="DotInject::InjectorException" /> exception when the process is of a different architecture than the injecting process.
		/// </para>
		/// </summary>
		/// <param name="path">The path to the executable to create the process from.</param>
		/// <param name="workingDirectory">The path to the working directory.</param>
		/// <param name="arguments">The command-line arguments to start the process with.</param>
		/// <exception cref="DotInject::InjectorException">Thrown when the process is of a different architecture than the injecting process.</exception>
		SuspendingInjector(System::String^ path, System::String^ workingDirectory, System::String^ arguments);

		/// <summary>
		/// Creates a new instance of the injector. Starts a suspended process with command-line arguments and prepares it for library injection on startup.
		/// <para>
		/// <list type="bullet">
		/// <item><description>The <paramref name="path"/> parameter takes a string containing the path to the executable to create the process from.</description></item>
		/// <item><description>The <paramref name="arguments"/> parameter takes a string containing the command-line arguments to start the process with.</description></item>
		/// </list>
		/// </para>
		/// <para>
		/// Throws a <see cref="DotInject::InjectorException" /> exception when the process is of a different architecture than the injecting process.
		/// </para>
		/// </summary>
		/// <param name="path">The path to the executable to create the process from.</param>
		/// <param name="arguments">The command-line arguments to start the process with.</param>
		/// <exception cref="DotInject::InjectorException">Thrown when the process is of a different architecture than the injecting process.</exception>
		SuspendingInjector(System::String^ path, System::String^ arguments) : SuspendingInjector(path, System::IO::Path::GetDirectoryName(path), arguments) {};

		/// <summary>
		/// Creates a new instance of the injector. Starts a suspended process without command-line arguments and prepares it for library injection on startup.
		/// <para>
		/// The <paramref name="path"/> parameter takes a string containing the path to the executable to create the process from.
		/// </para>
		/// <para>
		/// Throws a <see cref="DotInject::InjectorException" /> exception when the process is of a different architecture than the injecting process.
		/// </para>
		/// </summary>
		/// <param name="path">The path to the executable to create the process from.</param>
		/// <exception cref="DotInject::InjectorException">Thrown when the process is of a different architecture than the injecting process.</exception>
		SuspendingInjector(System::String^ path) : SuspendingInjector(path, System::String::Empty) {};

		~SuspendingInjector();
		!SuspendingInjector();

		/// <summary>
		/// Starts the process and finalizes the injector.
		/// <para>
		/// Throws a <see cref="DotInject::InjectorException" /> exception when an error occurs while starting the process.
		/// </para>
		/// <para>Returns a boolean showing whether the process has been started or requires a restart.</para>
		/// </summary>
		/// <exception cref="DotInject::InjectorException">Thrown when an error occurs while starting the process.</exception>
		/// <returns>A boolean showing whether the process has been started or requires a restart.</returns>
		bool Start();

		/// <value>Has the process been started. Read-only.</value>
		property bool Started {
			bool get() {
				return started;
			}
		}

	private:
		HANDLE mainThreadHandle;
		bool started;

	};
}
