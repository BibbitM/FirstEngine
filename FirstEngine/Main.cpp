#include "PlatformDefinitions.h"
#include "Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdLine);

	Application application( hInstance, nCmdShow );

	int ret = application.Run( 1280, 720 );

	return ret;
}
