#include <iostream>
#include <string>
#include "udpsocket.h"
#include "socketexception.h"

using namespace std;

int main()
{
	UdpSocket udpSocket;
	string advertisment;
	bool error = false;

	while (!error) {
		try {
			udpSocket >> advertisment;
		}
		catch (SocketException& e) {
			cout << e.description();
			error = true;
		}
		cout << "Received: " << advertisment << "\n";
	}

	return 0;
}
