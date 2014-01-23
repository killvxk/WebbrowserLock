
#include <windows.h>
#include <tchar.h>

int main()
{
	FARPROC proc;

	proc = GetProcAddress(LoadLibrary(_T("System.Server.Communication.dll")),"CommunicationStart");

	proc();
	while (true)
	{
		Sleep(10000);
	}
}



