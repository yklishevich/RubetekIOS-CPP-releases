#pragma once

#include <msw/config.hpp>
#include <msw/create_director.hpp>

#ifdef MSW_WINAPI_FILE
#   include <msw/include/windows.hpp>
#else
#   include <sys/stat.h> // S_I file modes
#   include <fcntl.h>    // open, O_RDONLY, O_WRONLY
#   include <unistd.h>   // read, write, lseek
#endif


namespace msw  {
namespace file {

    typedef
#ifdef MSW_WINAPI_FILE
        HANDLE
#else
        int
#endif
        handle;

    inline handle open_read(char const* file_name)
    {
        handle h =
#ifdef MSW_WINAPI_FILE
        ::CreateFileA
        (
              file_name
            , GENERIC_READ
            , FILE_SHARE_READ
            , NULL
            , OPEN_EXISTING
            , FILE_ATTRIBUTE_NORMAL
            , NULL
        );
        if (h == INVALID_HANDLE_VALUE) h = 0;
#else
        ::open
        (
              file_name
            , O_RDONLY
#ifdef MSW_WINDOWS
            | O_BINARY
#endif
        );
        if (h < 0) h = 0;
#endif
        return h;
    }
    inline handle open_write(char const* file_name, bool append = false)
    {
        create_directory_for_file(file_name);
        handle h =
#ifdef MSW_WINAPI_FILE
        ::CreateFileA
        (
              file_name
            , GENERIC_WRITE
            , FILE_SHARE_WRITE
            , NULL
            , append ? OPEN_ALWAYS : CREATE_ALWAYS
            , FILE_ATTRIBUTE_NORMAL
            , NULL
        );
        if (h == INVALID_HANDLE_VALUE) h = 0;
#else
        ::open
        (
              file_name
            , O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC)
#   ifdef MSW_WINDOWS
            | O_BINARY
#   endif
#   ifndef MSW_ANDROID
            , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
#   endif
        );
        if (h < 0) h = 0;
#endif
        return h;
    }

    inline void close(handle h)
    {
#ifdef MSW_WINAPI_FILE
        ::CloseHandle(h);
#else
        ::close(h);
#endif
    }
    inline bool read(handle h, void* data, unsigned count_of_bytes, unsigned long& readed_bytes)
    {
#ifdef MSW_WINAPI_FILE
        return ::ReadFile(h, data, count_of_bytes, &readed_bytes, NULL) == TRUE;
#else
        int ret = ::read(h, data, count_of_bytes);
        if (ret == -1) return false;
        readed_bytes = ret;
        return true;
#endif
    }
    inline bool write(handle h, void const* data, unsigned count_of_bytes)
    {
        unsigned long written_bytes = 0;
#ifdef MSW_WINAPI_FILE
        if (!::WriteFile(h, data, count_of_bytes, &written_bytes, NULL)) return false;
#else
        int ret = ::write(h, data, count_of_bytes);
        if (ret == -1) return false;
        written_bytes = ret;
#endif
        return written_bytes == count_of_bytes;
    }

#ifdef MSW_WINAPI_FILE
    inline long long win_seek(handle h, long long distance_to_move, int move_method)
    {
        LARGE_INTEGER pos;
        LARGE_INTEGER dist;
        dist.QuadPart = distance_to_move;
        return ::SetFilePointerEx(h, dist, &pos, move_method) ? pos.QuadPart : -1;
    }
#else
    inline long long lin_seek(handle h, long long distance_to_move, int move_method)
    {
#   ifndef MSW_DISABLE_LARGE_FILE
        static_assert(sizeof(off_t) == 8, "Necessary large file support. Please define macros: _FILE_OFFSET_BITS 64 or difine in the compiler options: -D_FILE_OFFSET_BITS64");
#   endif
        return ::lseek(h, distance_to_move, move_method);
    }
#endif

    inline long long seek_from_begin(handle h, long long distance_to_move)
    {
        return
#ifdef MSW_WINAPI_FILE
        win_seek(h, distance_to_move, FILE_BEGIN);
#else
        lin_seek(h, distance_to_move, SEEK_SET);
#endif
    }
    inline long long seek_from_end(handle h, long long distance_to_move)
    {
        return
#ifdef MSW_WINAPI_FILE
        win_seek(h, distance_to_move, FILE_END);
#else
        lin_seek(h, distance_to_move, SEEK_END);
#endif
    }
    inline long long seek_from_current(handle h, long long distance_to_move)
    {
        return
#ifdef MSW_WINAPI_FILE
        win_seek(h, distance_to_move, FILE_CURRENT);
#else
        lin_seek(h, distance_to_move, SEEK_CUR);
#endif
    }

    inline bool peek(handle h, void* data, unsigned count_of_bytes, unsigned long& readed_bytes)
    {
        long long pos = seek_from_current(h, 0);
        bool ret = read(h, data, count_of_bytes, readed_bytes);
        seek_from_begin(h, pos);
        return ret;
    }

    inline long long size(handle h)
    {
        long long size;
#ifdef MSW_WINAPI_FILE
        if (!::GetFileSizeEx(h, &reinterpret_cast<::LARGE_INTEGER&>(size))) return -1;
#else
        long long pos = seek_from_current(h, 0);
        size = seek_from_end(h, 0);
        seek_from_begin(h, pos);
#endif
        return size;
    }

}}
