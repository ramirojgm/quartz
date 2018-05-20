#include "../quartz.h"

namespace cstd {
	#include <stdio.h>
	#include <sys/socket.h>
	#include <stdlib.h>
	#include <netinet/in.h>
	#include <string.h>
}

quartz::http::socket::socket(int fd)
{
	this->fd = fd;
}

quartz::http::socket::~socket()
{
	if(this->fd){
		shutdown(this->fd,cstd::SHUT_RDWR);
		close(this->fd);
	}
}

void	
quartz::http::socket::send(const std::string & str)
{
	this->send(str.c_str(),str.size());
}

void		
quartz::http::socket::send(const void * data, int length)
{
	using namespace cstd;

	size_t sended = 0;
	size_t send_count = 0;

	while(sended < length)
	{
		send_count = cstd::send(this->fd,(void *)((long int)data + sended),length - sended,0);
		if(send_count > 0)
			sended += send_count;
		else
			throw std::exception();
	}
}

std::string 	
quartz::http::socket::recv(int length)
{
	std::string ret = "";
	void * mem = cstd::malloc(length);
	this->recv(mem,length);
	ret.assign((char*)mem,length);
	free(mem);
	return ret;
}

std::string 	
quartz::http::socket::recv()
{
	std::string ret = "";
	void * mem = cstd::malloc(MAX_LINE_LENGTH);
	int length = this->recv(mem);
	ret.assign((char*)mem,length);
	free(mem);
	return ret;
}


void 	
quartz::http::socket::recv(void * data,int length)
{
	using namespace cstd;

	size_t received = 0;
	size_t recv_count = 0;

	while(received < length)
	{
		recv_count = cstd::recv(this->fd,(void *)((long int)data + received),length - received,0);
		if(recv_count > 0)
			received += recv_count;
		else
			throw std::exception();
	}
}

int 	
quartz::http::socket::recv(void * data)
{
	using namespace cstd;

	size_t received = 0;
	size_t recv_count = 0;

	while(received < MAX_LINE_LENGTH)
	{
		recv_count = cstd::recv(this->fd,(void *)((long int)data + received),1,0);
		if(recv_count > 0)
		{
			received += recv_count;
			if(received >= 2 && cstd::strncmp((char *)((long int)data + received - 2),"\r\n",2) == 0)
				break;
		}
		else
			throw std::exception();
	}
	return received;
}