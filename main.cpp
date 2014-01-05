#include <iostream>
#include <string>
#include <jsoncpp/json.h>
#include "udpsocket.h"
#include "socketexception.h"

using namespace std;

int main()
{
	UdpSocket udpSocket;
	string advertisment;
	bool error = false;
	Json::Reader reader;
	Json::Value message;

	while (!error) {
		try {
			udpSocket >> advertisment;
		}
		catch (SocketException& e) {
			cout << e.description();
			error = true;
		}

		if (reader.parse(advertisment, message)) {
			if (message.isMember("DeviceName"))
				cout << "Device name: " << message["DeviceName"].toStyledString() << "\n";
			if (message.isMember("ListenerIP") && message.isMember("ListenerPort")) {
				Json::Value listenerIP(message["ListenerIP"]);
				for (int i=0; listenerIP.isMember(to_string(i)); i++) {
					cout << "IP address " << i << ": " << listenerIP[to_string(i)] << "Port: " << message["ListenerPort"] << "\n";
				}
			}
		}
	}

	return 0;
}
