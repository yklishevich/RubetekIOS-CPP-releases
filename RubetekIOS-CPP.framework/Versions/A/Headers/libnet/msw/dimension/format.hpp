#pragma once 

#include <msw/lexical_cast/format_size.hpp>
#include <msw/lexical_cast/format_count.hpp>
#include <msw/dimension/packet_multimeter.hpp>


namespace msw
{
    inline std::string format_packet_multimeter_count(packet_multimeter::snapshot const& shot)
    {
        return format_count(shot.packets.distance, "pkt") + " (" + format_size(shot.bytes.distance, "B)");
    }

    inline std::string format_packet_multimeter_speed(packet_multimeter::snapshot const& shot)
    {
        return format_count(shot.packets.speed, "pkt/s") + " (" + format_size(shot.bytes.speed, "B/s)");
    }

    inline std::string format_multimeter(multimeter::snapshot const& shot, std::string const& measure_unit = "")
    {
        return format_count(shot.distance, measure_unit) + " (" + format_size(shot.speed, measure_unit + "/s)");
    }
}
