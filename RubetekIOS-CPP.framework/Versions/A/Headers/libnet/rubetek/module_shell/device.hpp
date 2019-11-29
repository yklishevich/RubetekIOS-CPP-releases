#pragma once

#include <vector>
#include <functional>
#include <unordered_map>

#include <msw/format.hpp>
#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>

#include <rubetek/essence/property.hpp>
#include <rubetek/module_shell/device_session.hpp>


namespace rubetek      {
namespace module_shell {

    struct device
        : msw::noncopyable
    {
        typedef std::vector<property_id_type>    readable_property_id_list ;
        typedef std::unique_ptr<device_session>  session_ptr               ;
        typedef std::function<void(std::string)> on_error                  ;

        explicit             device                    (readable_property_id_list const&, session_ptr, on_error = nullptr) ;
                             ~device                   ()                                                                  ;

        session_ptr          eject_session             ()                                                                  ;

        void                 change_properties         (property_list const&)                                              ;
        property_list        check_readable_properties ()                                                                  ;
        void                 idle                      ()                                                                  ;

    private:
        bool                 accessible                ()                                                                  ;
        property::value_type property_value            (property_id_type)                                                  ;

        typedef std::unordered_map<property_id_type, property::value_type> readable_property_values;

        readable_property_id_list const readable_property_id_list_ ;
        on_error                  const on_error_                  ;
        readable_property_values        readable_property_values_  ;
        session_ptr                     session_                   ;
        bool                            trouble_                   ;
    };

}}


namespace rubetek      {
namespace module_shell {

    inline device::device(readable_property_id_list const& readable_property_id_list, session_ptr session, on_error on_error)
        : readable_property_id_list_ ( readable_property_id_list )
        , on_error_                  ( on_error                  )
        , session_                   ( std::move(session)        )
        , trouble_                   ( false                     )
    {}
    inline device::~device()
    {}

    inline device::session_ptr device::eject_session()
    {
        return std::move(session_);
    }

    inline void device::change_properties(property_list const& properties)
    {
        if (accessible())
        {
            for (property const& prop : properties)
            {
                try
                {
                    session_->write_property(prop);
                }
                catch (std::exception const& e)
                {
                    trouble_ = true;
                    std::string const error = msw::format("write property id (", prop.id, "): ", e.what());
                    if (on_error_) on_error_(error);
                    else msw::throw_runtime_error(error);
                    break;
                }
            }
        }
    }
    inline property_list device::check_readable_properties()
    {
        property_list properties;
        try
        {
            if (accessible())
            {
                readable_property_values last_readable_property_values(readable_property_values_);
                readable_property_values_.clear();
                for (property_id_type id : readable_property_id_list_)
                    readable_property_values_[id] = property_value(id);
                for (std::pair<property_id_type, property::value_type> const& prop : readable_property_values_)
                {
                    auto it = last_readable_property_values.find(prop.first);
                    bool const found = it != last_readable_property_values.end();
                    if (!(found && (it->second == prop.second)))
                        properties.push_back(property(prop.first, prop.second));
                }
            }
            else
            {
                if (!readable_property_values_.empty())
                {
                    properties.push_back(property(0, false));
                    readable_property_values_.clear();
                }
            }
        }
        catch (std::exception const& e)
        {
            trouble_ = true;
            std::string const error = msw::format("check readable properties: ", e.what());
            if (on_error_) on_error_(error);
            else msw::throw_runtime_error(error);
        }

        return properties;
    }
    inline void device::idle()
    {
        MSW_ASSERT(session_);
        session_->idle();
    }

    inline bool device::accessible()
    {
        if (trouble_) return false;
        MSW_ASSERT(session_);
        return session_->accessible();
    }
    inline property::value_type device::property_value(property_id_type id)
    {
        MSW_ASSERT(session_);
        return session_->read_property_value(id);
    }

}}
