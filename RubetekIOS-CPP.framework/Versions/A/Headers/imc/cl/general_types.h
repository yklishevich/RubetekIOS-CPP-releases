/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef general_TYPES_H
#define general_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include "version_types.h"


namespace rubetek { namespace proto {

struct Sender {
  enum type {
    CONTROLLER = 0,
    CLIENT = 1,
    EDITOR = 2,
    SERVER = 3,
    VIDEO_RELAY = 4
  };
};

extern const std::map<int, const char*> _Sender_VALUES_TO_NAMES;

struct PacketType {
  enum type {
    CONNECT = 0,
    OPEN = 1,
    SESSION = 2,
    CLOSE = 3,
    PAYLOAD = 4,
    PING = 5,
    EDITOR = 6,
    RESPONSE = 7,
    VER = 8,
    UNAVAILABLE = 9,
    CONTROLLER_QUERY = 10,
    CONTROLLER_RESPONSE = 11,
    CONTROLLER_BROADCAST = 12,
    AUXILIARY = 16,
    SESSION_TRANSFER = 17
  };
};

extern const std::map<int, const char*> _PacketType_VALUES_TO_NAMES;

struct StatusCode {
  enum type {
    OK = 0,
    ERR = 1
  };
};

extern const std::map<int, const char*> _StatusCode_VALUES_TO_NAMES;

typedef struct _Packet__isset {
  _Packet__isset() : type(false), sender(false), payload(false) {}
  bool type;
  bool sender;
  bool payload;
} _Packet__isset;

class Packet {
 public:

  static const char* ascii_fingerprint; // = "19AAB18E981C4EB11AEBB34F40FF0939";
  static const uint8_t binary_fingerprint[16]; // = {0x19,0xAA,0xB1,0x8E,0x98,0x1C,0x4E,0xB1,0x1A,0xEB,0xB3,0x4F,0x40,0xFF,0x09,0x39};

  Packet() : type((PacketType::type)0), sender((Sender::type)0), payload() {
  }

  virtual ~Packet() throw() {}

  PacketType::type type;
  Sender::type sender;
  std::string payload;

  _Packet__isset __isset;

  void __set_type(const PacketType::type val) {
    type = val;
  }

  void __set_sender(const Sender::type val) {
    sender = val;
  }

  void __set_payload(const std::string& val) {
    payload = val;
    __isset.payload = true;
  }

  bool operator == (const Packet & rhs) const
  {
    if (!(type == rhs.type))
      return false;
    if (!(sender == rhs.sender))
      return false;
    if (__isset.payload != rhs.__isset.payload)
      return false;
    else if (__isset.payload && !(payload == rhs.payload))
      return false;
    return true;
  }
  bool operator != (const Packet &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Packet & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Packet &a, Packet &b);

typedef struct _Response__isset {
  _Response__isset() : code(false), message(false) {}
  bool code;
  bool message;
} _Response__isset;

class Response {
 public:

  static const char* ascii_fingerprint; // = "24652790C81ECE22B629CB60A19F1E93";
  static const uint8_t binary_fingerprint[16]; // = {0x24,0x65,0x27,0x90,0xC8,0x1E,0xCE,0x22,0xB6,0x29,0xCB,0x60,0xA1,0x9F,0x1E,0x93};

  Response() : code((StatusCode::type)0), message() {
  }

  virtual ~Response() throw() {}

  StatusCode::type code;
  std::string message;

  _Response__isset __isset;

  void __set_code(const StatusCode::type val) {
    code = val;
  }

  void __set_message(const std::string& val) {
    message = val;
    __isset.message = true;
  }

  bool operator == (const Response & rhs) const
  {
    if (!(code == rhs.code))
      return false;
    if (__isset.message != rhs.__isset.message)
      return false;
    else if (__isset.message && !(message == rhs.message))
      return false;
    return true;
  }
  bool operator != (const Response &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Response & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Response &a, Response &b);

typedef struct _CloseSession__isset {
  _CloseSession__isset() : session_id(false) {}
  bool session_id;
} _CloseSession__isset;

class CloseSession {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  CloseSession() : session_id() {
  }

  virtual ~CloseSession() throw() {}

  std::string session_id;

  _CloseSession__isset __isset;

  void __set_session_id(const std::string& val) {
    session_id = val;
  }

  bool operator == (const CloseSession & rhs) const
  {
    if (!(session_id == rhs.session_id))
      return false;
    return true;
  }
  bool operator != (const CloseSession &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CloseSession & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(CloseSession &a, CloseSession &b);

typedef struct _Payload__isset {
  _Payload__isset() : session_id(false), data(false) {}
  bool session_id;
  bool data;
} _Payload__isset;

class Payload {
 public:

  static const char* ascii_fingerprint; // = "07A9615F837F7D0A952B595DD3020972";
  static const uint8_t binary_fingerprint[16]; // = {0x07,0xA9,0x61,0x5F,0x83,0x7F,0x7D,0x0A,0x95,0x2B,0x59,0x5D,0xD3,0x02,0x09,0x72};

  Payload() : session_id(), data() {
  }

  virtual ~Payload() throw() {}

  std::string session_id;
  std::string data;

  _Payload__isset __isset;

  void __set_session_id(const std::string& val) {
    session_id = val;
  }

  void __set_data(const std::string& val) {
    data = val;
  }

  bool operator == (const Payload & rhs) const
  {
    if (!(session_id == rhs.session_id))
      return false;
    if (!(data == rhs.data))
      return false;
    return true;
  }
  bool operator != (const Payload &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Payload & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Payload &a, Payload &b);

}} // namespace

#endif
