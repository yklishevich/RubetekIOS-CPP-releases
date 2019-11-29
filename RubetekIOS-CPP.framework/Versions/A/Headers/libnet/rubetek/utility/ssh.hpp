#pragma once

#include <cassert>
#include <string>
#include <stdexcept>

#include <libssh2.h>

#ifdef SMART_HOUSE_LINUX
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/time.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <sys/types.h>
	#ifdef HAVE_SYS_SELECT_H
		#include <sys/select.h>
	#endif
#endif


namespace rubetek {
namespace libssh {

	struct global
	{
		global()
		{
			if (libssh2_init(0)) throw std::runtime_error("libssh2 initialization failed");
		}
		~global()
		{
			libssh2_exit();
		}
	};

	struct session;

	struct tcp_socket
	{
		explicit tcp_socket(std::string const& address)
			: sock_(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))
		{
			struct sockaddr_in sin;
			sin.sin_family = AF_INET;
			if (INADDR_NONE == (sin.sin_addr.s_addr = inet_addr(address.c_str()))) 
				throw std::runtime_error("libssh2: bad host address");
			sin.sin_port = htons(22);
			std::cout << "connecting\n";
			if (connect(sock_, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in))) 
				throw std::runtime_error("libssh2: failed to connect");
		}
		~tcp_socket()
		{
#ifdef SMART_HOUSE_WINDOWS
			closesocket(sock_);
#else
			close(sock_);
#endif
		}

	private:
		friend struct session;

		int sock_;
	};

	struct channel;

	struct session
	{
		session()
			: session_(libssh2_session_init())
		{
			if (!session_) throw std::runtime_error("libssh2: could not initialize SSH session");
		}
		~session()
		{
			libssh2_session_free(session_);
		}

		void handshake(tcp_socket const& sock)
		{
			if (libssh2_session_handshake(session_, sock.sock_)) throw std::runtime_error("libssh2: error when starting up SSH session");
		}

		void userauth_publickey_fromfile(std::string const& user_name, std::string const& keyfile1, std::string const& keyfile2)
		{
			if (libssh2_userauth_publickey_fromfile(session_, user_name.c_str(), keyfile1.c_str(), keyfile2.c_str(), "")) 
				throw std::runtime_error("libssh2: authentication failed");
		}

	private:
		friend struct channel;

		LIBSSH2_SESSION* session_;
	};

	struct channel
	{
		explicit channel(session const& ses)
			: channel_(libssh2_channel_open_session(ses.session_))
		{
			if (libssh2_channel_shell(channel_)) throw std::runtime_error("libssh2: unable to request shell on allocated pty");
		}
		~channel()
		{
			libssh2_channel_free(channel_);
		}

		void set_blocking()
		{
			libssh2_channel_set_blocking(channel_, 0);
			if (!channel_) throw std::runtime_error("libssh2: can't open channel");
		}

		bool read(unsigned char* data, std::size_t& size)
		{
			assert(size);
			int res = libssh2_channel_read(channel_, reinterpret_cast<char*>(data), size);
			if (res == LIBSSH2_ERROR_EAGAIN) return false;
			else if (res >= 0)
			{
				size = res;
			 	return true;
			}
			throw std::runtime_error("libssh2: channel faild");
		}

		void write(unsigned char const* data, std::size_t size)
		{
			if (libssh2_channel_write(channel_, reinterpret_cast<char const*>(data), size) != static_cast<int>(size))
				throw std::runtime_error("libssh2: write erorr");
		}

	private:
		LIBSSH2_CHANNEL* channel_;
	};

}}


#ifdef SMART_HOUSE_RUNTIME_DEBUG_SHARED
	#pragma comment(lib, "libssh2-vc120-debug-rt=shared.lib")
#elif defined SMART_HOUSE_RUNTIME_DEBUG_STATIC
	#pragma comment(lib, "libssh2-vc120-debug.lib")
#elif defined SMART_HOUSE_RUNTIME_RELEASE_SHARED
	#pragma comment(lib, "libssh2-vc120-rt=shared.lib")
#elif defined SMART_HOUSE_RUNTIME_RELEASE_STATIC
	#pragma comment(lib, "libssh2-vc120.lib")
#endif
#ifdef SMART_HOUSE_WINDOWS
	#pragma comment(lib, "libeay32.lib")
	#pragma comment(lib, "ssleay32.lib")
#endif