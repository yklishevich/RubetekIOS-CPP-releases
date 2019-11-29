#pragma once

#include <string>

#include <msw/ascii/is_template.hpp>


namespace dawn  {
namespace ascii {

    DAWN_ASCII_IS_TEMPLATE(lowercase_letter, 97, 122)
    DAWN_ASCII_IS_TEMPLATE(uppercase_letter, 65, 90)

    inline bool is_letter(char letter)
    {
        return is_lowercase_letter(letter) || is_uppercase_letter(letter);
    }
    inline bool is_letters(std::string const& letters)
    {
        return is_lowercase_letters(letters) || is_uppercase_letters(letters);
    }

}}
