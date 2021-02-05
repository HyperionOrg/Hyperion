#include "Hyperion/HyperionServer.h"

int main()
{
	Hyperion::HyperionServer* hyperionServer = new Hyperion::HyperionServer();
	hyperionServer->Run();
	delete hyperionServer;
	return 0;
}