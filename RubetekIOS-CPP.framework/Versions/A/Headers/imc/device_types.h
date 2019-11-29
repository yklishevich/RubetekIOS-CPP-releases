/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef device_TYPES_H
#define device_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include "property_types.h"


namespace rubetek { namespace proto { namespace imc {

typedef struct _deivce_info__isset {
  _deivce_info__isset() : id(false), properties(false) {}
  bool id;
  bool properties;
} _deivce_info__isset;

class deivce_info {
 public:

  static const char* ascii_fingerprint; // = "5718DCEBD0FE4AB821A39DAEE61D0CF8";
  static const uint8_t binary_fingerprint[16]; // = {0x57,0x18,0xDC,0xEB,0xD0,0xFE,0x4A,0xB8,0x21,0xA3,0x9D,0xAE,0xE6,0x1D,0x0C,0xF8};

  deivce_info() : id() {
  }

  virtual ~deivce_info() throw() {}

  std::string id;
  std::vector< ::rubetek::proto::imc::property_info>  properties;

  _deivce_info__isset __isset;

  void __set_id(const std::string& val) {
    id = val;
  }

  void __set_properties(const std::vector< ::rubetek::proto::imc::property_info> & val) {
    properties = val;
  }

  bool operator == (const deivce_info & rhs) const
  {
    if (!(id == rhs.id))
      return false;
    if (!(properties == rhs.properties))
      return false;
    return true;
  }
  bool operator != (const deivce_info &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const deivce_info & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(deivce_info &a, deivce_info &b);

}}} // namespace

#endif
