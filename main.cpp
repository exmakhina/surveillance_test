#include <iostream>
#include <string>
#include <jsoncpp/json.h>
#include "udpsocket.h"
#include "tcpsocket.h"
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
					TcpSocket tcpSocket;
					string IP = listenerIP[to_string(i)].asString();
					int port = message["ListenerPort"].asInt();

					cout << "Try to connect to  " << IP << ":" << port << "\n";

					try {
						tcpSocket.connect(IP, port);
					}
					catch (SocketException& e) {
						cout << e.description();
						continue;
					}

					cout << "Connected to " << IP << "Port: " << port << "\n";
				}
			}
		}
	}

	return 0;
}
