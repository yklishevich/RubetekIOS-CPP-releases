/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef cl_protocol_TYPES_H
#define cl_protocol_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>



namespace rubetek { namespace proto {

typedef struct _CL_Connect__isset {
  _CL_Connect__isset() : client_id(false), controller_ids(false) {}
  bool client_id;
  bool controller_ids;
} _CL_Connect__isset;

class CL_Connect {
 public:

  static const char* ascii_fingerprint; // = "09A67A266242E872217E8BB1F6E483B3";
  static const uint8_t binary_fingerprint[16]; // = {0x09,0xA6,0x7A,0x26,0x62,0x42,0xE8,0x72,0x21,0x7E,0x8B,0xB1,0xF6,0xE4,0x83,0xB3};

  CL_Connect() : client_id() {
  }

  virtual ~CL_Connect() throw() {}

  std::string client_id;
  std::vector<std::string>  controller_ids;

  _CL_Connect__isset __isset;

  void __set_client_id(const std::string& val) {
    client_id = val;
  }

  void __set_controller_ids(const std::vector<std::string> & val) {
    controller_ids = val;
    __isset.controller_ids = true;
  }

  bool operator == (const CL_Connect & rhs) const
  {
    if (!(client_id == rhs.client_id))
      return false;
    if (__isset.controller_ids != rhs.__isset.controller_ids)
      return false;
    else if (__isset.controller_ids && !(controller_ids == rhs.controller_ids))
      return false;
    return true;
  }
  bool operator != (const CL_Connect &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CL_Connect & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(CL_Connect &a, CL_Connect &b);

typedef struct _CL_OpenSession__isset {
  _CL_OpenSession__isset() : controller_id(false) {}
  bool controller_id;
} _CL_OpenSession__isset;

class CL_OpenSession {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  CL_OpenSession() : controller_id() {
  }

  virtual ~CL_OpenSession() throw() {}

  std::string controller_id;

  _CL_OpenSession__isset __isset;

  void __set_controller_id(const std::string& val) {
    controller_id = val;
  }

  bool operator == (const CL_OpenSession & rhs) const
  {
    if (!(controller_id == rhs.controller_id))
      return false;
    return true;
  }
  bool operator != (const CL_OpenSession &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CL_OpenSession & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(CL_OpenSession &a, CL_OpenSession &b);

typedef struct _CL_Session__isset {
  _CL_Session__isset() : controller_id(false), session_id(false), controller_ip(false) {}
  bool controller_id;
  bool session_id;
  bool controller_ip;
} _CL_Session__isset;

class CL_Session {
 public:

  static const char* ascii_fingerprint; // = "AB879940BD15B6B25691265F7384B271";
  static const uint8_t binary_fingerprint[16]; // = {0xAB,0x87,0x99,0x40,0xBD,0x15,0xB6,0xB2,0x56,0x91,0x26,0x5F,0x73,0x84,0xB2,0x71};

  CL_Session() : controller_id(), session_id(), controller_ip() {
  }

  virtual ~CL_Session() throw() {}

  std::string controller_id;
  std::string session_id;
  std::string controller_ip;

  _CL_Session__isset __isset;

  void __set_controller_id(const std::string& val) {
    controller_id = val;
  }

  void __set_session_id(const std::string& val) {
    session_id = val;
  }

  void __set_controller_ip(const std::string& val) {
    controller_ip = val;
  }

  bool operator == (const CL_Session & rhs) const
  {
    if (!(controller_id == rhs.controller_id))
      return false;
    if (!(session_id == rhs.session_id))
      return false;
    if (!(controller_ip == rhs.controller_ip))
      return false;
    return true;
  }
  bool operator != (const CL_Session &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CL_Session & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(CL_Session &a, CL_Session &b);

typedef struct _CL_Unavailable__isset {
  _CL_Unavailable__isset() : controller_id(false) {}
  bool controller_id;
} _CL_Unavailable__isset;

class CL_Unavailable {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  CL_Unavailable() : controller_id() {
  }

  virtual ~CL_Unavailable() throw() {}

  std::string controller_id;

  _CL_Unavailable__isset __isset;

  void __set_controller_id(const std::string& val) {
    controller_id = val;
  }

  bool operator == (const CL_Unavailable & rhs) const
  {
    if (!(controller_id == rhs.controller_id))
      return false;
    return true;
  }
  bool operator != (const CL_Unavailable &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CL_Unavailable & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(CL_Unavailable &a, CL_Unavailable &b);

}} // namespace

#endif
