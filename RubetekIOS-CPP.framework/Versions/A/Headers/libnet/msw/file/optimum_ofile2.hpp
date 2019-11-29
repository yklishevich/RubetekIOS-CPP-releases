#pragma once

#include <msw/std/memory.hpp>
#include <msw/file/optimum_ofile.hpp>


namespace msw
{
    struct optimum_ofile2
    {
        explicit       optimum_ofile2 (
                                            std::string const& filename
                                          , msw::size<byte>    block_size    = MB
                                          , bool               lazy_creation = true
                                      )                         ;
        void           write          (range<byte const>)       ;
        uint64         size           ()                  const ;
        void           flush          ()                        ;
    private:
        optimum_ofile& get            ()                  const ;
        std::string                    const   filename_   ;
        msw::size<byte>                const   block_size_ ;
        std::unique_ptr<optimum_ofile> mutable ofile_      ;
    };
}


namespace msw
{
    inline optimum_ofile2::optimum_ofile2(std::string const& filename, msw::size<byte> block_size, bool lazy_creation)
        : filename_   ( filename   )
        , block_size_ ( block_size )
    {
        if (!lazy_creation) get();
    }
    inline void optimum_ofile2::write(range<byte const> r)
    {
        get().write(r);
    }
    inline uint64 optimum_ofile2::size() const
    {
        return get().size();
    }
    inline void optimum_ofile2::flush()
    {
        get().flush();
    }

    inline optimum_ofile& optimum_ofile2::get() const
    {
        if (!ofile_) ofile_ = msw::make_unique<optimum_ofile>(filename_, block_size_);
        return *ofile_;
    }
}
