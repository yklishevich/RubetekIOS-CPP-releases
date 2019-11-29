#pragma once

#include <array>
#include <mutex>

#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/algorithm.hpp>
#include <msw/noncopyable.hpp>
#include <msw/include/mmsystem.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/system_error/throw.hpp>


namespace dawn {
namespace win  {

    void wave_out_callback(HWAVEOUT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);

    struct wave_player
        : msw::noncopyable
    {
        wave_player
        (
              msw::uint16 channels_count  = 1
            , msw::uint16 samples_per_sec = 8000
            , msw::uint16 bits_per_sample = 8
        )
            : block_length_ (msw::KB * 16)
        {
            wfx_.wFormatTag      = WAVE_FORMAT_PCM                          ;
            wfx_.nChannels       = channels_count                           ;
            wfx_.nSamplesPerSec  = samples_per_sec                          ;
            wfx_.wBitsPerSample  = bits_per_sample                          ;
            wfx_.nBlockAlign     = wfx_.wBitsPerSample / 8 * wfx_.nChannels ;
            wfx_.nAvgBytesPerSec = wfx_.nSamplesPerSec * wfx_.nBlockAlign   ;
            wfx_.cbSize          = 0                                        ;

            if (::waveOutOpen(&handle_, WAVE_MAPPER, &wfx_, reinterpret_cast<DWORD_PTR>(wave_out_callback), reinterpret_cast<DWORD_PTR>(this), CALLBACK_FUNCTION) != MMSYSERR_NOERROR)
                msw::system_error::throw_exception("wave open fail");

            for (int n = 0; n != 2; n++)
            {
                msw::raw_zeroize(header_[n]);
                header_[n].dwBufferLength = static_cast<unsigned>(block_length_.count());
                if (::waveOutPrepareHeader(handle_, &header_[n], sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
                    msw::system_error::throw_exception("wave prepare header");
            }
        }
        ~wave_player()
        {
            std::unique_lock<std::mutex> lock(mutex_);
            buffer_wait_.clear();
            lock.unlock();
            ::waveOutReset(handle_);
        }
        void play(msw::range<msw::byte const> block)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            buffer_wait_.push_back(block);
            for (int n = 0; n != 2; n++)
            {
                if (buffer_[n].empty())
                {
                    if (lock) lock.unlock();
                    play_buffer(n);
                }
            }
        }
        bool plays()
        {
            std::unique_lock<std::mutex> lock(mutex_);
            return buffer_wait_.has_items() || buffer_[0].has_items() || buffer_[1].has_items();
        }
        void callback(WAVEHDR* header)
        {
            int const n = (header == &header_[0]) ? 0 : ((header == &header_[1]) ? 1 : -1);
            MSW_ASSERT(n != -1);
            play_buffer(n);
        }
    private:
        void play_buffer(int n)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            buffer_[n].clear();
            if (buffer_wait_.has_items())
            {
                msw::range<msw::byte> block = buffer_wait_.front_max(block_length_);
                buffer_[n].push_back(block);
                if (block.size() == buffer_wait_.size()) buffer_wait_.clear();
                else buffer_wait_.erase_from_to_front(block.size());
                lock.unlock();
                {
                    header_[n].lpData = const_cast<LPSTR>(buffer_[n].data<char>().native());
                    header_[n].dwBufferLength = static_cast<unsigned>(buffer_[n].size().count());
                    if (::waveOutWrite(handle_, &header_[n], sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
                        msw::system_error::throw_exception("wave play fail");
                }
            }
        }

        msw::size<msw::byte>                  const block_length_ ;
        WAVEFORMATEX                                wfx_          ;
        HWAVEOUT                                    handle_       ;
        std::array<WAVEHDR, 2>                      header_       ;
        msw::buffer<msw::byte>                      buffer_wait_  ;
        std::array<msw::buffer<msw::byte>, 2>       buffer_       ;
        std::mutex                                  mutex_        ;
    };


    inline void wave_out_callback(HWAVEOUT, UINT msg, DWORD_PTR instance, DWORD_PTR p, DWORD_PTR)
    {
        MSW_ASSERT(instance);
        if (msg == WOM_DONE) reinterpret_cast<wave_player*>(instance)->callback(reinterpret_cast<WAVEHDR*>(p));
    }

}}
