#pragma once


namespace msw
{
    typedef unsigned char      byte   ;
    typedef unsigned short     wbyte  ;
    typedef unsigned int       qbyte  ;
    typedef unsigned long long ebyte  ;

    typedef char               int8   ;
    typedef short              int16  ;
    typedef int                int32  ;
    typedef long long          int64  ;

    typedef unsigned char      uint8  ;
    typedef unsigned short     uint16 ;
    typedef unsigned int       uint32 ;
    typedef unsigned long long uint64 ;

    static uint8  const uint8_max  = static_cast<uint8 >(-1);
    static uint16 const uint16_max = static_cast<uint16>(-1);
    static uint32 const uint32_max = static_cast<uint32>(-1);
    static uint64 const uint64_max = static_cast<uint64>(-1);

#define MSW_PLAIN_TYPE_CHECK_SIZE(T, S) static_assert(sizeof(T) == S, "size of "#T" must be equal to "#S" B");
    MSW_PLAIN_TYPE_CHECK_SIZE( byte  , 1 )
    MSW_PLAIN_TYPE_CHECK_SIZE( wbyte , 2 )
    MSW_PLAIN_TYPE_CHECK_SIZE( qbyte , 4 )
    MSW_PLAIN_TYPE_CHECK_SIZE( ebyte , 8 )
    MSW_PLAIN_TYPE_CHECK_SIZE( int8  , 1 )
    MSW_PLAIN_TYPE_CHECK_SIZE( int16 , 2 )
    MSW_PLAIN_TYPE_CHECK_SIZE( int32 , 4 )
    MSW_PLAIN_TYPE_CHECK_SIZE( int64 , 8 )
    MSW_PLAIN_TYPE_CHECK_SIZE( uint8 , 1 )
    MSW_PLAIN_TYPE_CHECK_SIZE( uint16, 2 )
    MSW_PLAIN_TYPE_CHECK_SIZE( uint32, 4 )
    MSW_PLAIN_TYPE_CHECK_SIZE( uint64, 8 )
#undef MSW_PLAIN_TYPE_CHECK_SIZE

    struct bit
    {
        typedef byte        base_type                             ;
        typedef uint8       offset_type                           ;
        static  uint8 const base_capacity = sizeof(base_type) * 8 ;
    private:
        ~bit();
    };
}
