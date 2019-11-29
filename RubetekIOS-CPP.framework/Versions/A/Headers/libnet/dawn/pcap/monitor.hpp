#pragma once

#include <mutex>
#include <thread>

#include <msw/dimension/packet_multimeter.hpp>

#include <lama/pcap/include_all.hpp>


namespace dawn {
namespace pcap {

    struct monitor_receiver
    {
        explicit monitor_receiver(std::string name)
            : receiver_ ( name )
        {}
        void receive()
        {
            receiver_.next
            (
                [this](lama::pcap::packet_header, msw::range<msw::byte const> packet)
                {
                    multimeter_.add_packet(packet);
                }
            );
        }
        msw::packet_multimeter::snapshot snapshot()
        {
            return multimeter_.make_snapshot();
        }
    private:
        lama::pcap::stable_receiver receiver_   ;
        msw::packet_multimeter      multimeter_ ;
    };

    struct monitor
    {
        struct statistics
        {
            lama::pcap::device_info          device_info ;
            msw::packet_multimeter::snapshot snapshot    ;
        };

        monitor()
            : do_(true)
        {
            std::vector<lama::pcap::device_info> const devices = lama::pcap::find_devices();
            for (lama::pcap::device_info const& info : devices)
            {
                receivers_.push_back(std::make_unique<monitor_receiver>(info.name));
                statistics_.push_back(statistics{info, msw::packet_multimeter::snapshot()});
            }
            thread_ = std::thread
            (
                [this]()
                {
                    while (do_)
                    {
                        for (std::unique_ptr<monitor_receiver>& r : receivers_)
                        {
                            if (!do_) break;
                            r->receive();
                        }
                        std::unique_lock<std::mutex> lock(mutex_);
                        size_t n = 0;
                        for (std::unique_ptr<monitor_receiver>& r : receivers_)
                            statistics_[n++].snapshot = r->snapshot();
                    }
                }
            );
        }
        ~monitor()
        {
            do_ = false;
            thread_.join();
        }
        int count() const
        {
            return static_cast<int>(statistics_.size());
        }
        statistics get(int n)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            return statistics_[n];
        }
    private:
        bool                                           do_         ;
        std::vector<statistics>                        statistics_ ;
        std::vector<std::unique_ptr<monitor_receiver>> receivers_  ;
        std::mutex                                     mutex_      ;
        std::thread                                    thread_     ;
    };

}}
