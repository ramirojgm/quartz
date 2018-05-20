#ifndef _QUARTZ_PACKAGE_H_
#define _QUARTZ_PACKAGE_H_

namespace quartz
{
	namespace http
	{
		class package
		{
		private:
			std::map<std::string, std::string> attrs;			

		protected:
			package();

			void			read_package(socket * socket);

			void			write_package(socket * socket);

		public:
			void			clear();

			bool			has(const std::string & name);

			int				get_int(const std::string & name);

			void			set_int(const std::string & name,int value);

			double			get_double(const std::string & name);

			void			set_double(const std::string & name,double value);

			std::string 	get_string(const std::string & name);

			void			set_string(const std::string & name,const std::string & value);

			virtual 		~package();
		};
	}

}

#endif