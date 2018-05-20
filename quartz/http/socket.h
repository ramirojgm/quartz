#ifndef _QUARTZ_SOCKET_H_
#define _QUARTZ_SOCKET_H_

namespace quartz
{
	namespace http
	{
		const int MAX_LINE_LENGTH = 1024;

		class socket
		{
		private:
			int fd;

		public:
			socket(int fd);

			void 		send(const std::string & str);

			void		send(const void * data, int length);

			std::string	recv(int length);

			std::string recv();

			void 		recv(void * data,int length);

			int 		recv(void * data);

			virtual 	~socket();
		};
	}

}

#endif