#ifndef _UDPSOCKET_H_
#define _UDPSOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <vector>

class UdpSocket {
public:
	UdpSocket();
	~UdpSocket();

	const UdpSocket& operator >> (std::string&) const;
	void close();

private:
	int sockHandle;
	sockaddr_in sockAddr;
};

#endif /* _UDPSOCKET_H_ */
