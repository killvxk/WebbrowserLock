using System;
using VMProtect.KeyGen;

namespace Usage
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
				string data = @""; // put the exported data here
                Generator g = new Generator(data);
                g.UserName = "John Doe";
                g.EMail = "john@doe.com";
                g.ExpirationDate = DateTime.Now.AddMonths(1);
                g.MaxBuildDate = DateTime.Now.AddYears(1);
                g.RunningTimeLimit = 15;
				g.HardwareID = "AQIDBAgHBgU=";
                g.UserData = new byte[] { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
                string serial = g.Generate();
                Console.WriteLine("Serial number:\n{0}\n", serial);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: {0}", ex);
            }
        }
    }
}
