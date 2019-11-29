#pragma once


namespace msw       {
namespace pcap_file {
namespace ng        {

    enum class block_type : msw::qbyte
    {
          reserved
        , interface_description
        , packet
        , simple_packet
        , name_resolution
        , interface_statistics_block
        , enhanced_packet
        , header = 0x0a0d0d0a
    };

    inline std::string block_type_to_string(block_type type)
    {
#define MSW_PCAP_FILE_NG_BLOCK_TYPE_TO_STRING(T) if (type == block_type::T) return #T;
        MSW_PCAP_FILE_NG_BLOCK_TYPE_TO_STRING( reserved                   )
        MSW_PCAP_FILE_NG_BLOCK_TYPE_TO_STRING( interface_description      )
        MSW_PCAP_FILE_NG_BLOCK_TYPE_TO_STRING( packet                     )
        MSW_PCAP_FILE_NG_BLOCK_TYPE_TO_STRING( simple_packet              )
        MSW_PCAP_FILE_NG_BLOCK_TYPE_TO_STRING( name_resolution            )
        MSW_PCAP_FILE_NG_BLOCK_TYPE_TO_STRING( interface_statistics_block )
        MSW_PCAP_FILE_NG_BLOCK_TYPE_TO_STRING( enhanced_packet            )
        MSW_PCAP_FILE_NG_BLOCK_TYPE_TO_STRING( header                     )
#undef MSW_PCAP_FILE_NG_BLOCK_TYPE_TO_STRING
        return "unknown";
    }

    msw::uint32 const block_header_min_length = 12;

#pragma pack(push, 1)
    struct block_header
    {
        block_type  type   ;
        msw::uint32 length ;

        block_header& default_initialize()
        {
            type   = block_type::reserved ;
            length = 0                    ;
            return *this;
        }
    };
#pragma pack(pop)

    struct block_reader
        : noncopyable
    {
        explicit    block_reader     (std::string const& file_name)       ;

        bool        has_blocks       ()                             const ;
        block_type  get_block_type   ()                             const ;
        size<byte>  block_size       ()                             const ;
        range<byte> read_block       ()                                   ;

        uint64      readed_raw_bytes ()                             const ;

    private:

        void read_block_header();

        optimum_ifile ifile_        ;
        bool          has_blocks_   ;
        block_header  block_header_ ;
    };


    inline block_reader::block_reader(std::string const& file_name)
        : ifile_ (file_name)
    {
        read_block_header();
    }

    inline bool block_reader::has_blocks() const
    {
        return has_blocks_;
    }
    inline block_type block_reader::get_block_type() const
    {
        MSW_ASSERT(has_blocks());
        return block_header_.type;
    }
    inline size<byte> block_reader::block_size() const
    {
        MSW_ASSERT(has_blocks());
        return block_header_.length;
    }
    inline range<byte> block_reader::read_block()
    {
        MSW_ASSERT(has_blocks());
        if (block_size() > MB) throw std::runtime_error("very large block size");
        range<byte> block = ifile_.read_imperative(block_size());
        read_block_header();
        return block;
    }

    inline uint64 block_reader::readed_raw_bytes() const
    {
        return ifile_.position();
    }

    inline void block_reader::read_block_header()
    {
        range<byte const> r = ifile_.read_imperative(sizeof(block_header), has_blocks_);
        if (has_blocks_) block_header_ = r.as<block_header>();
        else             block_header_.default_initialize();
    }

}}}


/*
http://www.winpcap.org/ntar/draft/PCAP-DumpFileFormat.html
https://github.com/HoneProject/Linux-Sensor/wiki/Augmented-PCAP-Next-Generation-Dump-File-Format
*/
