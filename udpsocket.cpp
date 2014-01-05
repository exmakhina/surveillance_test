#include <string>
#include <cstring>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "settings.h"
#include "udpsocket.h"
#include "socketexception.h"

using namespace std;

UdpSocket::UdpSocket()
{
	sockHandle = ::socket(AF_INET,SOCK_DGRAM,0);	// UDP socket, Internet domain
	if (sockHandle < 0)
		throw SocketException("Error creating the UDP socket\n");

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	sockAddr.sin_port=htons(Settings::instance().getBroadcastPort());
	if (::bind(sockHandle, (sockaddr *)&sockAddr, sizeof(sockaddr)) < 0)
		throw SocketException("Unable to bind the socket the the IP:PORT.\n");
}

UdpSocket::~UdpSocket()
{
	if (sockHandle >= 0)
		::close(sockHandle);
}

const UdpSocket& UdpSocket::operator >> (string& message) const
{
	int sent;
	char buf[1024];
	int size;

	if (sockHandle >= 0) {
		size = ::recv(sockHandle, buf, 1024, 0);
	} else
		throw SocketException("Socket not created.\n");

	if (size < 0)
		throw SocketException("Error receiving UDP packet.\n");

	message = buf;

	return *this;
}
