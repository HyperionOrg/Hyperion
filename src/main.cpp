#include "Hyperion/HyperionServer.h"

using namespace Hyperion;

int main()
{
	HyperionServer* hyperionServer = new HyperionServer();
	hyperionServer->Run();
	delete hyperionServer;
	return 0;
}
