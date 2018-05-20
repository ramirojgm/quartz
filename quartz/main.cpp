#include "quartz.h"
#include <iostream>

using namespace quartz;

int main (int argc, char *argv[])
{
	http::socketlistener listener(8080);
	while(true)
	{
		http::socket * client = listener.accept();
		std::cout<<"request:"<<client->recv()<<std::endl;
		client->send("http 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\nRamiro Jóse García Moraga");
		delete client;
	}
	return 0;
}

