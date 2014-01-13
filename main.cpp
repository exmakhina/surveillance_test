#include <iostream>
#include <string>
#include <jsoncpp/json.h>
#include "udpsocket.h"
#include "tcpsocket.h"
#include "socketexception.h"

using namespace std;

int main(int argc, char* argv[])
{
	UdpSocket udpSocket;
	string advertisment;
	Json::Reader reader;
	Json::Value message;
	Json::Value request, response;
	string requestString, responseString;
	string command;

	if (argc < 2) {
		cout << "Usage:\n";
		cout << "surveillance_test start|stop|kill\n";
	} else	{
		/* Check the input parameter and create the request */
		request["Request"]["Sender"] = "host";
		command = argv[1];
		if (command.compare("start") == 0) {
			request["Request"]["Command"] = 0;
			request["Request"]["Value"] = 0;
		} else if (command.compare("stop") == 0) {
			request["Request"]["Command"] = 1;
			request["Request"]["Value"] = 0;
		} else if (command.compare("kill") == 0) {
			request["Request"]["Command"] = 2;
			request["Request"]["Value"] = 0;
		} else {
			cout << "Usage:\n";
			cout << "surveillance_test start|stop|kill\n";
			return -1;
		}
		requestString = request.toStyledString();

		/* Wait for the advertisment message */
		try {
			udpSocket >> advertisment;
		}
		catch (SocketException& e) {
			cout << e.description();
		}
		udpSocket.close();	// Socket not needed any more.

		/* Parse it to know where to connect to */
		if (reader.parse(advertisment, message)) {
			/* Grab the device's name from the advertisement message */
			/* and fill the "Recipient" field of the request */
			if (message.isMember("DeviceName")) {
				cout << "Received advertisement message from: " << message["DeviceName"].toStyledString();
				request["Request"]["Recipient"] = message["DeviceName"].asString();
			}

			/* Try to connect to one of the IP provided by the camera */
			if (message.isMember("ListenerIP") && message.isMember("ListenerPort")) {
				Json::Value listenerIP(message["ListenerIP"]);		// extract the IP list from the advertisment message
				for (int i=0; listenerIP.isMember(to_string(i)); i++) {
					TcpSocket tcpSocket;		// create a socket
					string IP = listenerIP[to_string(i)].asString();
					int port = message["ListenerPort"].asInt();

					cout << "Try to connect to " << IP << ":" << port << "\n";
					try {
						tcpSocket.connect(IP, port);
					}
					catch (SocketException& e) {
						cout << e.description();
						continue;		// if connection fails, try the next IP
					}
					cout << "Connected to " << IP << ":" << port << "\n";

					/* send the request to the TCP port */
					try {
						tcpSocket << requestString;
					}
					catch (SocketException& e) {
						tcpSocket.close();
						cout << e.description();
						continue;
					}

					/* Wait for an answer */
					try {
						tcpSocket >> responseString;
					}
					catch (SocketException& e) {
						tcpSocket.close();
						cout << e.description();
						continue;
					}

					/* Parse the answer */
					if (reader.parse(responseString, response)) {
						cout << message["DeviceName"].asString() << " return status: " << response["Response"]["Status"].toStyledString();
						tcpSocket.close();
						break;	// end of communication. Exit the loop, do not wait for a connection from another socket.
					}

					/* Close the socket */
					tcpSocket.close();
				}	/* for (int i=0; listenerIP.isMember(to_string(i)); i++) */
			}
		}
	}

	return 0;
}
