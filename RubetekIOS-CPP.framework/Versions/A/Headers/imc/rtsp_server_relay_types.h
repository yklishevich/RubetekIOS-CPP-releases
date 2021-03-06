/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef rtsp_server_relay_TYPES_H
#define rtsp_server_relay_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>



namespace rubetek { namespace tag { namespace rtsp_server_relay {

struct COMMAND {
  enum type {
    REQUEST_VIDEO = 1400,
    START_PUSH_VIDEO = 1401,
    STOP_PUSH_VIDEO = 1402,
    VIDEO_READY = 1403,
    ERROR = 1404
  };
};

extern const std::map<int, const char*> _COMMAND_VALUES_TO_NAMES;

struct ERROR {
  enum type {
    CAN_NOT_START_TRANSMITTER = 0,
    CAN_NOT_FIND_SRC_STREAM = 1,
    SERVER_TOO_BUSY = 2
  };
};

extern const std::map<int, const char*> _ERROR_VALUES_TO_NAMES;

typedef struct _RequestVideoInfo__isset {
  _RequestVideoInfo__isset() : cc_id(false), local_uri(false) {}
  bool cc_id;
  bool local_uri;
} _RequestVideoInfo__isset;

class RequestVideoInfo {
 public:

  static const char* ascii_fingerprint; // = "07A9615F837F7D0A952B595DD3020972";
  static const uint8_t binary_fingerprint[16]; // = {0x07,0xA9,0x61,0x5F,0x83,0x7F,0x7D,0x0A,0x95,0x2B,0x59,0x5D,0xD3,0x02,0x09,0x72};

  RequestVideoInfo() : cc_id(), local_uri() {
  }

  virtual ~RequestVideoInfo() throw() {}

  std::string cc_id;
  std::string local_uri;

  _RequestVideoInfo__isset __isset;

  void __set_cc_id(const std::string& val) {
    cc_id = val;
  }

  void __set_local_uri(const std::string& val) {
    local_uri = val;
  }

  bool operator == (const RequestVideoInfo & rhs) const
  {
    if (!(cc_id == rhs.cc_id))
      return false;
    if (!(local_uri == rhs.local_uri))
      return false;
    return true;
  }
  bool operator != (const RequestVideoInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RequestVideoInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(RequestVideoInfo &a, RequestVideoInfo &b);

typedef struct _PushVideoInfo__isset {
  _PushVideoInfo__isset() : uri_src(false), uri_dest(false) {}
  bool uri_src;
  bool uri_dest;
} _PushVideoInfo__isset;

class PushVideoInfo {
 public:

  static const char* ascii_fingerprint; // = "07A9615F837F7D0A952B595DD3020972";
  static const uint8_t binary_fingerprint[16]; // = {0x07,0xA9,0x61,0x5F,0x83,0x7F,0x7D,0x0A,0x95,0x2B,0x59,0x5D,0xD3,0x02,0x09,0x72};

  PushVideoInfo() : uri_src(), uri_dest() {
  }

  virtual ~PushVideoInfo() throw() {}

  std::string uri_src;
  std::string uri_dest;

  _PushVideoInfo__isset __isset;

  void __set_uri_src(const std::string& val) {
    uri_src = val;
  }

  void __set_uri_dest(const std::string& val) {
    uri_dest = val;
  }

  bool operator == (const PushVideoInfo & rhs) const
  {
    if (!(uri_src == rhs.uri_src))
      return false;
    if (!(uri_dest == rhs.uri_dest))
      return false;
    return true;
  }
  bool operator != (const PushVideoInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PushVideoInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(PushVideoInfo &a, PushVideoInfo &b);

typedef struct _VideoUri__isset {
  _VideoUri__isset() : uri(false) {}
  bool uri;
} _VideoUri__isset;

class VideoUri {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  VideoUri() : uri() {
  }

  virtual ~VideoUri() throw() {}

  std::string uri;

  _VideoUri__isset __isset;

  void __set_uri(const std::string& val) {
    uri = val;
  }

  bool operator == (const VideoUri & rhs) const
  {
    if (!(uri == rhs.uri))
      return false;
    return true;
  }
  bool operator != (const VideoUri &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const VideoUri & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(VideoUri &a, VideoUri &b);

typedef struct _VideoReady__isset {
  _VideoReady__isset() : play_uri(false), local_uri(false) {}
  bool play_uri;
  bool local_uri;
} _VideoReady__isset;

class VideoReady {
 public:

  static const char* ascii_fingerprint; // = "07A9615F837F7D0A952B595DD3020972";
  static const uint8_t binary_fingerprint[16]; // = {0x07,0xA9,0x61,0x5F,0x83,0x7F,0x7D,0x0A,0x95,0x2B,0x59,0x5D,0xD3,0x02,0x09,0x72};

  VideoReady() : play_uri(), local_uri() {
  }

  virtual ~VideoReady() throw() {}

  std::string play_uri;
  std::string local_uri;

  _VideoReady__isset __isset;

  void __set_play_uri(const std::string& val) {
    play_uri = val;
  }

  void __set_local_uri(const std::string& val) {
    local_uri = val;
  }

  bool operator == (const VideoReady & rhs) const
  {
    if (!(play_uri == rhs.play_uri))
      return false;
    if (!(local_uri == rhs.local_uri))
      return false;
    return true;
  }
  bool operator != (const VideoReady &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const VideoReady & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(VideoReady &a, VideoReady &b);

typedef struct _Error__isset {
  _Error__isset() : error(false), str_error(false) {}
  bool error;
  bool str_error;
} _Error__isset;

class Error {
 public:

  static const char* ascii_fingerprint; // = "3F5FC93B338687BC7235B1AB103F47B3";
  static const uint8_t binary_fingerprint[16]; // = {0x3F,0x5F,0xC9,0x3B,0x33,0x86,0x87,0xBC,0x72,0x35,0xB1,0xAB,0x10,0x3F,0x47,0xB3};

  Error() : error(0), str_error() {
  }

  virtual ~Error() throw() {}

  int32_t error;
  std::string str_error;

  _Error__isset __isset;

  void __set_error(const int32_t val) {
    error = val;
  }

  void __set_str_error(const std::string& val) {
    str_error = val;
  }

  bool operator == (const Error & rhs) const
  {
    if (!(error == rhs.error))
      return false;
    if (!(str_error == rhs.str_error))
      return false;
    return true;
  }
  bool operator != (const Error &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Error & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Error &a, Error &b);

}}} // namespace

#endif
