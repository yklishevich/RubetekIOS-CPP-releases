#pragma once

#include <string>
#include <functional>

#include <rubetek/utility/string_map.hpp>
#include <rubetek/utility/noncopyable.hpp>
#include <rubetek/module/property_map.hpp>
#include <rubetek/module/device/property/accessor.hpp>


namespace rubetek {
namespace module  {

    template <typename T>
    struct definite_property_accessor
        : property_accessor
    {
        typedef std::function<T()>            read_property_type  ;
        typedef std::function<void(T const&)> write_property_type ;

                   definite_property_accessor (read_property_type, write_property_type) ;

        value_type read                       ()                                        ;
        void       write                      (value_type const&)                       ;

    private:
        read_property_type  read_  ;
        write_property_type write_ ;
    };


    struct property_indexes
        : utility::string_map<std::size_t>
    {
        property_indexes();
    };


    struct property_manager
        : utility::noncopyable
    {
                                                                                  property_manager (property_accessor_map&, property_map const&)          ;
                                                                                  property_manager (property_manager&&)                                   ;

        template <typename T, typename ReadProperty>                         void push             (std::string const& name, ReadProperty, int = 0)       ;
        template <typename T, typename WriteProperty>                        void push             (std::string const& name, int, WriteProperty)          ;
        template <typename T, typename ReadProperty, typename WriteProperty> void push             (std::string const& name, ReadProperty, WriteProperty) ;

    private:
        property_accessor_map& accessors_        ;
        property_indexes       property_indexes_ ;
    };

}}


namespace rubetek {
namespace module  {

    template <typename T>
    definite_property_accessor<T>::definite_property_accessor(read_property_type read, write_property_type write)
        : read_  ( read  )
        , write_ ( write )
    {}

    template <typename T>
    typename definite_property_accessor<T>::value_type definite_property_accessor<T>::read()
    {
        if (!read_) throw std::runtime_error("property can't read");
        return read_();
    }
    template <typename T>
    void definite_property_accessor<T>::write(value_type const& value)
    {
        if (!write_) throw std::runtime_error("property can't write");
        write_(value.unpack<T>());
    }

}}


namespace rubetek {
namespace module  {

    property_indexes::property_indexes()
        : utility::string_map<std::size_t>("property")
    {}

}}


namespace rubetek {
namespace module  {

    inline property_manager::property_manager(property_accessor_map& accessors, property_map const& property_map)
        : accessors_(accessors)
    {
        for (auto const& prop : property_map)
            property_indexes_.add(prop.second, prop.first);
    }
    inline property_manager::property_manager(property_manager&& rhs)
        : accessors_        ( rhs.accessors_                   )
        , property_indexes_ ( std::move(rhs.property_indexes_) )
    {}

    template <typename T, typename ReadProperty>
    void property_manager::push(std::string const& name, ReadProperty read_property, int)
    {
        push<T>(name, read_property, typename definite_property_accessor<T>::write_property_type());
    }

    template <typename T, typename WriteProperty>
    void property_manager::push(std::string const& name, int, WriteProperty write_property)
    {
        push<T>(name, typename definite_property_accessor<T>::read_property_type(), write_property);
    }

    template <typename T, typename ReadProperty, typename WriteProperty>
    void property_manager::push(std::string const& name, ReadProperty read_property, WriteProperty write_property)
    {
        std::size_t const index = property_indexes_.get(name);
        property_accessor_ptr& accessor_ptr = accessors_[index];
        if (accessor_ptr) throw std::runtime_error("property '" + name + "' already exist");
        else accessor_ptr = property_accessor_ptr(new definite_property_accessor<T>(read_property, write_property));
    }

}}
