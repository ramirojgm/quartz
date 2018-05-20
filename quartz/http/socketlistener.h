#ifndef _QUARTZ_SOCKET_LISTENER_H_
#define _QUARTZ_SOCKET_LISTENER_H_

namespace quartz
{
	namespace http
	{
		class socketlistener 
		{
		private:
			int fd;

		public:
			socketlistener(int inet_port);

			socket *	accept();

			virtual 	~socketlistener();
		};
	}
}

#endif