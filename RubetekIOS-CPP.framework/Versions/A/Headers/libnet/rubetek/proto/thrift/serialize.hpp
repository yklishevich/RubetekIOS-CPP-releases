#pragma once

#include <string>
#include <memory>
#include <functional>

#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

#include <msw/config.hpp>
#include <msw/return_move.hpp>

#include <rubetek/config.hpp>

#include <lama/thrift/include/thrift.hpp>
#include <lama/thrift/include/autolink.hpp>
#include <lama/thrift/include/protocol/tprotocol.hpp>
#include <lama/thrift/include/protocol/tbinaryprotocol.hpp>
#include <lama/thrift/include/transport/tbuffertransports.hpp>


namespace rubetek
{
    template<typename T>
    void thrift_deserialize(std::string const& s, T& tag)
    {
        using namespace apache::thrift            ;
        using namespace apache::thrift::transport ;
        using namespace apache::thrift::protocol  ;
#ifdef RUBETEK_THRIFT_USE_STD_PTR
        using std::shared_ptr;
#else
        using boost::shared_ptr;
#endif

        shared_ptr<TMemoryBuffer>    trans       ( new TMemoryBuffer    ((uint8_t*)s.data(), static_cast<uint32_t>(s.size()) ) );
        shared_ptr<TFramedTransport> framed_trans( new TFramedTransport (trans                                               ) );
        shared_ptr<TProtocol>        proto       ( new TBinaryProtocol  (framed_trans                                        ) );
        tag.read(proto.get());
    }

    template<typename T>
    void thrift_deserialize(std::string const& s, T& tag, boost::optional<std::string>& error)
    {
        try
        {
            thrift_deserialize<T>(s, tag);
        }
        catch(std::exception const& e)
        {
            error = e.what();
        }
    }

    template<typename T>
    std::string thrift_serialize(T const& tag)
    {
        using namespace apache::thrift            ;
        using namespace apache::thrift::transport ;
        using namespace apache::thrift::protocol  ;
#ifdef RUBETEK_THRIFT_USE_STD_PTR
        using std::shared_ptr;
#else
        using boost::shared_ptr;
#endif

        shared_ptr<TMemoryBuffer>    trans        ( new TMemoryBuffer    (             ) );
        shared_ptr<TFramedTransport> framed_trans ( new TFramedTransport (trans        ) );
        shared_ptr<TProtocol>        proto        ( new TBinaryProtocol  (framed_trans ) );
        tag.write(proto.get());
        framed_trans->flush();
        MSW_RETURN_MOVE(trans->getBufferAsString());
    }
}
