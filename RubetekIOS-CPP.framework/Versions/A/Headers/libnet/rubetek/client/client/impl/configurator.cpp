#include <string>
#include <fstream>

#include <boost/filesystem.hpp>

#include <msw/assert.hpp>

#include <lama/boost/include/property_tree/info_parser.hpp>

#include <rubetek/essence/unique_id/controller.hpp>
#ifndef RUBETEK_HEADER_ONLY
#   include <rubetek/client/client/configurator.hpp>
#endif



namespace rubetek {
namespace client  {

    inline configurator::configurator(std::string const& path)
        : filename_                 ( path + "/link.info"              )
        , auto_connect_by_location_ ( default_auto_connect_by_location )
        , auto_connect_to_recent_   ( default_auto_connect_to_recent   )
    {
        boost::filesystem::create_directories(path);
        load();
    }

    inline void configurator::auto_connect_by_location(bool yes)
    {
        if (auto_connect_by_location_ != yes)
        {
            auto_connect_by_location_ = yes;
            save();
        }
    }
    inline bool configurator::auto_connect_by_location() const
    {
        return auto_connect_by_location_;
    }
    inline void configurator::auto_connect_to_recent(bool yes)
    {
        if (auto_connect_to_recent_ != yes)
        {
            auto_connect_to_recent_ = yes;
            save();
        }
    }
    inline bool configurator::auto_connect_to_recent() const
    {
        return auto_connect_to_recent_;
    }
    inline void configurator::recent_controller(unique_controller_id::type id)
    {
        if (recent_controller_ != id)
        {
            recent_controller_ = id;
            save();
        }
    }
    inline unique_controller_id::type configurator::recent_controller() const
    {
        MSW_ASSERT(has_recent_controller());
        return recent_controller_;
    }
    inline bool configurator::has_recent_controller() const
    {
        return !recent_controller_.empty();
    }
    inline void configurator::remove_recent_controller()
    {
        recent_controller_.clear();
        save();
    }

    inline void configurator::load()
    {
        boost::property_tree::ptree pt;
        std::ifstream file(filename_);
        boost::property_tree::read_info(file, pt);
#ifdef RUBETEK_CLIENT_ENABLE_AUTO_CONNECT_SETTINGS
        auto_connect_by_location_ = pt.get<bool> ( "auto_connect_by_location" , default_auto_connect_by_location );
        auto_connect_to_recent_   = pt.get<bool> ( "auto_connect_to_recent"   , default_auto_connect_to_recent   );
#else
        auto_connect_by_location_ =  default_auto_connect_by_location ;
        auto_connect_to_recent_   =  default_auto_connect_to_recent   ;
#endif
        recent_controller_ = pt.get<std::string> ( "recent_controller", "");
    }
    inline void configurator::save() const
    {
        boost::property_tree::ptree pt;
        pt.add( "auto_connect_by_location" , auto_connect_by_location_ );
        pt.add( "auto_connect_to_recent"   , auto_connect_to_recent_   );
        pt.add( "recent_controller"        , recent_controller_        );
        std::ofstream file(filename_);
        boost::property_tree::write_info(file, pt);
    }

}}
