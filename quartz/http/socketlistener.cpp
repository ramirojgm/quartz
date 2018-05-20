#include "../quartz.h"

namespace cstd
{
	#include <stdio.h>
	#include <sys/socket.h>
	#include <stdlib.h>
	#include <netinet/in.h>
	#include <string.h>
}

quartz::http::socketlistener::socketlistener(int inet_port)
{
	using namespace cstd;

    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    this->fd = cstd::socket(AF_INET, SOCK_STREAM, 0);
    if(this->fd == 0)
    	throw new std::exception();
    
    if (cstd::setsockopt(this->fd, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
    	close(this->fd);
        throw new std::exception();
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = cstd::htons( inet_port );
      
    if (cstd::bind(this->fd,(struct sockaddr*)&address,sizeof(address)) < 0)
    {
        close(this->fd);
        throw new std::exception();
    }
}

quartz::http::socketlistener::~socketlistener()
{


}

quartz::http::socket *
quartz::http::socketlistener::accept()
{
	int new_socket = 0;
 	if (cstd::listen(this->fd, 1) < 0)
        throw new std::exception();

    if ((new_socket = cstd::accept(this->fd,NULL,NULL)) < 0)
        throw new std::exception();

	return new quartz::http::socket(new_socket);
}