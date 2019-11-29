#pragma once

#include <msw/plain_types.hpp>
#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>

#include <lama/libzip/include/libzip.hpp>


namespace lama {
namespace zipp {

    typedef struct zip_stat file_info;

    struct unpacker
        : msw::noncopyable
    {
        explicit               unpacker      (std::string const& file_name)                        ;
        ~                      unpacker      ()                                                    ;

        int                    files_count   ()                                                    ;
        file_info              get_file_info (int index)                                           ;
        file_info              get_file_info (std::string const& file_name)                        ;
        bool                   can_extract   (int index)                                           ;
        bool                   can_extract   (std::string const& file_name)                        ;
        msw::range<msw::byte>  extract       (int index, msw::range<msw::byte>)                    ;
        msw::range<msw::byte>  extract       (std::string const& file_name, msw::range<msw::byte>) ;
        msw::buffer<msw::byte> extract       (int index)                                           ;
        msw::buffer<msw::byte> extract       (std::string const& file_name)                        ;

    private:

        struct attach_file
        {
                      attach_file (std::string const& file_name, zip*)       ;
            ~         attach_file ()                                         ;
            zip_file* handle      ()                                   const ;
        private:
            zip_file* file_;
        };

        zip* zip_;
    };

    std::string zip_error_to_string(int error);

}}


namespace lama {
namespace zipp {

    inline unpacker::unpacker(std::string const& file_name)
    {
        int error = 0;
        zip_ = ::zip_open(file_name.c_str(), 0, &error);
        if (error) throw std::runtime_error("failed to open zip '" + file_name + "'. error: " + zip_error_to_string(error));
    }
    inline unpacker::~unpacker()
    {
        ::zip_close(zip_);
    }

    inline int unpacker::files_count()
    {
        int const total_files = zip_get_num_files(zip_);
        if (total_files == -1) throw std::runtime_error(std::string("can't get files count. error: ") + ::zip_strerror(zip_));
        return total_files;
    }
    inline file_info unpacker::get_file_info(int index)
    {
        file_info st;
        ::zip_stat_init(&st);
        if (::zip_stat_index(zip_, index, 0, &st) == -1)
            throw std::runtime_error("can't get file info [" + std::to_string(index) + "]. error: " + ::zip_strerror(zip_));
        return st;
    }
    inline file_info unpacker::get_file_info(std::string const& file_name)
    {
        file_info st;
        ::zip_stat_init(&st);
        if (::zip_stat(zip_, file_name.c_str(), 0, &st) == -1)
            throw std::runtime_error("can't get file info '" + file_name + "'. error: " + ::zip_strerror(zip_));
        return st;
    }
    inline bool unpacker::can_extract(std::string const& file_name)
    {
        struct zip_stat st;
        ::zip_stat_init(&st);
        return ::zip_stat(zip_, file_name.c_str(), 0, &st) != -1;
    }
    inline bool unpacker::can_extract(int index)
    {
        struct zip_stat st;
        ::zip_stat_init(&st);
        return ::zip_stat_index(zip_, index, 0, &st) != -1;
    }
    inline msw::range<msw::byte> unpacker::extract(int index, msw::range<msw::byte> mem)
    {
        if (!can_extract(index))
            throw std::runtime_error("can't extract [" + std::to_string(index) + "]. error: " + ::zip_strerror(zip_));

        file_info const fi = get_file_info(index);

        msw::size<msw::byte> const file_size(static_cast<msw::size<msw::byte>::count_type>(fi.size));
        if (file_size > mem.size())
            throw std::runtime_error("can't extract [" + std::to_string(index) + "]. not enough memory");
        mem.crop_front(file_size);

        attach_file attach_file_(fi.name, zip_);
        ::zip_fread(attach_file_.handle(), mem.data().native(), mem.size().count());

        return mem;
    }
    inline msw::range<msw::byte> unpacker::extract(std::string const& file_name, msw::range<msw::byte> mem)
    {
        if (!can_extract(file_name))
            throw std::runtime_error("can't extract '" + file_name + "'. error: " + ::zip_strerror(zip_));

        msw::size<msw::byte> const file_size(static_cast<msw::size<msw::byte>::count_type>(get_file_info(file_name).size));
        if (file_size > mem.size())
            throw std::runtime_error("can't extract '" + file_name + "'. not enough memory");
        mem.crop_front(file_size);

        attach_file attach_file_(file_name, zip_);
        ::zip_fread(attach_file_.handle(), mem.data().native(), mem.size().count());

        return mem;
    }
    inline msw::buffer<msw::byte> unpacker::extract(int index)
    {
        if (!can_extract(index))
            throw std::runtime_error("can't extract [" + std::to_string(index) + "]. error: " + ::zip_strerror(zip_));

        file_info const fi = get_file_info(index);

        msw::buffer<msw::byte> file_data(msw::size<msw::byte>(static_cast<msw::size<msw::byte>::count_type>(fi.size)));

        attach_file attach_file_(fi.name, zip_);
        ::zip_fread(attach_file_.handle(), file_data.data().native(), file_data.size().count());

        return std::move(file_data);
    }
    inline msw::buffer<msw::byte> unpacker::extract(std::string const& file_name)
    {
        if (!can_extract(file_name))
            throw std::runtime_error("can't extract '" + file_name + "'. error: " + ::zip_strerror(zip_));

        msw::buffer<msw::byte> file_data(msw::size<msw::byte>(static_cast<msw::size<msw::byte>::count_type>(get_file_info(file_name).size)));

        attach_file attach_file_(file_name, zip_);
        ::zip_fread(attach_file_.handle(), file_data.data().native(), file_data.size().count());

        return std::move(file_data);
    }

    inline unpacker::attach_file::attach_file(std::string const& file_name, zip* zip)
    {
        file_ = ::zip_fopen(zip, file_name.c_str(), 0);
        if (!file_) throw std::runtime_error(std::string("error occurred while retrieving the file '" + file_name + "'. error: " + ::zip_file_strerror(file_)));
    }
    inline unpacker::attach_file::~attach_file()
    {
        ::zip_fclose(file_);
    }
    inline zip_file* unpacker::attach_file::handle() const
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
