/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef script_TYPES_H
#define script_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>



namespace rubetek { namespace tag { namespace script {

struct COMMAND {
  enum type {
    REMOVE_SCRIPT = 600
  };
};

extern const std::map<int, const char*> _COMMAND_VALUES_TO_NAMES;

typedef struct _RemoveScript__isset {
  _RemoveScript__isset() : id(false) {}
  bool id;
} _RemoveScript__isset;

class RemoveScript {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  RemoveScript() : id() {
  }

  virtual ~RemoveScript() throw() {}

  std::string id;

  _RemoveScript__isset __isset;

  void __set_id(const std::string& val) {
    id = val;
  }

  bool operator == (const RemoveScript & rhs) const
  {
    if (!(id == rhs.id))
      return false;
    return true;
  }
  bool operator != (const RemoveScript &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoveScript & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(RemoveScript &a, RemoveScript &b);

}}} // namespace

#endif