#pragma once

#include <string>
#include <vector>


namespace dawn   {
namespace base64 {

    std::vector<unsigned char>decode(std::basic_string<char> const&);

}}


namespace dawn   {
namespace base64 {

    inline std::vector<unsigned char> decode(std::basic_string<char> const& input)
    {
        static const char pad_char = '=';
        if (input.length() % 4) throw std::runtime_error("non-valid Base 64");
        size_t padding = 0;
        if (input.length())
        {
            if (input[input.length()-1] == pad_char)
                padding++;
            if (input[input.length()-2] == pad_char)
                padding++;
        }
        std::vector<unsigned char> decoded_bytes;
        decoded_bytes.reserve(((input.length()/4)*3) - padding);
        unsigned temp = 0; //Holds decoded quanta
        std::basic_string<char>::const_iterator cursor = input.begin();
        while (cursor < input.end())
        {
            for (size_t quantumPosition = 0; quantumPosition < 4; quantumPosition++)
            {
                temp <<= 6;
                if       (*cursor >= 0x41 && *cursor <= 0x5A)   // This area will need tweaking if
                    temp |= *cursor - 0x41;                     // you are using an alternate alphabet
                else if  (*cursor >= 0x61 && *cursor <= 0x7A)
                    temp |= *cursor - 0x47;
                else if  (*cursor >= 0x30 && *cursor <= 0x39)
                    temp |= *cursor + 0x04;
                else if  (*cursor == 0x2B)
                    temp |= 0x3E;                               //change to 0x2D for URL alphabet
                else if  (*cursor == 0x2F)
                    temp |= 0x3F;                               //change to 0x5F for URL alphabet
                else if  (*cursor == pad_char)                  //pad
                {
                    switch( input.end() - cursor )
                    {
                    case 1: //One pad character
                        decoded_bytes.push_back((temp >> 16) & 0x000000FF);
                        decoded_bytes.push_back((temp >> 8 ) & 0x000000FF);
                        return decoded_bytes;
                    case 2: //Two pad characters
                        decoded_bytes.push_back((temp >> 10) & 0x000000FF);
                        return decoded_bytes;
                    default: throw std::runtime_error("invalid padding in Base 64!");
                    }
                }
                else throw std::runtime_error("non-valid character in Base 64!");
                cursor++;
            }
            decoded_bytes.push_back((temp >> 16) & 0x000000FF);
            decoded_bytes.push_back((temp >> 8 ) & 0x000000FF);
            decoded_bytes.push_back((temp      ) & 0x000000FF);
        }
        return decoded_bytes;
    }

}}
