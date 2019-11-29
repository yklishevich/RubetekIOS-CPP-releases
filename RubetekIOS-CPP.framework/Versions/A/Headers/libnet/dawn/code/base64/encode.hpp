#pragma once

#include <string>
#include <vector>


namespace dawn   {
namespace base64 {

    std::basic_string<char> encode(std::vector<unsigned char> const&) ;
    std::basic_string<char> encode(std::basic_string<char> const&)    ;

}}


namespace dawn   {
namespace base64 {

    inline std::basic_string<char> encode(std::vector<unsigned char> const& in_buf)
    {
        static const char encodeLookup[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        static const char padCharacter = '=';
        std::basic_string<char> encoded_string;
        encoded_string.reserve(((in_buf.size()/3) + (in_buf.size() % 3 > 0)) * 4);
        unsigned temp;
        std::vector<unsigned char>::const_iterator cursor = in_buf.begin();
        for(size_t idx = 0; idx < in_buf.size()/3; idx++)
        {
            temp  = (*cursor++) << 16; //Convert to big endian
            temp += (*cursor++) << 8;
            temp += (*cursor++);
            encoded_string.append(1, encodeLookup[(temp & 0x00FC0000) >> 18]);
            encoded_string.append(1, encodeLookup[(temp & 0x0003F000) >> 12]);
            encoded_string.append(1, encodeLookup[(temp & 0x00000FC0) >> 6 ]);
            encoded_string.append(1, encodeLookup[(temp & 0x0000003F)      ]);
        }
        switch(in_buf.size() % 3)
        {
        case 1:
            temp  = (*cursor++) << 16; //Convert to big endian
            encoded_string.append(1, encodeLookup[(temp & 0x00FC0000) >> 18]);
            encoded_string.append(1, encodeLookup[(temp & 0x0003F000) >> 12]);
            encoded_string.append(2, padCharacter);
            break;
        case 2:
            temp  = (*cursor++) << 16; //Convert to big endian
            temp += (*cursor++) << 8;
            encoded_string.append(1, encodeLookup[(temp & 0x00FC0000) >> 18]);
            encoded_string.append(1, encodeLookup[(temp & 0x0003F000) >> 12]);
            encoded_string.append(1, encodeLookup[(temp & 0x00000FC0) >> 6 ]);
            encoded_string.append(1, padCharacter);
            break;
        }
        return encoded_string;
    }

    inline std::basic_string<char> encode(std::basic_string<char> const& s)
    {
        return encode(std::vector<unsigned char>(s.begin(), s.end()));
    }

}}
