#pragma once
#define ZIP_EXTERN

#include <zip.h>

#include <rubetek/essence/buffer.hpp>
#include <rubetek/utility/noncopyable.hpp>


namespace rubetek {
namespace utility {

	struct zip_file_unpacker
		: utility::noncopyable
	{
		explicit	zip_file_unpacker	(std::string const& file_name)	;
		~			zip_file_unpacker	()								;

		bool		can_extract			(std::string const& file_name)	;
		buffer		extract				(std::string const& file_name)	;
		unsigned	crc					(std::string const& file_name)	;

	private:

		struct attach_file
		{
						attach_file	(std::string const& file_name, zip*)			;
			~			attach_file	()												;
			zip_file*	handle		()										const	;
		private:
			zip_file* file_;
		};

		zip* zip_;
	};

	std::string zip_error_to_string(int error);

}}


namespace rubetek {
namespace utility {

	inline zip_file_unpacker::zip_file_unpacker(std::string const& file_name)
	{
		int error = 0;
		zip_ = zip_open(file_name.c_str(), 0, &error);
		if (error) throw std::runtime_error("failed to open zip <" + file_name + ">. error: " + zip_error_to_string(error));
	}
	inline zip_file_unpacker::~zip_file_unpacker()
	{
		zip_close(zip_);
	}

	inline bool zip_file_unpacker::can_extract(std::string const& file_name)
	{
		struct zip_stat st;
		zip_stat_init(&st);
		return zip_stat(zip_, file_name.c_str(), 0, &st) != -1;
	}
	inline buffer zip_file_unpacker::extract(std::string const& file_name)
	{
		struct zip_stat st;
		zip_stat_init(&st);
		if (zip_stat(zip_, file_name.c_str(), 0, &st) == -1)
			throw std::runtime_error("can't extract <" + file_name + ">. error: " + zip_strerror(zip_));

		attach_file attach_file_(file_name, zip_);
		buffer file_data(static_cast<std::size_t>(st.size));
		zip_fread(attach_file_.handle(), file_data.data(), file_data.size());

		return std::move(file_data);
	}
	inline unsigned zip_file_unpacker::crc(std::string const& file_name)
	{
		struct zip_stat st;
		zip_stat_init(&st);
		if (zip_stat(zip_, file_name.c_str(), 0, &st) == -1)
			throw std::runtime_error("can't get crc <" + file_name + ">. error: " + zip_strerror(zip_));
		return st.crc;
	}

	inline zip_file_unpacker::attach_file::attach_file(std::string const& file_name, zip* zip)
	{
		file_ = zip_fopen(zip, file_name.c_str(), 0);
		if (!file_) throw std::runtime_error(std::string("error occurred while retrieving the file <" + file_name + ">. error: " + zip_file_strerror(file_)));
	}
	inline zip_file_unpacker::attach_file::~attach_file()
	{
		zip_fclose(file_);
	}
	inline zip_file* zip_file_unpacker::attach_file::handle() const
	{
		return file_;
	}


	inline std::string zip_error_to_string(int error)
	{
		char error_str[1024];
		zip_error_to_str(error_str, sizeof(error_str), error, errno);
		return error_str;
	}

}}


#ifdef SMART_HOUSE_RUNTIME_DEBUG_SHARED
	#pragma comment(lib, "libzip-vc120-debug-rt=shared.lib")
#elif defined SMART_HOUSE_RUNTIME_DEBUG_STATIC
	#pragma comment(lib, "libzip-vc120-debug.lib")
#elif defined SMART_HOUSE_RUNTIME_RELEASE_SHARED
	#pragma comment(lib, "libzip-vc120-rt=shared.lib")
#elif defined SMART_HOUSE_RUNTIME_RELEASE_STATIC
	#pragma comment(lib, "libzip-vc120.lib")
#endif