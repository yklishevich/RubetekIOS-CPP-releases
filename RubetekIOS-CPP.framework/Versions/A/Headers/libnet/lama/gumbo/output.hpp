#pragma once

#include <list>
#include <string>

#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/assert.hpp>
#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>

#include <lama/gumbo/include/gumbo.hpp>


namespace lama  {
namespace gumbo {

    struct allocator
        : msw::noncopyable
    {
        static void* allocate(void* userdata, size_t size)
        {
            return reinterpret_cast<allocator*>(userdata)->allocate(size);
        }
        static void deallocate(void* /*userdata*/, void* /*ptr*/)
        {

        }
        void* allocate(size_t size)
        {
            bufs_.emplace_back(msw::size<msw::byte>(size));
            return bufs_.back().data().native();
        }
    private:
        std::list<msw::buffer<msw::byte>> bufs_;
    };

    struct output
    {
        explicit output(char const* in)
            : options_ (kGumboDefaultOptions)
            , value    (::gumbo_parse(in))
        {
            MSW_ASSERT(value)
        }
        explicit output(std::string const& in)
            : output(in.c_str())
        {}

        explicit output(char const* buffer, size_t buffer_length, bool stop_on_first_error = false)
            : options_ (set_options(this, stop_on_first_error))
            , value    (::gumbo_parse_with_options(&options_, buffer, buffer_length))
        {
            MSW_ASSERT(value)
            if (!has_data()) msw::throw_runtime_error("gumbo parse failed");
        }
        explicit output(msw::range<msw::byte const> block, bool stop_on_first_error = false)
            : output(block.data<char>().native(), block.size().count(), stop_on_first_error)
        {}

        bool has_data() const
        {
            return !!value->root;
        }
        bool has_errors() const
        {
            return value->errors.length != 0;
        }

        ~output()
        {
            ::gumbo_destroy_output(&options_, value);
        }
    private:
        static GumboOptions set_options(output* /*self*/, bool stop_on_first_error)
        {
            GumboOptions options(kGumboDefaultOptions);
            //options.allocator = allocator::allocate;
            //options.deallocator = allocator::deallocate;
            //options.userdata = &self->allocator_;
            options.stop_on_first_error = stop_on_first_error;
            return options;
        }

        allocator    allocator_ ;
        GumboOptions options_   ;
    public:
        GumboOutput* value;
    };

}}
