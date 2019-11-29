#pragma once 

#include <msw/size.hpp>


namespace msw
{
    template <typename Block>
    size<byte> total_size_of_blocks(Block&& block)
    {
        return make_range<byte const>(std::forward<Block>(block)).size();
    }
    template <typename Block, typename ...Blocks>
    size<byte> total_size_of_blocks(Block&& block, Blocks&&... blocks)
    {
        return total_size_of_blocks(std::forward<Block>(block)) + total_size_of_blocks(std::forward<Blocks>(blocks)...);
    }
}
