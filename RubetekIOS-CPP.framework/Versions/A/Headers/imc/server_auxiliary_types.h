/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef server_auxiliary_TYPES_H
#define server_auxiliary_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>



namespace rubetek { namespace proto {

struct Target {
  enum type {
    SELF = 0,
    CLIENT = 1,
    CONTROLLER = 2,
    SERVICE = 3
  };
};

extern const std::map<int, const char*> _Target_VALUES_TO_NAMES;

typedef struct _PktRoute__isset {
  _PktRoute__isset() : target(false), session_id(false), payload_type(false), payload(false) {}
  bool target;
  bool session_id;
  bool payload_type;
  bool payload;
} _PktRoute__isset;

class PktRoute {
 public:

  static const char* ascii_fingerprint; // = "332552C58C9EF8C11AB089DD19C59E75";
  static const uint8_t binary_fingerprint[16]; // = {0x33,0x25,0x52,0xC5,0x8C,0x9E,0xF8,0xC1,0x1A,0xB0,0x89,0xDD,0x19,0xC5,0x9E,0x75};

  PktRoute() : target((Target::type)0), session_id(), payload_type(0), payload() {
  }

  virtual ~PktRoute() throw() {}

  Target::type target;
  std::string session_id;
  int32_t payload_type;
  std::string payload;

  _PktRoute__isset __isset;

  void __set_target(const Target::type val) {
    target = val;
  }

  void __set_session_id(const std::string& val) {
    session_id = val;
  }

  void __set_payload_type(const int32_t val) {
    payload_type = val;
  }

  void __set_payload(const std::string& val) {
    payload = val;
  }

  bool operator == (const PktRoute & rhs) const
  {
    if (!(target == rhs.target))
      return false;
    if (!(session_id == rhs.session_id))
      return false;
    if (!(payload_type == rhs.payload_type))
      return false;
    if (!(payload == rhs.payload))
      return false;
    return true;
  }
  bool operator != (const PktRoute &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PktRoute & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(PktRoute &a, PktRoute &b);

}} // namespace

#endif