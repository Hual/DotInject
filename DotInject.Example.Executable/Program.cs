using DotInject;

namespace DotInject.Example.Executable
{
    class Program
    {
        static void Main(string[] args)
        {
            string notepadPath = System.IO.Path.Combine(System.Environment.GetFolderPath(System.Environment.SpecialFolder.Windows), "notepad.EXE");
            string libraryPath = System.IO.Path.Combine(System.AppDomain.CurrentDomain.BaseDirectory, "DotInject.Example.Library.dll");

            try
            {
                using (SuspendingInjector injector = new SuspendingInjector(notepadPath))
                {
                    System.Console.WriteLine("Created suspended process: " + injector.Process);
                    injector.InjectLibrary(libraryPath);
                    injector.Start();
                }
            }
            catch (InjectorException ex)
            {
                System.Console.WriteLine("Exception: " + ex.Message + " | System error code: " + ex.SystemErrorCode + " | For reference see 'System Error Codes' on MSDN.");
            }
        }
    }
}
