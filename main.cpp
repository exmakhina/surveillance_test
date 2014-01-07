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
	string response;

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
				cout << "Device name: " << message["DeviceName"].toStyledString();
			if (message.isMember("ListenerIP") && message.isMember("ListenerPort")) {
				/* Try to connect to any IP provided by the camera */
				Json::Value listenerIP(message["ListenerIP"]);		// extract the IP list from the advertisment message
				for (int i=0; listenerIP.isMember(to_string(i)); i++) {
					TcpSocket tcpSocket;		// create a socket
					string IP = listenerIP[to_string(i)].asString();
					int port = message["ListenerPort"].asInt();

					cout << "Try to connect to  " << IP << ":" << port << "\n";

					try {
						tcpSocket.connect(IP, port);
					}
					catch (SocketException& e) {
						cout << e.description();
						continue;		// if connection fails, try the next IP
					}

					cout << "Connected to " << IP << "Port: " << port << "\n";
					while (!error) {
						try {
							tcpSocket << "Prout Caca Boudin\n";
						}
						catch (SocketException& e) {
							cout << e.description();
							error = true;
							continue;
						}

						try {
							tcpSocket >> response;
						}
						catch (SocketException& e) {
							cout << e.description();
							error = true;
						}
					}

					tcpSocket.close();
				}
			}
		}
	}

	return 0;
}
